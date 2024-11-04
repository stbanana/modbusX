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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(pMaster->RxExist.Buffer[3] & 0x01)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < pMaster->Parse.SendRegNum; i++)
    {
        if(pMaster->RxExist.Buffer[3 + (i >> 3)] & (0x01 << (i % 8)))
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(pMaster->RxExist.Buffer[3] & 0x01)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < pMaster->Parse.SendRegNum; i++)
    {
        if(pMaster->RxExist.Buffer[3 + (i >> 3)] & (0x01 << (i % 8)))
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, (((uint16_t)pMaster->RxExist.Buffer[3] << 8) + pMaster->RxExist.Buffer[4]), MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < pMaster->Parse.SendRegNum; i++)
    {
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, (((uint16_t)pMaster->RxExist.Buffer[3 + (i << 1)] << 8) + pMaster->RxExist.Buffer[4 + (i << 1)]), MBX_MAP_FIND_MODE_CONTINUOUS);
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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, (((uint16_t)pMaster->RxExist.Buffer[3] << 8) + pMaster->RxExist.Buffer[4]), MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < pMaster->Parse.SendRegNum; i++)
    {
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, (((uint16_t)pMaster->RxExist.Buffer[3 + (i << 1)] << 8) + pMaster->RxExist.Buffer[4 + (i << 1)]), MBX_MAP_FIND_MODE_CONTINUOUS);
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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(pMaster->Parse.SendValue[0] & 0x01)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);

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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    pMaster->Parse.RegData = ((uint16_t)pMaster->Parse.SendValue[0] << 8) | pMaster->Parse.SendValue[1];
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(pMaster->Parse.SendValue[0] & 0x01)
    {
        pMaster->Parse.RegData = 0xFFFF;
    }
    else
    {
        pMaster->Parse.RegData = 0x0000;
    }
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < pMaster->Parse.SendRegNum; i++)
    {
        if(pMaster->Parse.SendValue[i >> 3] & (0x01 << (i % 8)))
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
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
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }
    pMaster->Parse.RegData = ((uint16_t)pMaster->Parse.SendValue[0] << 8) | pMaster->Parse.SendValue[1];
    MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
    for(i = 1; i < pMaster->Parse.SendRegNum; i++)
    {
        pMaster->Parse.RegData = ((uint16_t)pMaster->Parse.SendValue[0 + (i << 1)] << 8) | pMaster->Parse.SendValue[1 + (i << 1)];
        MBx_utility_map_addr_data_write_cast(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 对于错误代码的处理
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_Error_rtu_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0; // 临时参数

    /* 审查功能码是否匹配发送 */
    if(pMaster->Parse.Func != (pMaster->Parse.SendFunc + MBX_FUNC_EXCEPTION_OFFSET))
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
        MBxMasterErrortAdd(pMaster, pMaster->Parse.SendFunc, pMaster->RxExist.Buffer[2], pMaster->Parse.SendAddrStart, pMaster->Parse.SendRegNum);
        break;
    default:
        break;
    }

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 写时错误处理 */
    switch(pMaster->Parse.SendFunc)
    {
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
        pMaster->Parse.RegData = ((uint16_t)pMaster->Parse.SendValue[0] << 8) | pMaster->Parse.SendValue[1];
        MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
        break;
    case MBX_FUNC_WRITE_COIL_MUL: /* 复杂的多线圈提取 */
        i = 0;
        if(pMaster->Parse.SendValue[0] & 0x01)
        {
            pMaster->Parse.RegData = 0xFFFF;
        }
        else
        {
            pMaster->Parse.RegData = 0x0000;
        }
        MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
        for(i = 1; i < pMaster->Parse.SendRegNum; i++)
        {
            if(pMaster->Parse.SendValue[i >> 3] & (0x01 << (i % 8)))
            {
                pMaster->Parse.RegData = 0xFFFF;
            }
            else
            {
                pMaster->Parse.RegData = 0x0000;
            }
            MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
        }
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        pMaster->Parse.RegData = ((uint16_t)pMaster->Parse.SendValue[0] << 8) | pMaster->Parse.SendValue[1];
        MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_FIRST);
        for(i = 1; i < pMaster->Parse.SendRegNum; i++)
        {
            pMaster->Parse.RegData = ((uint16_t)pMaster->Parse.SendValue[0 + (i << 1)] << 8) | pMaster->Parse.SendValue[1 + (i << 1)];
            MBx_utility_map_addr_data_write(pMember->Map, pMember->MapNum, pMaster->Parse.SendAddrStart + i, pMaster->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS);
        }
        break;

    default:
        break;
    }

    /* 清理所有处理请求 */
    MBx_utility_map_w_cooperate_review( );

    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief modbus 主机消息系统的底层消息处理 对于错误代码的处理
 * @param pMaster MBX主机对象指针
 * @return 标准返回
 */
uint32_t MBx_Master_Error_TCP_Handle(_MBX_MASTER *pMaster)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint16_t                 i = 0; // 临时参数

    /* 提取到实际错误码 */
    switch(pMaster->RxExist.Buffer[2])
    {
    case MBX_EXCEPTION_UNFUNC:
    case MBX_EXCEPTION_UNADDR:
    case MBX_EXCEPTION_DATA:
    case MBX_EXCEPTION_FAULT:
    case MBX_EXCEPTION_PARITY:
        MBxMasterErrortAdd(pMaster, pMaster->Parse.SendFunc, pMaster->RxExist.Buffer[2], pMaster->Parse.SendAddrStart, pMaster->Parse.SendRegNum);
        break;
    default:
        break;
    }

    /* 找到对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, pMaster->Parse.SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 写时错误处理 */
    switch(pMaster->Parse.Func - MBX_FUNC_EXCEPTION_OFFSET)
    {
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
        break;
    case MBX_FUNC_WRITE_COIL_MUL: /* 复杂的多线圈提取 */
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        break;
    default:
        break;
    }

    return MBX_API_RETURN_DEFAULT;
}

#endif /* MBX_MASTER_ENABLE */