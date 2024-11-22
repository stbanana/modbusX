/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_TCP_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单主机消息解析系统的TCP分支, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
#if MBX_MODULE_TCP_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern uint32_t    MBx_Master_READ_COIL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_READ_DISC_INPUTL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_READ_REG_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_READ_INPUT_REG_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_WRITE_COIL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_WRITE_REG_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_WRITE_COIL_MUL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_WRITE_REG_MUL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_Error_Handle(_MBX_MASTER *pMaster);

static inline void MBx_Master_Parse_TCP_Func_Get(_MBX_MASTER *pMaster);
static inline void MBx_Master_Parse_TCP_AddrStar_Get(_MBX_MASTER *pMaster);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbusX从机消息解析系统 TCP分支
 * @param pMaster MBX从机对象指针
 */
void MBx_Master_TCP_Parse(_MBX_MASTER *pMaster)
{
    uint32_t FrameLen  = 0;                  // 当前帧的完整帧长度。规范应在函数最前定义。
    uint32_t ErrorCode = MBX_EXCEPTION_NONE; // 错误码

    /* 审查是否符合最小帧长 */
    if(pMaster->RxExist.Len < 9)
    {
        MBxRxBufferEmpty(pMaster);
        return; // 帧不完整, 弃帧
    }

    /* 解析TCP特有报文头，推移帧成为RTU帧 */
    FrameLen = ((uint16_t)pMaster->RxExist.Buffer[4] << 8) + pMaster->RxExist.Buffer[5];
    MBxRxBufferRemove(pMaster, 6);

    /* 提取功能码 */
    MBx_Master_Parse_TCP_Func_Get(pMaster);
    /* 提取寄存器起始地址 */
    MBx_Master_Parse_TCP_AddrStar_Get(pMaster);

    /* 审查是否符合帧长 */
    if(pMaster->RxExist.Len < FrameLen)
    {
        MBxRxBufferEmpty(pMaster);
        return; // 帧不完整, 弃帧
    }

    /* 进入不同的分支处理 */
    if(ErrorCode == MBX_EXCEPTION_NONE)
    {
        switch(pMaster->Parse.Func)
        {
        case MBX_FUNC_READ_COIL:
            ErrorCode = MBx_Master_READ_COIL_Handle(pMaster);
            break;
        case MBX_FUNC_READ_DISC_INPUT:
            ErrorCode = MBx_Master_READ_DISC_INPUTL_Handle(pMaster);
            break;
        case MBX_FUNC_READ_REG:
            ErrorCode = MBx_Master_READ_REG_Handle(pMaster);
            break;
        case MBX_FUNC_READ_INPUT_REG:
            ErrorCode = MBx_Master_READ_INPUT_REG_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_COIL:
            ErrorCode = MBx_Master_WRITE_COIL_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_REG:
            ErrorCode = MBx_Master_WRITE_REG_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_COIL_MUL:
            ErrorCode = MBx_Master_WRITE_COIL_MUL_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_REG_MUL:
            ErrorCode = MBx_Master_WRITE_REG_MUL_Handle(pMaster);
            break;
        default:
            ErrorCode = MBx_Master_Error_Handle(pMaster);
            break;
        }
    }

    if(ErrorCode != MBX_EXCEPTION_NONE)
    {
        MBx_MODULE_TRACE_ADD_ERR(pMaster, ErrorCode);
    }

    /* 清空接收buffer */
    MBxRxBufferEmpty(pMaster);

    pMaster->Parse.Tail = (pMaster->Parse.Tail + 1) % MBX_MASTER_REQUEST_QUEUE_MAX;
}

/**
 * @brief 提取 TCP消息功能码
 * @param pMaster MBX从机对象指针
 */
static inline void MBx_Master_Parse_TCP_Func_Get(_MBX_MASTER *pMaster)
{
    pMaster->Parse.Func = pMaster->RxExist.Buffer[1];
}

/**
 * @brief 提取 TCP寄存器起始地址
 * @param pMaster MBX从机对象指针
 */
static inline void MBx_Master_Parse_TCP_AddrStar_Get(_MBX_MASTER *pMaster)
{
    pMaster->Parse.AddrStart = (((uint16_t)pMaster->RxExist.Buffer[2] << 8) //地址高八位
                                | pMaster->RxExist.Buffer[3]);              // 地址低八位
}

#else
/**
 * @brief modbusX从机消息解析系统 TCP分支 未开启TCP主机功能 空处理
 * @param pMaster MBX从机对象指针
 */
void MBx_Master_TCP_Parse(_MBX_MASTER *pMaster)
{
}

#endif /* MBX_MODULE_TCP_MASTER_ENABLE */

#endif /* MBX_MASTER_ENABLE */