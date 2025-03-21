/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_User_Request.c
 * Author        : yono
 * Date          : 2024-08-03
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    供用户程序调用发起请求
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
 * @brief modbusX主机发出一条读线圈请求
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param StartAddr 读取起始地址
 * @param ReadNum 读取数量
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Read_Coil_Request(_MBX_MASTER *pMaster, //
                                      uint8_t      SlaveID,
                                      uint16_t     StartAddr,
                                      uint16_t     ReadNum)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_READ_COIL, StartAddr, ReadNum, (uint8_t *)MBX_PARA_NULL, MBX_PARA_NULL);
}

/**
 * @brief modbusX主机发出一条读离散输入请求
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param StartAddr 读取起始地址
 * @param ReadNum 读取数量
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Read_Disc_Input_Request(_MBX_MASTER *pMaster, //
                                            uint8_t      SlaveID,
                                            uint16_t     StartAddr,
                                            uint16_t     ReadNum)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_READ_DISC_INPUT, StartAddr, ReadNum, (uint8_t *)MBX_PARA_NULL, MBX_PARA_NULL);
}

/**
 * @brief modbusX主机发出一条读保持寄存器请求
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param StartAddr 读取起始地址
 * @param ReadNum 读取数量
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Read_Reg_Request(_MBX_MASTER *pMaster, //
                                     uint8_t      SlaveID,
                                     uint16_t     StartAddr,
                                     uint16_t     ReadNum)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_READ_REG, StartAddr, ReadNum, (uint8_t *)MBX_PARA_NULL, MBX_PARA_NULL);
}

/**
 * @brief modbusX主机发出一条读输入寄存器请求
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param StartAddr 读取起始地址
 * @param ReadNum 读取数量
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Read_Input_Reg_Request(_MBX_MASTER *pMaster, //
                                           uint8_t      SlaveID,
                                           uint16_t     StartAddr,
                                           uint16_t     ReadNum)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_READ_INPUT_REG, StartAddr, ReadNum, (uint8_t *)MBX_PARA_NULL, MBX_PARA_NULL);
}

/**
 * @brief modbusX主机发出一条写单个线圈请求
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param Addr 写入地址
 * @param Value 写入值 只有0与>0
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Write_Coil_Request(_MBX_MASTER *pMaster, //
                                       uint8_t      SlaveID,
                                       uint16_t     Addr,
                                       uint16_t     Value)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint8_t                  StandardData[2];

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    if(Value == 0)
    {
        StandardData[0] = 0;
        StandardData[1] = 0;
    }
    else
    {
        StandardData[0] = 0xFF;
        StandardData[1] = 0x00;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_WRITE_COIL, Addr, MBX_PARA_NULL, StandardData, 2);
}

/**
 * @brief modbusX主机发出一条写单个保持寄存器请求
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param Addr 写入地址
 * @param Value 写入值 
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Write_Reg_Request(_MBX_MASTER *pMaster, //
                                      uint8_t      SlaveID,
                                      uint16_t     Addr,
                                      uint16_t     Value)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;
    uint8_t                  StandardData[2];

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    StandardData[0] = Value & 0x00FF;
    StandardData[1] = (Value & 0xFF00) >> 8;

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_WRITE_REG, Addr, MBX_PARA_NULL, StandardData, 2);
}

/**
 * @brief modbusX主机发出一条写多个线圈请求 
 *              Data、DataLen传参需要自行格式化 见其中关于(0x0F)写多个线圈的格式 https://www.yono233.cn/posts/shoot/24_7_26_modbus)
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param StartAddr 写入起始地址
 * @param WriteNum 写入数量
 * @param Data 写入数据起始指针
 * @param DataLen 写入数据长度
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Write_Coil_Mul_Request(_MBX_MASTER *pMaster, //
                                           uint8_t      SlaveID,
                                           uint16_t     StartAddr,
                                           uint16_t     WriteNum,
                                           uint8_t     *Data,
                                           uint16_t     DataLen)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_WRITE_COIL_MUL, StartAddr, WriteNum, Data, DataLen);
}

/**
 * @brief modbusX主机发出一条写多个保持寄存器请求 
 * @param pMaster mbx主机对象
 * @param SlaveID 期望请求的从机号
 * @param StartAddr 写入起始地址
 * @param WriteNum 写入数量
 * @param Data 写入数据
 * @param DataLen 写入数据字节长度
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Write_Reg_Mul_Request(_MBX_MASTER *pMaster, //
                                          uint8_t      SlaveID,
                                          uint16_t     StartAddr,
                                          uint16_t     WriteNum,
                                          uint8_t     *Data,
                                          uint16_t     DataLen)
{
    _MBX_MASTER_TEAM_MEMBER *pMember;

    /* 查找是否存在对应的从机成员 */
    pMember = MBx_Master_Member_Find(pMaster, SlaveID);
    if(pMember == NULL)
    {
        return MBX_API_RETURN_MAP_UNFIND;
    }

    /* 填充一条请求 */
    return MBxMasterRequestAdd(pMaster, SlaveID, MBX_FUNC_WRITE_REG_MUL, StartAddr, WriteNum, Data, DataLen);
}

#endif /* MBX_MASTER_ENABLE */