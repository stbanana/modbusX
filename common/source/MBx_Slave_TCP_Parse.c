/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_TCP_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机消息解析系统的TCP分支, 内部函数, 不应由用户调用
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
extern void        MBx_Slave_Error_TCP_Handle(_MBX_SLAVE *pSlave, uint8_t ErrorCode);

static inline void MBx_Slave_Parse_TCP_Func_Get(_MBX_SLAVE *pSlave);
static inline void MBx_Slave_Parse_TCP_AddrStar_Get(_MBX_SLAVE *pSlave);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbusX从机消息解析系统 TCP分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_TCP_Parse(_MBX_SLAVE *pSlave)
{
    uint32_t FrameLen  = 0;                  // 当前帧的完整帧长度。
    uint32_t ErrorCode = MBX_EXCEPTION_NONE; // 错误码
    uint16_t ReplyLen  = 0;                  // 回复帧长度

    /* 审查是否符合最小帧长 */
    if(pSlave->RxExist.Len < 12U)
    {
        MBxRxBufferEmpty(pSlave);
        return; // 帧不完整, 弃帧
    }

    /* 解析TCP特有报文头，推移帧成为RTU帧 */
    MBxTxBufferPutc(pSlave, pSlave->RxExist.Buffer[0]);
    MBxTxBufferPutc(pSlave, pSlave->RxExist.Buffer[1]);
    MBxTxBufferPutc(pSlave, pSlave->RxExist.Buffer[2]);
    MBxTxBufferPutc(pSlave, pSlave->RxExist.Buffer[3]);
    FrameLen = ((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5];
    MBxRxBufferRemove(pSlave, 6);

    /* 提取功能码 */
    MBx_Slave_Parse_TCP_Func_Get(pSlave);
    /* 提取寄存器起始地址 */
    MBx_Slave_Parse_TCP_AddrStar_Get(pSlave);
    /* 检测帧完整性 */
    if(pSlave->RxExist.Len < FrameLen)
    {
        MBxRxBufferEmpty(pSlave);
        return; // 帧不完整, 弃帧
    }

    /* 审查从机ID号是否符合本机 */
    if(pSlave->Config.SlaveID != pSlave->RxExist.Buffer[0] && // 不等于本机
       0 != pSlave->RxExist.Buffer[0])                        // 非广播
    {
        MBxRxBufferRemove(pSlave, FrameLen);
        MBxTxBufferEmpty(pSlave);
        return;
    }

    /* 依据功能码不同.预填充回复帧长度 */
    switch(pSlave->Parse.Func)
    {
    case MBX_FUNC_READ_COIL:
    case MBX_FUNC_READ_DISC_INPUT:
        ReplyLen = (((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5]) >> 3;
        ReplyLen += (((((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5]) % 8) != 0) ? 4 : 3;
        MBxTxBufferPutReg(pSlave, ReplyLen);
        break;
    case MBX_FUNC_READ_REG:
    case MBX_FUNC_READ_INPUT_REG:
        ReplyLen = (((uint16_t)pSlave->RxExist.Buffer[4] << 8) + pSlave->RxExist.Buffer[5]) >> 2;
        ReplyLen += 3;
        MBxTxBufferPutReg(pSlave, ReplyLen);
        break;
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
    case MBX_FUNC_WRITE_COIL_MUL:
    case MBX_FUNC_WRITE_REG_MUL:
        MBxTxBufferPutReg(pSlave, 6);
        break;
    default:
        break;
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
        MBx_Slave_Error_TCP_Handle(pSlave, ErrorCode); // 剔除旧消息，产生错误回复消息
    }

    /* 删除已处理消息 */
    MBxRxBufferRemove(pSlave, FrameLen);
}

/**
 * @brief 提取 TCP消息功能码
 * @param pSlave MBX从机对象指针
 */
static inline void MBx_Slave_Parse_TCP_Func_Get(_MBX_SLAVE *pSlave)
{
    pSlave->Parse.Func = pSlave->RxExist.Buffer[1];
}

/**
 * @brief 提取 TCP寄存器起始地址
 * @param pSlave MBX从机对象指针
 */
static inline void MBx_Slave_Parse_TCP_AddrStar_Get(_MBX_SLAVE *pSlave)
{
    pSlave->Parse.AddrStart = (((uint16_t)pSlave->RxExist.Buffer[2] << 8) //地址高八位
                               | pSlave->RxExist.Buffer[3]);              // 地址低八位
}

#endif /* MBX_SLAVE_ENABLE */
