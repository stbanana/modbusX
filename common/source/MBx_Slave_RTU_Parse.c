/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_RTU_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机消息解析系统的RTU分支, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern uint32_t    MBx_Slave_READ_COIL_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_READ_DISC_INPUTL_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_READ_REG_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_READ_INPUT_REG_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_WRITE_COIL_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_WRITE_REG_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_WRITE_COIL_MUL_Handle(_MBX_SLAVE *pSlave);
extern uint32_t    MBx_Slave_WRITE_REG_MUL_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_Error_RTU_Handle(_MBX_SLAVE *pSlave, uint8_t ErrorCode);

static inline void MBx_Slave_Parse_RTU_Func_Get(_MBX_SLAVE *pSlave);
static inline void MBx_Slave_Parse_RTU_AddrStar_Get(_MBX_SLAVE *pSlave);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbusX从机消息解析系统 RTU分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_Parse(_MBX_SLAVE *pSlave)
{
    uint32_t FrameLen  = 0;                  // 当前帧的完整帧长度。
    uint32_t ErrorCode = MBX_EXCEPTION_NONE; // 错误码

    /* 审查是否符合最小帧长 */
    if(pSlave->RxExist.Len < 8)
    {
        MBxRxBufferEmpty(pSlave);
        return; // 帧不完整, 弃帧
    }

    /* 提取功能码 */
    MBx_Slave_Parse_RTU_Func_Get(pSlave);
    /* 提取寄存器起始地址 */
    MBx_Slave_Parse_RTU_AddrStar_Get(pSlave);
    /* 依据功能码不同. 检测帧完整性 */
    switch(pSlave->Parse.Func)
    {
    case MBX_FUNC_READ_COIL:
    case MBX_FUNC_READ_DISC_INPUT:
    case MBX_FUNC_READ_REG:
    case MBX_FUNC_READ_INPUT_REG:
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
        FrameLen = 8;
        break;
    case MBX_FUNC_WRITE_COIL_MUL:
        FrameLen = pSlave->RxExist.Buffer[6]; // 获得设置值字节数
        FrameLen += 9;                        // 增加定长数据长度
        if(pSlave->RxExist.Len < FrameLen)
        {
            MBxRxBufferEmpty(pSlave);
            return; // 帧不完整, 弃帧
        }
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        FrameLen = pSlave->RxExist.Buffer[6]; // 获得设置值字节数
        FrameLen += 9;                        // 增加定长数据长度
        if(pSlave->RxExist.Len < FrameLen)
        {
            MBxRxBufferEmpty(pSlave);
            return; // 帧不完整, 弃帧
        }
        break;
    default:
        MBxRxBufferEmpty(pSlave);
        return; // 未知指令，通常是错误帧
    }

    /* 审查从机ID号是否符合本机 */
    if(pSlave->Config.SlaveID != pSlave->RxExist.Buffer[0] && // 不等于本机
       0 != pSlave->RxExist.Buffer[0])                        // 非广播
    {
        MBxRxBufferRemove(pSlave, FrameLen);
        return;
    }

    /* 审查是否符合帧长 */
    if(pSlave->RxExist.Len < FrameLen)
    {
        MBxRxBufferEmpty(pSlave);
        return; // 帧不完整, 弃帧
    }

    /* 检测CRC (一个丑陋的写法，但能稍微节省性能)*/
    if((((uint16_t)(pSlave->RxExist.Buffer[FrameLen - 1]) << 8) | (pSlave->RxExist.Buffer[FrameLen - 2])) != // 提取CRC值
       MBx_utility_crc16(pSlave->RxExist.Buffer, FrameLen - 2))                                              // 计算CRC值
    {
        ErrorCode = MBX_EXCEPTION_CRC;
    }

    /* 进入不同的分支处理 */
    if(ErrorCode == MBX_EXCEPTION_NONE)
    {
        switch(pSlave->Parse.Func)
        {
        case MBX_FUNC_READ_COIL:
            ErrorCode = MBx_Slave_READ_COIL_Handle(pSlave);
            break;
        case MBX_FUNC_READ_DISC_INPUT:
            ErrorCode = MBx_Slave_READ_DISC_INPUTL_Handle(pSlave);
            break;
        case MBX_FUNC_READ_REG:
            ErrorCode = MBx_Slave_READ_REG_Handle(pSlave);
            break;
        case MBX_FUNC_READ_INPUT_REG:
            ErrorCode = MBx_Slave_READ_INPUT_REG_Handle(pSlave);
            break;
        case MBX_FUNC_WRITE_COIL:
            ErrorCode = MBx_Slave_WRITE_COIL_Handle(pSlave);
            break;
        case MBX_FUNC_WRITE_REG:
            ErrorCode = MBx_Slave_WRITE_REG_Handle(pSlave);
            break;
        case MBX_FUNC_WRITE_COIL_MUL:
            ErrorCode = MBx_Slave_WRITE_COIL_MUL_Handle(pSlave);
            break;
        case MBX_FUNC_WRITE_REG_MUL:
            ErrorCode = MBx_Slave_WRITE_REG_MUL_Handle(pSlave);
            break;
        default:
            ErrorCode = MBX_EXCEPTION_UNFUNC; // 产生未知指令错误
            break;
        }
    }

    if(ErrorCode != MBX_EXCEPTION_NONE)
    {
        MBx_Slave_Error_RTU_Handle(pSlave, ErrorCode); // 剔除旧消息，产生错误回复消息
    }

    _MBX_CRC16 crc;
    crc.Val = MBx_utility_crc16((uint8_t *)(pSlave->TxExist.Buffer), pSlave->TxExist.Len); // 计算CRC校验码
    MBxTxBufferPutc(pSlave, crc.H_L.L8);                                                   // CRC低8位
    MBxTxBufferPutc(pSlave, crc.H_L.H8);                                                   // CRC高8位

    /* 删除已处理消息 */
    MBxRxBufferRemove(pSlave, FrameLen);

    /* 广播时不回复，TODO:此为临时解决方案 */
    if(pSlave->Config.SlaveID == 0)
    {
        pSlave->TxExist.Len = 0;
    }
}

/**
 * @brief 提取 RTU消息功能码
 * @param pSlave MBX从机对象指针
 */
static inline void MBx_Slave_Parse_RTU_Func_Get(_MBX_SLAVE *pSlave)
{
    pSlave->Parse.Func = pSlave->RxExist.Buffer[1];
}

/**
 * @brief 提取 RTU寄存器起始地址
 * @param pSlave MBX从机对象指针
 */
static inline void MBx_Slave_Parse_RTU_AddrStar_Get(_MBX_SLAVE *pSlave)
{
    pSlave->Parse.AddrStart = (((uint16_t)pSlave->RxExist.Buffer[2] << 8) //地址高八位
                               | pSlave->RxExist.Buffer[3]);              // 地址低八位
}

#endif /* MBX_SLAVE_ENABLE */