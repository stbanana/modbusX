/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Handle.c
 * Author        : yono
 * Date          : 2024-08-02
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus 主机消息系统的底层消息处理, 内部函数, 不应由用户调用
     此时应当已经提取了完整指令
     系列函数使用标准返回
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#if MBX_MODULE_TCP_MASTER_ENABLE
#define MASTER_PARSE_SLAVEID(pMB)       pMB->Parse.SlaveID[pMB->Parse.Tail]
#define MASTER_PARSE_SENDFUNC(pMB)      pMB->Parse.SendFunc[pMB->Parse.Tail]
#define MASTER_PARSE_SENDADDRSTART(pMB) pMB->Parse.SendAddrStart[pMB->Parse.Tail]
#define MASTER_PARSE_SENDREGNUM(pMB)    pMB->Parse.SendRegNum[pMB->Parse.Tail]
#define MASTER_PARSE_SENDVALUE(pMB)     pMB->Parse.SendValue[pMB->Parse.Tail]
#else
#define MASTER_PARSE_SLAVEID(pMB)       pMB->Parse.SlaveID
#define MASTER_PARSE_SENDFUNC(pMB)      pMB->Parse.SendFunc
#define MASTER_PARSE_SENDADDRSTART(pMB) pMB->Parse.SendAddrStart
#define MASTER_PARSE_SENDREGNUM(pMB)    pMB->Parse.SendRegNum
#define MASTER_PARSE_SENDVALUE(pMB)     pMB->Parse.SendValue
#endif
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbus 主机消息系统的底层消息处理 读取一组线圈
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_READ_COIL_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(pMaster->RxExist.Buffer[3] & 0x01U)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
    {
        if(pMaster->RxExist.Buffer[3 + (i >> 3)] & (0x01 << (i % 8U)))
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 读取一组离散输入寄存器
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_READ_DISC_INPUTL_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(pMaster->RxExist.Buffer[3] & 0x01U)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
    {
        if(pMaster->RxExist.Buffer[3 + (i >> 3)] & (0x01 << (i % 8U)))
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 读取一个或多个保持寄存器
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_READ_REG_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), (((uint16_t)pMaster->RxExist.Buffer[3] << 8) + pMaster->RxExist.Buffer[4]), MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
    {
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, (((uint16_t)pMaster->RxExist.Buffer[3 + (i << 1)] << 8) + pMaster->RxExist.Buffer[4 + (i << 1)]), MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 读取一个或多个输入寄存器
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_READ_INPUT_REG_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), (((uint16_t)pMaster->RxExist.Buffer[3] << 8) + pMaster->RxExist.Buffer[4]), MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
    {
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, (((uint16_t)pMaster->RxExist.Buffer[3 + (i << 1)] << 8) + pMaster->RxExist.Buffer[4 + (i << 1)]), MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 写单个线圈
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_WRITE_COIL_Handle(_MBX_MASTER *pMaster)
{
    // 写入成功，同步映射数据
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(MASTER_PARSE_SENDVALUE(pMaster)[0] & 0x01U)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 写单个保持寄存器
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_WRITE_REG_Handle(_MBX_MASTER *pMaster)
{
    // 写入成功，同步映射数据
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    pMaster->Parse.RegData = ((uint16_t)MASTER_PARSE_SENDVALUE(pMaster)[0] << 8) | MASTER_PARSE_SENDVALUE(pMaster)[1];
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 写多个线圈
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_WRITE_COIL_MUL_Handle(_MBX_MASTER *pMaster)
{
    // 写入成功，同步映射数据
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(MASTER_PARSE_SENDVALUE(pMaster)[0] & 0x01U)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
    {
        if(MASTER_PARSE_SENDVALUE(pMaster)[i >> 3] & (0x01 << (i % 8U)))
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 写入多个保持寄存器
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_WRITE_REG_MUL_Handle(_MBX_MASTER *pMaster)
{
    // 写入成功，同步映射数据
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0;

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->RxExist.Buffer[0]);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }
    pMaster->Parse.RegData = ((uint16_t)MASTER_PARSE_SENDVALUE(pMaster)[0] << 8) | MASTER_PARSE_SENDVALUE(pMaster)[1];
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
    {
        pMaster->Parse.RegData = ((uint16_t)MASTER_PARSE_SENDVALUE(pMaster)[0 + (i << 1)] << 8) | MASTER_PARSE_SENDVALUE(pMaster)[1 + (i << 1)];
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 对于错误代码的处理
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_Error_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0; // 临时参数

    /* 审查功能码是否匹配发送 */
    if(pMaster->Parse.Func != (MASTER_PARSE_SENDFUNC(pMaster) + MBX_FUNC_EXCEPTION_OFFSET))
    {
        return MBX_API_RETURN_ERR_INDEFINITE;
    }

    /* 提取到实际错误码 */
    switch(pMaster->RxExist.Buffer[2])
    {
    case MBX_EXCEPTION_UNFUNC:
    case MBX_EXCEPTION_UNADDR:
    case MBX_EXCEPTION_DATA:
    case MBX_EXCEPTION_FAULT:
    case MBX_EXCEPTION_PARITY:
        MBxMasterErrortAdd(pMaster, MASTER_PARSE_SENDFUNC(pMaster), pMaster->RxExist.Buffer[2], MASTER_PARSE_SENDADDRSTART(pMaster), MASTER_PARSE_SENDREGNUM(pMaster));
        break;
    default:
        break;
    }

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, MASTER_PARSE_SLAVEID(pMaster));
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 写时错误处理 */
    switch(MASTER_PARSE_SENDFUNC(pMaster))
    {
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
        pMaster->Parse.RegData = ((uint16_t)MASTER_PARSE_SENDVALUE(pMaster)[0] << 8) | MASTER_PARSE_SENDVALUE(pMaster)[1];
        MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
        break;
    case MBX_FUNC_WRITE_COIL_MUL: /* 复杂的多线圈提取 */
        i = 0;
        if(MASTER_PARSE_SENDVALUE(pMaster)[0] & 0x01U)
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
        for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
        {
            if(MASTER_PARSE_SENDVALUE(pMaster)[i >> 3] & (0x01U << (i % 8U)))
            {
                pMaster->Parse.RegData = 0xFFFF;
            }
            else
            {
                pMaster->Parse.RegData = 0x0000;
            }
            MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
        }
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        pMaster->Parse.RegData = ((uint16_t)MASTER_PARSE_SENDVALUE(pMaster)[0] << 8) | MASTER_PARSE_SENDVALUE(pMaster)[1];
        MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster), pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
        for(i = 1; i < MASTER_PARSE_SENDREGNUM(pMaster); i++)
        {
            pMaster->Parse.RegData = ((uint16_t)MASTER_PARSE_SENDVALUE(pMaster)[0 + (i << 1)] << 8) | MASTER_PARSE_SENDVALUE(pMaster)[1 + (i << 1)];
            MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, MASTER_PARSE_SENDADDRSTART(pMaster) + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
        }
        break;

    default:
        break;
    }

    /* 清理所有处理请求 */
    MBx_utility_map_w_cooperate_review( );

    return MBX_API_RETURN_DEFAULT;
}

#undef MASTER_PARSE_SLAVEID
#undef MASTER_PARSE_SENDFUNC
#undef MASTER_PARSE_SENDADDRSTART
#undef MASTER_PARSE_SENDREGNUM
#undef MASTER_PARSE_SENDVALUE

#endif /* MBX_MASTER_ENABLE */
