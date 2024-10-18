/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_RTU_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单主机消息解析系统的RTU分支, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern uint32_t    MBx_Master_RTU_READ_COIL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_READ_DISC_INPUTL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_READ_REG_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_READ_INPUT_REG_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_WRITE_COIL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_WRITE_REG_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_WRITE_COIL_MUL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_WRITE_REG_MUL_Handle(_MBX_MASTER *pMaster);
extern uint32_t    MBx_Master_RTU_Error_Handle(_MBX_MASTER *pMaster);

static inline void MBx_Master_Parse_RTU_Func_Get(_MBX_MASTER *pMaster);
static inline void MBx_Master_Parse_RTU_AddrStar_Get(_MBX_MASTER *pMaster);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbusX从机消息解析系统 RTU分支
 * @param pMaster MBX从机对象指针
 */
void MBx_Master_RTU_Parse(_MBX_MASTER *pMaster)
{
    /* 审查从机ID号是否符合请求 */
    if(pMaster->Parse.SlaveID != pMaster->RxExist.Buffer[0])
    {
        MBxRxBufferEmpty(pMaster);
        return;
    }

    uint32_t FrameLen  = 0;                  // 当前帧的完整帧长度。规范应在函数最前定义，为了在审查不通过时减少栈操作，在常规审查后定义
    uint32_t ErrorCode = MBX_EXCEPTION_NONE; // 错误码

    /* 提取功能码 */
    MBx_Master_Parse_RTU_Func_Get(pMaster);
    /* 提取寄存器起始地址 */
    MBx_Master_Parse_RTU_AddrStar_Get(pMaster);
    /* 依据功能码不同. 检测帧完整性 */
    switch(pMaster->Parse.Func)
    {
    case MBX_FUNC_READ_COIL:
    case MBX_FUNC_READ_DISC_INPUT:
    case MBX_FUNC_READ_REG:
    case MBX_FUNC_READ_INPUT_REG:
        FrameLen = 5 + pMaster->RxExist.Buffer[2];
        break;
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
    case MBX_FUNC_WRITE_COIL_MUL:
    case MBX_FUNC_WRITE_REG_MUL:
        FrameLen = 8;
        break;
    default: // 可能是异常帧回复
        FrameLen = 5;
        break;
    }

    /* 审查是否符合帧长 */
    if(pMaster->RxExist.Len < FrameLen)
    {
        MBxRxBufferEmpty(pMaster);
        return; // 帧不完整, 弃帧
    }

    /* 检测CRC (一个丑陋的写法，但能稍微节省性能)*/
    if((((uint16_t)(pMaster->RxExist.Buffer[FrameLen - 1]) << 8) | (pMaster->RxExist.Buffer[FrameLen - 2])) != // 提取CRC值
       MBx_utility_crc16(pMaster->RxExist.Buffer, FrameLen - 2))                                               // 计算CRC值
    {
        ErrorCode = MBX_EXCEPTION_CRC;
    }

    /* 进入不同的分支处理 */
    if(ErrorCode == MBX_EXCEPTION_NONE)
    {
        switch(pMaster->Parse.Func)
        {
        case MBX_FUNC_READ_COIL:
            ErrorCode = MBx_Master_RTU_READ_COIL_Handle(pMaster);
            break;
        case MBX_FUNC_READ_DISC_INPUT:
            ErrorCode = MBx_Master_RTU_READ_DISC_INPUTL_Handle(pMaster);
            break;
        case MBX_FUNC_READ_REG:
            ErrorCode = MBx_Master_RTU_READ_REG_Handle(pMaster);
            break;
        case MBX_FUNC_READ_INPUT_REG:
            ErrorCode = MBx_Master_RTU_READ_INPUT_REG_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_COIL:
            ErrorCode = MBx_Master_RTU_WRITE_COIL_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_REG:
            ErrorCode = MBx_Master_RTU_WRITE_REG_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_COIL_MUL:
            ErrorCode = MBx_Master_RTU_WRITE_COIL_MUL_Handle(pMaster);
            break;
        case MBX_FUNC_WRITE_REG_MUL:
            ErrorCode = MBx_Master_RTU_WRITE_REG_MUL_Handle(pMaster);
            break;
        default:
            ErrorCode = MBx_Master_RTU_Error_Handle(pMaster);
            break;
        }
    }

    if(ErrorCode != MBX_EXCEPTION_NONE)
    {
        MBx_MODULE_TRACE_ADD_ERR(pMaster, ErrorCode);
    }

    /* 清空接收buffer */
    MBxRxBufferEmpty(pMaster);
}

/**
 * @brief 提取 RTU消息功能码
 * @param pMaster MBX从机对象指针
 */
static inline void MBx_Master_Parse_RTU_Func_Get(_MBX_MASTER *pMaster)
{
    pMaster->Parse.Func = pMaster->RxExist.Buffer[1];
}

/**
 * @brief 提取 RTU寄存器起始地址
 * @param pMaster MBX从机对象指针
 */
static inline void MBx_Master_Parse_RTU_AddrStar_Get(_MBX_MASTER *pMaster)
{
    pMaster->Parse.AddrStart = (((uint16_t)pMaster->RxExist.Buffer[2] << 8) //地址高八位
                                | pMaster->RxExist.Buffer[3]);              // 地址低八位
}

#endif /* MBX_MASTER_ENABLE */