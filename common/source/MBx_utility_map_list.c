/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_utility_map_list.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbusX 地址映射表处理模块
            在没有更好的查找映射方式前，采用静态地址表，二分法查找的方式进行映射
            TODO:存在多功能函数及变量，以及可合并为函数的部分，未来需优化
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
/* Private types -------------------------------------------------------------*/

/**
 * @brief 每个16位数据碎片的获得完成标识
 */
typedef struct
{
    uint8_t PuzzleL0:1; // 最低的16位碎片 BIT0
    uint8_t PuzzleL1:1; // 低1的16位碎片
    uint8_t PuzzleL2:1; // 低2的16位碎片
    uint8_t PuzzleL3:1; // 低3的16位碎片 BIT4
    uint8_t def     :4; // 填位
} _MBX_MAP_WRITE_DATA_PUZZLES_BIT_FLAG;
typedef union CanProtoMsgID
{
    uint8_t                              all;
    _MBX_MAP_WRITE_DATA_PUZZLES_BIT_FLAG bit;
} _MBX_MAP_PUZZLES_COMPLETE;

/**
 * @brief 协助处理写入请求的结构，避免多次触发同一内存区域的写入处理 cooperate-
 *          对于超越16位的程序变量，需要多个寄存器一同寄存。
 *          例如一个32位值，存在只写入高16位或低16位(此时只操作一个寄存器)、改变整个32位(写入两个寄存器)。
 *          这些请求可能来自单个寄存器写入请求、可能来自多个寄存器写入请求，为了方便得映射，做大一统处理。
 */
typedef struct
{
    void                     *Memory;          // 内存区域头地址(寄存)
    uint8_t                   Type;            // 内存区域映射的数据类型（寄存）
    MBX_MAP_REG_HANDLE        Handle;          // 写时处理函数(寄存)
    uint16_t                  Puzzles[4];      // 最多4个16位碎片，用于处理32位、64位等数据类型
    uint8_t                   PendingFlag;     // 是否有未完成的写入请求 1具有(写下方16位碎片时产生写入请求) 0无(处理函数调用后清除写入请求)
    _MBX_MAP_PUZZLES_COMPLETE PuzzlesComplete; // 16位碎片的填充完成标识
#if MBX_EXTEN_REG_HANDLE_ENABLE
    _MBX_REG_HANDLE_PARA *Para; // 写时传入协参
#endif
} _MBX_MAP_WRITE_DATA_CO;

/* Private macros ------------------------------------------------------------*/

#if MBX_EXTEN_REG_HANDLE_ENABLE
_MBX_REG_HANDLE_PARA DefaultPara;
#define __MBX_HANDLE_(p, d) p->Handle((void *)(p->Puzzles d), (p->Para))
#else
#define __MBX_HANDLE_(p, d) p->Handle((void *)(p->Puzzles d))
#endif
/* Private variables ---------------------------------------------------------*/

/* 
    连续性审查及映射查找共享的当前已查找序列，避免多次查找起始地址
    如果多线程支持，此变量应当包含入结构体，暂不支持多线程
 */
static uint16_t midReg = 0;

/* 
    写入协处理 
    如果多线程支持，此变量应当包含入结构体，暂不支持多线程
*/
_MBX_MAP_WRITE_DATA_CO MPxMapwriteCo = {0};

/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint16_t MBX_utility_map_entry_data_get(const _MBX_MAP_LIST_ENTRY *entry);
static uint32_t MBX_utility_map_entry_data_set(const _MBX_MAP_LIST_ENTRY *entry, _MBX_MAP_WRITE_DATA_CO *co, uint16_t value);
static uint32_t MBX_utility_map_entry_data_set_cast(const _MBX_MAP_LIST_ENTRY *entry, uint16_t value);
/* Private functions ---------------------------------------------------------*/

/***********映射读支持部分***********/
/**
 * @brief 从指定映射表中读指定地址的值
 * @param Map 地址映射表头
 * @param MapMax 地址映射总数量
 * @param Addr 查找的地址
 * @param Data 获得映射到的值
 * @param Mode 查找模式 见 MBx_utility.h "定义map映射查找模式"
 * @return 标准返回
 */
uint32_t MBx_utility_map_addr_data_read(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t Addr, uint16_t *Data, uint8_t mode)
{
    uint32_t state = MBX_API_RETURN_MAP_UNFIND;
    uint16_t mid   = 0;

    if(mode == MBX_MAP_FIND_MODE_FIRST) /* 首次查找，二分法迅速遍历 */
    {
        uint16_t left  = 0;
        uint16_t right = MapMax - 1;

        while((left <= right) &&                    // 条件1 未完全查找
              (Map[left].Addr <= Addr) &&           // 条件2 左值节点addr未超越
              (state == MBX_API_RETURN_MAP_UNFIND)) // 条件3 未找到
        {
            mid = left + ((right - left) >> 1);

            /* 计算下次的查找范围 */
            if(Map[mid].Addr < Addr)
            {
                left = mid + 1; // 继续查找右半部分
            }
            else if(Map[mid].Addr > Addr)
            {
                right = mid - 1; // 继续查找左半部分
            }
            else
            { /* 已查找到的分支 */
                midReg = mid;
                state  = MBX_API_RETURN_DEFAULT;
                *Data  = MBX_utility_map_entry_data_get(&Map[mid]);
            }
        }
    }
    else if(mode == MBX_MAP_FIND_MODE_CONTINUOUS) /* 继续查找，线性继续 */
    {
        for(mid = midReg;                      // 继续前次查找流程
            (mid < MapMax) &&                  // 条件1 未完全查找
            (Map[mid].Addr <= Addr) &&         // 条件2 地址未越过 未找到
            (state != MBX_API_RETURN_DEFAULT); // 条件3 未找到
            mid++)
        {
            if(Map[mid].Addr == Addr)
            { /* 已查找到的分支 */
                midReg = mid;
                state  = MBX_API_RETURN_DEFAULT;
                *Data  = MBX_utility_map_entry_data_get(&Map[mid]);
            }
        }
    }
    return state;
}

/**
 * @brief 从指定映射表中审查一段地址是否存在可读的映射定义 
 *              modbus没有只写不可读的地址，不审查Memory属性，哪怕是NULL也应可读
 * @param Map 地址映射表头
 * @param MapMax 地址映射总数量
 * @param AddrStart 查找的地址
 * @param RegNum 连续的寄存器数量
 * @return 标准返回
 */
uint32_t MBx_utility_map_r_continuity_review(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t AddrStart, uint16_t RegNum)
{
    uint32_t state = MBX_API_RETURN_MAP_UNFIND;
    uint16_t mid   = 0;
    uint16_t left  = 0;
    uint16_t right = MapMax - 1;

    while((left <= right) &&                    // 条件1 未完全查找
          (Map[left].Addr <= AddrStart) &&      // 条件2 左值节点addr未超越
          (state == MBX_API_RETURN_MAP_UNFIND)) // 条件3 未找到
    {
        mid = left + ((right - left) >> 1);

        /* 计算下次的查找范围 */
        if(Map[mid].Addr < AddrStart)
        {
            left = mid + 1; // 继续查找右半部分
        }
        else if(Map[mid].Addr > AddrStart)
        {
            right = mid - 1; // 继续查找左半部分
        }
        else
        { /* 已查找到的分支 */
            midReg = mid;
            state  = MBX_API_RETURN_DEFAULT;
        }
    }

    if(state != MBX_API_RETURN_DEFAULT) // 未找到起始址 return
    {
        return state;
    }

    for(left = 0;                        // 借助左值变量进行循环
        left < RegNum &&                 // 条件1 未足够审查
        state == MBX_API_RETURN_DEFAULT; // 条件2 未出现地址不连续
        left++)
    {
        if(Map[midReg + left].Addr != Map[midReg].Addr + left)
        {
            state = MBX_API_RETURN_MAP_UNFIND;
        }
    }

    return state;
}

/***********映射写支持部分***********/
/**
 * @brief 从指定映射表中写指定地址的值,触发写函数
 * @param Map 地址映射表头
 * @param MapMax 地址映射总数量
 * @param Addr 查找的地址
 * @param Data 获得映射到的值
 * @param Mode 查找模式 见 MBx_utility.h "定义map映射查找模式"
 * @return 标准返回
 */
uint32_t MBx_utility_map_addr_data_write(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t Addr, uint16_t Data, uint8_t mode)
{
    uint32_t state = MBX_API_RETURN_MAP_UNFIND;
    uint16_t mid   = 0;

    if(mode == MBX_MAP_FIND_MODE_FIRST) /* 首次查找，二分法迅速遍历 */
    {
        uint16_t left  = 0;
        uint16_t right = MapMax - 1;

        while((left <= right) &&                    // 条件1 未完全查找
              (Map[left].Addr <= Addr) &&           // 条件2 左值节点addr未超越
              (state == MBX_API_RETURN_MAP_UNFIND)) // 条件3 未找到
        {
            mid = left + ((right - left) >> 1);

            /* 计算下次的查找范围 */
            if(Map[mid].Addr < Addr)
            {
                left = mid + 1; // 继续查找右半部分
            }
            else if(Map[mid].Addr > Addr)
            {
                right = mid - 1; // 继续查找左半部分
            }
            else
            { /* 已查找到的分支 */
                midReg = mid;
                state  = MBX_utility_map_entry_data_set(&Map[mid], &MPxMapwriteCo, Data);
            }
        }
    }
    else if(mode == MBX_MAP_FIND_MODE_CONTINUOUS) /* 继续查找，线性继续 */
    {
        for(mid = midReg;                      // 继续前次查找流程
            (mid < MapMax) &&                  // 条件1 未完全查找
            (Map[mid].Addr <= Addr) &&         // 条件2 地址未越过
            (state != MBX_API_RETURN_DEFAULT); // 条件3 未找到
            mid++)
        {
            if(Map[mid].Addr == Addr)
            { /* 已查找到的分支 */
                midReg = mid;
                state  = MBX_utility_map_entry_data_set(&Map[mid], &MPxMapwriteCo, Data);
            }
        }
    }
    return state;
}

/**
 * @brief 从指定映射表中写指定地址的值,强制写内存
 * @param Map 地址映射表头
 * @param MapMax 地址映射总数量
 * @param Addr 查找的地址
 * @param Data 获得映射到的值
 * @param Mode 查找模式 见 MBx_utility.h "定义map映射查找模式"
 * @return 标准返回
 */
uint32_t MBx_utility_map_addr_data_write_cast(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t Addr, uint16_t Data, uint8_t mode)
{
    uint32_t state = MBX_API_RETURN_MAP_UNFIND;
    uint16_t mid   = 0;

    if(mode == MBX_MAP_FIND_MODE_FIRST) /* 首次查找，二分法迅速遍历 */
    {
        uint16_t left  = 0;
        uint16_t right = MapMax - 1;

        while((left <= right) &&                    // 条件1 未完全查找
              (Map[left].Addr <= Addr) &&           // 条件2 左值节点addr未超越
              (state == MBX_API_RETURN_MAP_UNFIND)) // 条件3 未找到
        {
            mid = left + ((right - left) >> 1);

            /* 计算下次的查找范围 */
            if(Map[mid].Addr < Addr)
            {
                left = mid + 1; // 继续查找右半部分
            }
            else if(Map[mid].Addr > Addr)
            {
                right = mid - 1; // 继续查找左半部分
            }
            else
            { /* 已查找到的分支 */
                midReg = mid;
                state  = MBX_utility_map_entry_data_set_cast(&Map[mid], Data);
            }
        }
    }
    else if(mode == MBX_MAP_FIND_MODE_CONTINUOUS) /* 继续查找，线性继续 */
    {
        for(mid = midReg;                      // 继续前次查找流程
            (mid < MapMax) &&                  // 条件1 未完全查找
            (Map[mid].Addr <= Addr) &&         // 条件2 地址未越过
            (state != MBX_API_RETURN_DEFAULT); // 条件3 未找到
            mid++)
        {
            if(Map[mid].Addr == Addr)
            { /* 已查找到的分支 */
                midReg = mid;
                state  = MBX_utility_map_entry_data_set_cast(&Map[mid], Data);
            }
        }
    }
    return state;
}

/**
 * @brief 从指定映射表中审查一段地址是否存在可写的映射定义 
 *              modbus写入，地址映射表采用回调函数形式处理写，没有回调函数代表不可写
 * @param Map 地址映射表头
 * @param MapMax 地址映射总数量
 * @param AddrStart 查找的地址
 * @param RegNum 连续的寄存器数量
 * @return 标准返回
 */
uint32_t MBx_utility_map_w_continuity_review(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t AddrStart, uint16_t RegNum)
{
    uint32_t state = MBX_API_RETURN_MAP_UNFIND;
    uint16_t mid   = 0;
    uint16_t left  = 0;
    uint16_t right = MapMax - 1;

    while((left <= right) &&                    // 条件1 未完全查找
          (Map[left].Addr <= AddrStart) &&      // 条件2 左值节点addr未超越
          (state == MBX_API_RETURN_MAP_UNFIND)) // 条件3 未找到
    {
        mid = left + ((right - left) >> 1);

        /* 计算下次的查找范围 */
        if(Map[mid].Addr < AddrStart)
        {
            left = mid + 1; // 继续查找右半部分
        }
        else if(Map[mid].Addr > AddrStart)
        {
            right = mid - 1; // 继续查找左半部分
        }
        else
        { /* 已查找到的分支 */
            midReg = mid;
            state  = MBX_API_RETURN_DEFAULT;
        }
    }
    if(state != MBX_API_RETURN_DEFAULT) // 未找到起始址 return
    {
        return state;
    }

    for(left = 0;                        // 借助左值变量进行循环
        left < RegNum &&                 // 条件1 未足够审查
        state == MBX_API_RETURN_DEFAULT; // 条件2 未出现地址不连续
        left++)
    {
        if(Map[midReg + left].Addr != Map[midReg].Addr + left || Map[midReg + left].Handle == NULL)
        {
            state = MBX_API_RETURN_MAP_UNFIND;
        }
    }

    return state;
}

/**
 * @brief 写入协处理未完成检查
 *              在退出写入函数前调用，处理完所有的写寄存器请求
 * @return 标准返回
 */
uint32_t MBx_utility_map_w_cooperate_review(void)
{
    uint32_t state = MBX_API_RETURN_DEFAULT;
    if(MPxMapwriteCo.PuzzlesComplete.all != 0xFF && MPxMapwriteCo.PendingFlag == 1)
    {
        MPxMapwriteCo.PendingFlag = 0;

        /* 使用源 Memory 填充至合适长度，触发写处理 */
#if !MBX_ENDIAN_MODE_BIG
        switch(MPxMapwriteCo.Type)
        {
        case MBX_REG_TYPE_U32_L:
        case MBX_REG_TYPE_U32_H: // 需2个16位碎片
            if(MPxMapwriteCo.PuzzlesComplete.all != 0xFF)
            {
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL0 == 0)
                {
                    MPxMapwriteCo.Puzzles[0] = (*(uint16_t *)MPxMapwriteCo.Memory);
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL1 == 0)
                {
                    MPxMapwriteCo.Puzzles[1] = (*(uint16_t *)(MPxMapwriteCo.Memory + 2));
                }
            }
            break;
        case MBX_REG_TYPE_U64_0:
        case MBX_REG_TYPE_U64_1:
        case MBX_REG_TYPE_U64_2:
        case MBX_REG_TYPE_U64_3: // 需4个16位碎片
            if(MPxMapwriteCo.PuzzlesComplete.all != 0xFF)
            {
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL0 == 0)
                {
                    MPxMapwriteCo.Puzzles[0] = (*(uint16_t *)MPxMapwriteCo.Memory);
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL1 == 0)
                {
                    MPxMapwriteCo.Puzzles[1] = (*(uint16_t *)(MPxMapwriteCo.Memory + 2));
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL2 == 0)
                {
                    MPxMapwriteCo.Puzzles[2] = (*(uint16_t *)(MPxMapwriteCo.Memory + 4));
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL3 == 0)
                {
                    MPxMapwriteCo.Puzzles[3] = (*(uint16_t *)(MPxMapwriteCo.Memory + 6));
                }
            }
            break;
        default:
            break;
        }

        if(MPxMapwriteCo.Handle != NULL)
            state = __MBX_HANDLE_((&MPxMapwriteCo), +0);

#else
        switch(MPxMapwriteCo.Type)
        {
        case MBX_REG_TYPE_U32_L:
        case MBX_REG_TYPE_U32_H: // 需2个16位碎片
            if(MPxMapwriteCo.PuzzlesComplete.all != 0xFF)
            {
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL0 == 0)
                {
                    MPxMapwriteCo.Puzzles[0] = (*(uint16_t *)MPxMapwriteCo.Memory + 2);
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL1 == 0)
                {
                    MPxMapwriteCo.Puzzles[1] = (*(uint16_t *)(MPxMapwriteCo.Memory));
                }
            }
            break;
        case MBX_REG_TYPE_U64_0:
        case MBX_REG_TYPE_U64_1:
        case MBX_REG_TYPE_U64_2:
        case MBX_REG_TYPE_U64_3: // 需4个16位碎片
            if(MPxMapwriteCo.PuzzlesComplete.all != 0xFF)
            {
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL0 == 0)
                {
                    MPxMapwriteCo.Puzzles[0] = (*(uint16_t *)MPxMapwriteCo.Memory + 6);
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL1 == 0)
                {
                    MPxMapwriteCo.Puzzles[1] = (*(uint16_t *)(MPxMapwriteCo.Memory + 4));
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL2 == 0)
                {
                    MPxMapwriteCo.Puzzles[2] = (*(uint16_t *)(MPxMapwriteCo.Memory + 2));
                }
                if(MPxMapwriteCo.PuzzlesComplete.bit.PuzzleL3 == 0)
                {
                    MPxMapwriteCo.Puzzles[3] = (*(uint16_t *)(MPxMapwriteCo.Memory));
                }
            }
            break;
        default:
            break;
        }

        if(MPxMapwriteCo.Handle != NULL)
            switch(MPxMapwriteCo.Type)
            {
            case MBX_REG_TYPE_U8:
                state = __MBX_HANDLE_((&MPxMapwriteCo), +1);
                break;
            default:
                state = __MBX_HANDLE_((&MPxMapwriteCo), +0);
                break;
            }
#endif
    }
    return state;
}

/***********内部工具***********/
/**
 * @brief 依据条目属性 返回对应的16位内存
 * @param entry 条目
 * @return 虚拟16位内存值
 */
static uint16_t MBX_utility_map_entry_data_get(const _MBX_MAP_LIST_ENTRY *entry)
{
#if !MBX_ENDIAN_MODE_BIG
    switch(entry->Type)
    {
    case MBX_REG_TYPE_U8:
        return (*(uint8_t *)(entry->Memory));
    case MBX_REG_TYPE_U16:
        return (*(uint16_t *)(entry->Memory));
    case MBX_REG_TYPE_U32_L:
        return (*(uint16_t *)(entry->Memory));
    case MBX_REG_TYPE_U32_H:
        return (*(uint16_t *)(entry->Memory + 2));
    case MBX_REG_TYPE_U64_0:
        return (*(uint64_t *)(entry->Memory));
    case MBX_REG_TYPE_U64_1:
        return (*(uint64_t *)(entry->Memory + 2));
    case MBX_REG_TYPE_U64_2:
        return (*(uint64_t *)(entry->Memory + 4));
    case MBX_REG_TYPE_U64_3:
        return (*(uint64_t *)(entry->Memory + 6));
    case MBX_REG_TYPE_BIT_ONLY:
        return (((*(uint8_t *)entry->Memory)) & 0x1) ? 0x1 : 0x0;
    default:
        if(entry->Type >= MBX_REG_TYPE_BIT_U8_BASE && entry->Type < MBX_REG_TYPE_BIT_U8_BASE + 8)
        { /* 8位组合单bit */
            return (*(uint8_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U8_BASE));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U16_BASE && entry->Type < MBX_REG_TYPE_BIT_U16_BASE + 16)
        { /* 16位组合单bit */
            return (*(uint16_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U16_BASE));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U32_BASE && entry->Type < MBX_REG_TYPE_BIT_U32_BASE + 32)
        { /* 32位组合单bit */
            return (*(uint32_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U32_BASE));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U64_BASE && entry->Type < MBX_REG_TYPE_BIT_U64_BASE + 64)
        { /* 64位组合单bit */
            return (*(uint64_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U64_BASE));
        }
        else
        {
            return 0;
        }
    }
#else
    switch(entry->Type)
    {
    case MBX_REG_TYPE_U8:
        return (*(uint8_t *)(entry->Memory));
    case MBX_REG_TYPE_U16:
        return (*(uint16_t *)(entry->Memory));
    case MBX_REG_TYPE_U32_L:
        return (*(uint16_t *)(entry->Memory + 2));
    case MBX_REG_TYPE_U32_H:
        return (*(uint16_t *)(entry->Memory));
    case MBX_REG_TYPE_U64_0:
        return (*(uint64_t *)(entry->Memory + 6));
    case MBX_REG_TYPE_U64_1:
        return (*(uint64_t *)(entry->Memory + 4));
    case MBX_REG_TYPE_U64_2:
        return (*(uint64_t *)(entry->Memory + 2));
    case MBX_REG_TYPE_U64_3:
        return (*(uint64_t *)(entry->Memory));
    case MBX_REG_TYPE_BIT_ONLY:
        return (((*(uint8_t *)entry->Memory)) & 0x80) ? 0x1 : 0x0;
    default:
        if(entry->Type >= MBX_REG_TYPE_BIT_U8_BASE && entry->Type < MBX_REG_TYPE_BIT_U8_BASE + 8)
        { /* 8位组合单bit */
            return (*(uint8_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U8_BASE));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U16_BASE && entry->Type < MBX_REG_TYPE_BIT_U16_BASE + 16)
        { /* 16位组合单bit */
            return (*(uint16_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U16_BASE));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U32_BASE && entry->Type < MBX_REG_TYPE_BIT_U32_BASE + 32)
        { /* 32位组合单bit */
            return (*(uint32_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U32_BASE));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U64_BASE && entry->Type < MBX_REG_TYPE_BIT_U64_BASE + 64)
        { /* 64位组合单bit */
            return (*(uint64_t *)entry->Memory) & (1 << (entry->Type - MBX_REG_TYPE_BIT_U64_BASE));
        }
        else
        {
            return 0;
        }
    }
#endif
}

/**
 * @brief 依据条目属性 设置对应的16位内存 触发写函数
 * @param entry 条目
 * @param co 协处理协寄存
 * @param value 设置值
 * @return 标准返回
 */
static uint32_t MBX_utility_map_entry_data_set(const _MBX_MAP_LIST_ENTRY *entry, _MBX_MAP_WRITE_DATA_CO *co, uint16_t value)
{
    uint32_t state = MBX_API_RETURN_DEFAULT; // 默认处理成功，存在寄存请求而不处理的情况，处理函数调用时改变状态
    /* 未有已知待处理写请求，寄存并发起请求 */
    if(co->PendingFlag == 0)
    {
        co->Memory      = entry->Memory;
        co->Type        = entry->Type;
        co->Handle      = entry->Handle;
        co->PendingFlag = 1;
        switch(co->Type)
        {
        case MBX_REG_TYPE_U8:
        case MBX_REG_TYPE_U16: // 需1个16位碎片
            co->PuzzlesComplete.all = 0xFE;
            break;
        case MBX_REG_TYPE_U32_L:
        case MBX_REG_TYPE_U32_H: // 需2个16位碎片
            co->PuzzlesComplete.all = 0xFC;
            break;
        case MBX_REG_TYPE_U64_0:
        case MBX_REG_TYPE_U64_1:
        case MBX_REG_TYPE_U64_2:
        case MBX_REG_TYPE_U64_3: // 需4个16位碎片
            co->PuzzlesComplete.all = 0xF0;
            break;
        case MBX_REG_TYPE_BIT_ONLY: // 需1个16位碎片
            co->PuzzlesComplete.all = 0xFE;
            break;
        default:
            if(co->Type >= MBX_REG_TYPE_BIT_U8_BASE && co->Type < MBX_REG_TYPE_BIT_U8_BASE + 8) // 写bit立即处理,需1个16位碎片
            {
                co->PuzzlesComplete.all = 0xFE;
            }
            else if(co->Type >= MBX_REG_TYPE_BIT_U16_BASE && co->Type < MBX_REG_TYPE_BIT_U16_BASE + 16) // 需1个16位碎片
            {
                co->PuzzlesComplete.all = 0xFE;
            }
            else if(co->Type >= MBX_REG_TYPE_BIT_U32_BASE && co->Type < MBX_REG_TYPE_BIT_U32_BASE + 32) // 需1个16位碎片
            {
                co->PuzzlesComplete.all = 0xFE;
            }
            else if(co->Type >= MBX_REG_TYPE_BIT_U64_BASE && co->Type < MBX_REG_TYPE_BIT_U64_BASE + 64) // 需1个16位碎片
            {
                co->PuzzlesComplete.all = 0xFE;
            }
            else
            {
                co->PendingFlag = 0; // 类型不支持，错误调用，取消请求，返回错误
                state           = MBX_API_RETURN_ERR_PARAM;
            }
            break;
        }
#if MBX_EXTEN_REG_HANDLE_ENABLE /* 注册协参 */
        co->Para = entry->Para;
#endif
    }

#if !MBX_ENDIAN_MODE_BIG /* 小端拼图，大小端在碎片拼图中的填充不同 */
    switch(entry->Type)
    {
    case MBX_REG_TYPE_U8:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U16:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U32_L:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U32_H:
        co->Puzzles[1]                   = value;
        co->PuzzlesComplete.bit.PuzzleL1 = 1;
        break;
    case MBX_REG_TYPE_U64_0:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U64_1:
        co->Puzzles[1]                   = value;
        co->PuzzlesComplete.bit.PuzzleL1 = 1;
        break;
    case MBX_REG_TYPE_U64_2:
        co->Puzzles[2]                   = value;
        co->PuzzlesComplete.bit.PuzzleL2 = 1;
        break;
    case MBX_REG_TYPE_U64_3:
        co->Puzzles[3]                   = value;
        co->PuzzlesComplete.bit.PuzzleL3 = 1;
        break;
    case MBX_REG_TYPE_BIT_ONLY:
        co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    default:
        if(entry->Type >= MBX_REG_TYPE_BIT_U8_BASE && entry->Type < MBX_REG_TYPE_BIT_U8_BASE + 8)
        {
            // *(uint8_t *)co->Puzzles[0] |= (((uint8_t)(!!value)) << (entry->Type - MBX_REG_TYPE_BIT_U8_BASE)); // 拼凑，但bit寄存器应当立即处理
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U16_BASE && entry->Type < MBX_REG_TYPE_BIT_U16_BASE + 16)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U32_BASE && entry->Type < MBX_REG_TYPE_BIT_U32_BASE + 32)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U64_BASE && entry->Type < MBX_REG_TYPE_BIT_U64_BASE + 64)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        break;
    }

    /* 检查请求处理完毕，触发写处理 */
    if(co->PuzzlesComplete.all == 0xFF && co->PendingFlag == 1)
    {
        co->PendingFlag = 0;

        if(co->Handle != NULL)
            state = __MBX_HANDLE_((co), +0);
    }
#else /* 大端拼图 */
    switch(entry->Type)
    {
    case MBX_REG_TYPE_U8:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U16:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U32_L:
        co->Puzzles[1]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U32_H:
        co->Puzzles[2]                   = value;
        co->PuzzlesComplete.bit.PuzzleL1 = 1;
        break;
    case MBX_REG_TYPE_U64_0:
        co->Puzzles[3]                   = value;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    case MBX_REG_TYPE_U64_1:
        co->Puzzles[2]                   = value;
        co->PuzzlesComplete.bit.PuzzleL1 = 1;
        break;
    case MBX_REG_TYPE_U64_2:
        co->Puzzles[1]                   = value;
        co->PuzzlesComplete.bit.PuzzleL2 = 1;
        break;
    case MBX_REG_TYPE_U64_3:
        co->Puzzles[0]                   = value;
        co->PuzzlesComplete.bit.PuzzleL3 = 1;
        break;
    case MBX_REG_TYPE_BIT_ONLY:
        co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
        co->PuzzlesComplete.bit.PuzzleL0 = 1;
        break;
    default:
        if(entry->Type >= MBX_REG_TYPE_BIT_U8_BASE && entry->Type < MBX_REG_TYPE_BIT_U8_BASE + 8)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U16_BASE && entry->Type < MBX_REG_TYPE_BIT_U16_BASE + 16)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U32_BASE && entry->Type < MBX_REG_TYPE_BIT_U32_BASE + 32)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U64_BASE && entry->Type < MBX_REG_TYPE_BIT_U64_BASE + 64)
        {
            co->Puzzles[0]                   = value ? 0xFFFF : 0x0000;
            co->PuzzlesComplete.bit.PuzzleL0 = 1;
        }
        break;
    }

    /* 检查请求处理完毕，触发写处理 */
    if(co->PuzzlesComplete.all == 0xFF && co->PendingFlag == 1)
    {
        co->PendingFlag = 0;

        if(co->Handle != NULL)
            switch(co->Type)
            {
            case MBX_REG_TYPE_U8:
                state = __MBX_HANDLE_((co), +1);
                break;
            default:
                state = __MBX_HANDLE_((co), +0);
                break;
            }
    }
#endif
    return state;
}

/**
 * @brief 依据条目属性 设置对应的16位内存 强制写内存
 * @param entry 条目
 * @param value 设置值
 * @return 标准返回
 */
static uint32_t MBX_utility_map_entry_data_set_cast(const _MBX_MAP_LIST_ENTRY *entry, uint16_t value)
{
    uint32_t state = MBX_API_RETURN_DEFAULT; // 默认处理成功，存在寄存请求而不处理的情况，处理函数调用时改变状态

#if !MBX_ENDIAN_MODE_BIG /* 小端强制写，大小端在内存中的排布不同 */
    switch(entry->Type)
    {
    case MBX_REG_TYPE_U8:
        *(uint8_t *)(entry->Memory) = value;
        break;
    case MBX_REG_TYPE_U16:
        *(uint16_t *)(entry->Memory) = value;
        break;
    case MBX_REG_TYPE_U32_L:
        *(uint16_t *)(entry->Memory) = value;
        break;
    case MBX_REG_TYPE_U32_H:
        *(uint16_t *)(entry->Memory + 2) = value;
        break;
    case MBX_REG_TYPE_U64_0:
        *(uint16_t *)(entry->Memory) = value;
        break;
    case MBX_REG_TYPE_U64_1:
        *(uint16_t *)(entry->Memory + 2) = value;
        break;
    case MBX_REG_TYPE_U64_2:
        *(uint16_t *)(entry->Memory + 4) = value;
        break;
    case MBX_REG_TYPE_U64_3:
        *(uint16_t *)(entry->Memory + 6) = value;
        break;
    case MBX_REG_TYPE_BIT_ONLY:
        *(uint8_t *)(entry->Memory) = value;
        break;
    default:
        if(entry->Type >= MBX_REG_TYPE_BIT_U8_BASE && entry->Type < MBX_REG_TYPE_BIT_U8_BASE + 8)
        {
            if(value > 0)
                *(uint8_t *)(entry->Memory) |= (0x01 << (entry->Type - MBX_REG_TYPE_BIT_U8_BASE));
            else
                *(uint8_t *)(entry->Memory) &= (~(0x01 << (entry->Type - MBX_REG_TYPE_BIT_U8_BASE)));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U16_BASE && entry->Type < MBX_REG_TYPE_BIT_U16_BASE + 16)
        {
            if(value > 0)
                *(uint16_t *)(entry->Memory) |= (0x01 << (entry->Type - MBX_REG_TYPE_BIT_U16_BASE));
            else
                *(uint16_t *)(entry->Memory) &= (~(0x01 << (entry->Type - MBX_REG_TYPE_BIT_U16_BASE)));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U32_BASE && entry->Type < MBX_REG_TYPE_BIT_U32_BASE + 32)
        {
            if(value > 0)
                *(uint32_t *)(entry->Memory) |= (0x01 << (entry->Type - MBX_REG_TYPE_BIT_U32_BASE));
            else
                *(uint32_t *)(entry->Memory) &= (~(0x01 << (entry->Type - MBX_REG_TYPE_BIT_U32_BASE)));
        }
        else if(entry->Type >= MBX_REG_TYPE_BIT_U64_BASE && entry->Type < MBX_REG_TYPE_BIT_U64_BASE + 64)
        {
            if(value > 0)
                *(uint64_t *)(entry->Memory) |= (0x01UL << (entry->Type - MBX_REG_TYPE_BIT_U64_BASE));
            else
                *(uint64_t *)(entry->Memory) &= (~(0x01UL << (entry->Type - MBX_REG_TYPE_BIT_U64_BASE)));
        }
        break;
    }
#else /* 大端拼图 */

#endif
    return state;
}

#undef __MBX_HANDLE_
