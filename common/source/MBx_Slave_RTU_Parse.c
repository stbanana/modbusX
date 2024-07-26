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
extern void        MBx_Slave_RTU_READ_COIL_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_READ_INPUTL_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_READ_REG_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_READ_INPUT_REG_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_WRITE_COIL_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_WRITE_REG_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_WRITE_COIL_MUL_Handle(_MBX_SLAVE *pSlave);
extern void        MBx_Slave_RTU_WRITE_REG_MUL_Handle(_MBX_SLAVE *pSlave);

static inline void MBx_Slave_Parse_RTU_Func_Get(_MBX_SLAVE *pSlave);
static inline void MBx_Slave_Parse_RTU_AddrStar_Get(_MBX_SLAVE *pSlave);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbusX从机消息解析系统 RTU分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_Parse(_MBX_SLAVE *pSlave)
{
    /* 审查从机ID号是否符合本机。
    实际上应该检测提取完整帧后再审查, 为了减少运行语句, 在此审查。 */
    if(pSlave->Config.SlaveID != pSlave->RxExist.Buffer[0])
        return;

    /* 审查是否符合最小帧长 */
    if(pSlave->RxExist.Len < 8)
    {
        MBxRxBufferEmpty(pSlave);
        return; // 帧不完整, 弃帧
    }

    uint32_t FrameLen = 8; // 当前帧的完整帧长度。规范应在函数最前定义，为了在审查不通过时减少栈操作，在常规审查后定义

    /* 提取功能码 */
    MBx_Slave_Parse_RTU_Func_Get(pSlave);
    /* 提取寄存器起始地址 */
    MBx_Slave_Parse_RTU_AddrStar_Get(pSlave);
    /* 依据功能码不同. 检测帧完整性. 进入不同的分支处理 */
    switch(pSlave->Parse.Func)
    {
    case MBX_FUNC_READ_COIL:
        MBx_Slave_RTU_READ_COIL_Handle(pSlave);
        break;
    case MBX_FUNC_READ_INPUT:
        MBx_Slave_RTU_READ_INPUTL_Handle(pSlave);
    case MBX_FUNC_READ_REG:
        MBx_Slave_RTU_READ_REG_Handle(pSlave);
        break;
    case MBX_FUNC_READ_INPUT_REG:
        MBx_Slave_RTU_READ_INPUT_REG_Handle(pSlave);
        break;
    case MBX_FUNC_WRITE_COIL:
        MBx_Slave_RTU_WRITE_COIL_Handle(pSlave);
        break;
    case MBX_FUNC_WRITE_REG:
        MBx_Slave_RTU_WRITE_REG_Handle(pSlave);
        break;
    case MBX_FUNC_WRITE_COIL_MUL:
        FrameLen = pSlave->RxExist.Buffer[6]; // 获得设置值字节数
        FrameLen += 9;                        // 增加定长数据长度
        if(pSlave->RxExist.Len < FrameLen)
        {
            MBxRxBufferEmpty(pSlave);
            return; // 帧不完整, 弃帧
        }
        MBx_Slave_RTU_WRITE_COIL_MUL_Handle(pSlave);
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        FrameLen = pSlave->RxExist.Buffer[6]; // 获得设置值字节数
        FrameLen += 9;                        // 增加定长数据长度
        if(pSlave->RxExist.Len < FrameLen)
        {
            MBxRxBufferEmpty(pSlave);
            return; // 帧不完整, 弃帧
        }
        MBx_Slave_RTU_WRITE_REG_MUL_Handle(pSlave);
        break;
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
    pSlave->Parse.AddrStar = (((uint16_t)pSlave->RxExist.Buffer[2] << 8) //地址高八位
                              & pSlave->RxExist.Buffer[3]);              // 地址低八位
}

#endif /* MBX_SLAVE_ENABLE */