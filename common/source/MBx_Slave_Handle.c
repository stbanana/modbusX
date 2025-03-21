/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Handle.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus 从机消息系统的底层消息处理, 内部函数, 不应由用户调用
     此时应当已经提取了完整指令
     系列函数应当使用标准返回，但这样导致外层包装冗长，采用直接返回错误码
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbus 从机消息系统的底层消息处理 读取一组线圈
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_READ_COIL_Handle(_MBX_SLAVE *pSlave)
{
    uint8_t  ComboBit = 0;
    uint16_t i;

    /* 提取待解析的寄存器数量 */
    pSlave->Parse.RegNum = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];

    /* 审查查询数量是否合法 1~2000 */
    if(pSlave->Parse.RegNum < 0x0001 || pSlave->Parse.RegNum > 0x07D0)
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查查询范围是否均存在可读定义 */
    if((MBx_utility_map_r_continuity_review(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegNum)) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_UNADDR;
    }

    /* 提取查询 */
    for(i = 0; i < pSlave->Parse.RegNum; i++)
    {
        if((i & 0x0007) == 0)
            ComboBit = 0;

        if(MBx_utility_map_addr_data_read(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart + i, &pSlave->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS) != MBX_API_RETURN_DEFAULT)
        {
            return MBX_EXCEPTION_FAULT;
        }

        if(pSlave->Parse.RegData > 0)
            ComboBit |= (1 << (i & 0x0007));
        else
            ComboBit &= ~(1 << (i & 0x0007));

        if((i & 0x0007) == 0x0007)
            MBxTxBufferPutc(pSlave, ComboBit);
    }

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 读取一组离散输入寄存器
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_READ_DISC_INPUTL_Handle(_MBX_SLAVE *pSlave)
{
    uint8_t  ComboBit = 0;
    uint16_t i;

    /* 提取待解析的寄存器数量 */
    pSlave->Parse.RegNum = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];

    /* 审查查询数量是否合法 1~2000 */
    if(pSlave->Parse.RegNum < 0x0001 || pSlave->Parse.RegNum > 0x07D0)
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查查询范围是否均存在可读定义 */
    if((MBx_utility_map_r_continuity_review(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegNum)) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_UNADDR;
    }

    /* 提取查询 */
    for(i = 0; i < pSlave->Parse.RegNum; i++)
    {
        if((i & 0x0007) == 0)
            ComboBit = 0;

        if(MBx_utility_map_addr_data_read(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart + i, &pSlave->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS) != MBX_API_RETURN_DEFAULT)
        {
            return MBX_EXCEPTION_FAULT;
        }

        if(pSlave->Parse.RegData > 0)
            ComboBit |= (1 << (i & 0x0007));
        else
            ComboBit &= ~(1 << (i & 0x0007));

        if((i & 0x0007) == 0x0007)
            MBxTxBufferPutc(pSlave, ComboBit);
    }

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 读取一个或多个保持寄存器
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_READ_REG_Handle(_MBX_SLAVE *pSlave)
{
    uint16_t i;

    /* 提取待解析的寄存器数量 */
    pSlave->Parse.RegNum = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];

    /* 审查查询数量是否合法 1~2000 */
    if(pSlave->Parse.RegNum < 0x0001 || pSlave->Parse.RegNum > 0x07D0)
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查查询范围是否均存在可读定义 */
    if((MBx_utility_map_r_continuity_review(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegNum)) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_UNADDR;
    }
    /* 预填充回复流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);      // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func));        // 功能码
    MBxTxBufferPutc(pSlave, (pSlave->Parse.RegNum << 1)); // 字节数，即查询寄存器数量的2倍

    /* 提取查询 */
    for(i = 0; i < pSlave->Parse.RegNum; i++)
    {
        if(MBx_utility_map_addr_data_read(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart + i, &pSlave->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS) != MBX_API_RETURN_DEFAULT)
        {
            return MBX_EXCEPTION_FAULT;
        }
        MBxTxBufferPutReg(pSlave, pSlave->Parse.RegData);
    }

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 读取一个或多个输入寄存器
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_READ_INPUT_REG_Handle(_MBX_SLAVE *pSlave)
{
    uint16_t i;

    /* 提取待解析的寄存器数量 */
    pSlave->Parse.RegNum = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];

    /* 审查查询数量是否合法 1~2000 */
    if(pSlave->Parse.RegNum < 0x0001 || pSlave->Parse.RegNum > 0x07D0)
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查查询范围是否均存在可读定义 */
    if((MBx_utility_map_r_continuity_review(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegNum)) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_UNADDR;
    }

    /* 预填充回复流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);      // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func));        // 功能码
    MBxTxBufferPutc(pSlave, (pSlave->Parse.RegNum << 1)); // 字节数，即查询寄存器数量的2倍

    /* 提取查询 */
    for(i = 0; i < pSlave->Parse.RegNum; i++)
    {
        if(MBx_utility_map_addr_data_read(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart + i, &pSlave->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS) != MBX_API_RETURN_DEFAULT)
        {
            return MBX_EXCEPTION_FAULT;
        }
        MBxTxBufferPutReg(pSlave, pSlave->Parse.RegData);
    }

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 写单个线圈
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_WRITE_COIL_Handle(_MBX_SLAVE *pSlave)
{
    /* 预填充回复流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);      // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func));        // 功能码
    MBxTxBufferPutReg(pSlave, (pSlave->Parse.AddrStart)); // 寄存器地址

    /* 获得期望写入值 */
    pSlave->Parse.RegData = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) | (pSlave->RxExist.Buffer[5]);

    /* 审查写入值是否符合标准 */
    if((pSlave->Parse.RegData != 0xFF00) || (pSlave->Parse.RegData != 0x0000))
        return MBX_EXCEPTION_DATA;

    if(MBx_utility_map_addr_data_write(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegData, MBX_MAP_FIND_MODE_FIRST) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_FAULT;
    }
    if(MBx_utility_map_w_cooperate_review( ) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_FAULT;
    }

    MBxTxBufferPutReg(pSlave, pSlave->Parse.RegData);

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 写单个保持寄存器
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_WRITE_REG_Handle(_MBX_SLAVE *pSlave)
{
    /* 预填充回复流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);      // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func));        // 功能码
    MBxTxBufferPutReg(pSlave, (pSlave->Parse.AddrStart)); // 寄存器地址

    /* 获得期望写入值 */
    pSlave->Parse.RegData = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) | (pSlave->RxExist.Buffer[5]);

    if(MBx_utility_map_addr_data_write(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegData, MBX_MAP_FIND_MODE_FIRST) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_FAULT;
    }
    if(MBx_utility_map_w_cooperate_review( ) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_FAULT;
    }

    MBxTxBufferPutReg(pSlave, pSlave->Parse.RegData);

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 写多个线圈
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_WRITE_COIL_MUL_Handle(_MBX_SLAVE *pSlave)
{
    uint16_t i;
    uint16_t WriteData;

    /* 提取待解析的寄存器数量 */
    pSlave->Parse.RegNum = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];

    /* 审查写入数量是否合法 1~123 */
    if(pSlave->Parse.RegNum < 0x0001 || pSlave->Parse.RegNum > 0x007B)
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查写入寄存器数量与字节数信息是否匹配 */
    if(pSlave->RxExist.Buffer[6] != ((pSlave->Parse.RegNum >> 3) + (pSlave->Parse.RegNum & 0x07 ? 1 : 0)))
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查查询范围是否均存在定义 */
    if((MBx_utility_map_w_continuity_review(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegNum)) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_UNADDR;
    }

    /* 提取写入 */
    for(i = 0; i < pSlave->Parse.RegNum; i++)
    {
        /* 获得期望写入值 */
        pSlave->Parse.RegData = ((uint16_t)pSlave->RxExist.Buffer[7 + ((i & 0x0007) == 0x0000 ? (i >> 3) : ((i >> 3) + 1))]);
        if(pSlave->Parse.RegData == 0)
            WriteData = 0x0000;
        else
            WriteData = 0xFF00;
        if(MBx_utility_map_addr_data_write(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart + i, WriteData, MBX_MAP_FIND_MODE_CONTINUOUS) != MBX_API_RETURN_DEFAULT)
        {
            return MBX_EXCEPTION_FAULT;
        }
    }
    if(MBx_utility_map_w_cooperate_review( ) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_FAULT;
    }

    /* 填充回复流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);      // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func));        // 功能码
    MBxTxBufferPutReg(pSlave, (pSlave->Parse.AddrStart)); // 寄存器地址
    MBxTxBufferPutReg(pSlave, (pSlave->Parse.RegNum));    // 寄存器数

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 写入多个保持寄存器
 * @param pSlave MBX从机对象指针
 * @return 错误码返回
 */
uint32_t MBx_Slave_WRITE_REG_MUL_Handle(_MBX_SLAVE *pSlave)
{
    uint16_t i;

    /* 提取待解析的寄存器数量 */
    pSlave->Parse.RegNum = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];

    /* 审查写入数量是否合法 1~123 */
    if(pSlave->Parse.RegNum < 0x0001 || pSlave->Parse.RegNum > 0x007B)
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查写入寄存器数量与字节数信息是否匹配 */
    if(pSlave->RxExist.Buffer[6] != (pSlave->Parse.RegNum << 1))
    {
        return MBX_EXCEPTION_DATA;
    }

    /* 审查查询范围是否均存在定义 */
    if((MBx_utility_map_w_continuity_review(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart, pSlave->Parse.RegNum)) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_UNADDR;
    }

    /* 提取写入 */
    for(i = 0; i < pSlave->Parse.RegNum; i++)
    {
        /* 获得期望写入值 */
        pSlave->Parse.RegData = ((uint16_t)pSlave->RxExist.Buffer[7 + (i << 1)] << 8) | (pSlave->RxExist.Buffer[8 + (i << 1)]);
        if(MBx_utility_map_addr_data_write(pSlave->Config.Map, pSlave->Config.MapNum, pSlave->Parse.AddrStart + i, pSlave->Parse.RegData, MBX_MAP_FIND_MODE_CONTINUOUS) != MBX_API_RETURN_DEFAULT)
        {
            return MBX_EXCEPTION_FAULT;
        }
    }
    if(MBx_utility_map_w_cooperate_review( ) != MBX_API_RETURN_DEFAULT)
    {
        return MBX_EXCEPTION_FAULT;
    }

    /* 填充回复流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);      // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func));        // 功能码
    MBxTxBufferPutReg(pSlave, (pSlave->Parse.AddrStart)); // 寄存器地址
    MBxTxBufferPutReg(pSlave, (pSlave->Parse.RegNum));    // 寄存器数

    /* 验证长度 */
    if(pSlave->TxExist.Len + 2 > pSlave->TxExist.LenMAX)
    {
        return MBX_EXCEPTION_FAULT;
    }

    return MBX_EXCEPTION_NONE;
}

/**
 * @brief modbus 从机消息系统的底层消息处理 产生错误代码处理
 * @param pSlave MBX从机对象指针
 * @param ErrorCode 解析过程产生的错误代码 见MBx_api.h "错误码定义" 部分
 */
void MBx_Slave_Error_RTU_Handle(_MBX_SLAVE *pSlave, uint8_t ErrorCode)
{
    MBxTxBufferEmpty(pSlave); // 舍弃原有发送消息
    /* 产生错误消息流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);                           // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func + MBX_FUNC_EXCEPTION_OFFSET)); // 功能码+错误附加标识
    MBxTxBufferPutc(pSlave, ErrorCode);                                        // 错误码
}

/**
 * @brief modbus 从机消息系统的底层消息处理 产生错误代码处理
 * @param pSlave MBX从机对象指针
 * @param ErrorCode 解析过程产生的错误代码 见MBx_api.h "错误码定义" 部分
 */
void MBx_Slave_Error_TCP_Handle(_MBX_SLAVE *pSlave, uint8_t ErrorCode)
{
    pSlave->TxExist.Len = 4; // 保留事务号和协议标识，舍弃其他消息
    /* 重新填充帧长度 */
    MBxTxBufferPutReg(pSlave, 3);
    /* 产生错误消息流 */
    MBxTxBufferPutc(pSlave, pSlave->Config.SlaveID);                           // 从机ID
    MBxTxBufferPutc(pSlave, (pSlave->Parse.Func + MBX_FUNC_EXCEPTION_OFFSET)); // 功能码+错误附加标识
    MBxTxBufferPutc(pSlave, ErrorCode);                                        // 错误码
}

#endif /* MBX_SLAVE_ENABLE */