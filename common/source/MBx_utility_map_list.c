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
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
/* Private types -------------------------------------------------------------*/

/**
 * @brief 协助处理写入请求的结构，避免多次触发同一内存区域的写入处理
 *          对于超越16位的程序变量，需要多个寄存器一同寄存。
 *          例如一个32位值，存在只写入高16位或低16位(此时只操作一个寄存器)、改变整个32位(写入两个寄存器)。
 *          这些请求可能来自单个寄存器写入请求、可能来自多个寄存器写入请求，为了方便得映射，做大一统处理。
 */
typedef struct
{
    void    *Memory;     // 内存区域头地址（寄存）
    uint8_t  Type;       // 内存区域映射的数据类型（寄存）
    uint16_t Puzzles[4]; // 最多4个16位碎片，用于处理32位、64位等数据类型
} _MBX_MAP_SET_DATA_ASSIT;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint16_t MBX_utility_map_entry_data_get(_MBX_MAP_LIST_ENTRY *entry);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 从指定映射表中查找指定地址的值
 * @param Map 地址映射表头
 * @param MapNum 地址映射数量
 * @param Addr 查找的地址
 * @param Data 获得映射到的值
 * @param Mode 查找模式 见 MBx_utility.h "定义map映射查找模式"
 * @return 标准返回
 */
uint32_t MBx_utility_map_addr_data_get(_MBX_MAP_LIST_ENTRY *Map, uint16_t MapNum, uint16_t Addr, uint16_t *Data, uint8_t mode)
{
    static uint16_t midReg;
    uint16_t        mid;
    uint32_t        state = MBX_API_RETURN_MAP_UNFIND;
    if(mode = MBX_MAP_FIND_MODE_FIRST) /* 首次查找，二分法迅速遍历 */
    {
        uint16_t left  = 0;
        uint16_t right = MapNum - 1;

        while((left <= right) &&                 // 条件1 未完全查找
              (Map[left].Addr > Addr) &&         // 条件2 左值节点addr已超越
              (state != MBX_API_RETURN_DEFAULT)) // 条件3 未找到
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
    else if(mode = MBX_MAP_FIND_MODE_CONTINUOUS) /* 继续查找，线性继续 */
    {
        for(mid = midReg;                      // 继续前次查找流程
            (mid < MapNum) &&                  // 条件1 未完全查找
            (Map[mid].Addr > Addr) &&          // 条件2 地址已越过 未找到
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

/***********内部工具***********/

/**
 * @brief 依据条目属性 返回对应的16位内存
 * @param entry 条目
 * @return 虚拟16位内存值
 */
static uint16_t MBX_utility_map_entry_data_get(_MBX_MAP_LIST_ENTRY *entry)
{
    switch(entry->Type)
    {
    case MBX_REG_TYPE_U8:
        return *(uint8_t *)entry->Memory;
    case MBX_REG_TYPE_U16:
        return *(uint16_t *)entry->Memory;
    case MBX_REG_TYPE_U32_L:
        return (*(uint32_t *)entry->Memory) & 0x0000FFFF;
    case MBX_REG_TYPE_U32_H:
        return ((*(uint32_t *)entry->Memory) >> 16) & 0x0000FFFF;
    case MBX_REG_TYPE_U64_0:
        return (*(uint64_t *)entry->Memory) & 0x000000000000FFFF;
    case MBX_REG_TYPE_U64_1:
        return ((*(uint64_t *)entry->Memory) >> 16) & 0x000000000000FFFF;
    case MBX_REG_TYPE_U64_2:
        return ((*(uint64_t *)entry->Memory) >> 32) & 0x000000000000FFFF;
    case MBX_REG_TYPE_U64_3:
        return ((*(uint64_t *)entry->Memory) >> 48) & 0x000000000000FFFF;
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
}
