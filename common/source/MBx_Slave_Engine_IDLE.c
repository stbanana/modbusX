/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Engine_IDLE.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机驱动的运行, 空闲态处理分支, 内部函数, 不应由用户调用
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
 * @brief 驱动modbusX从机系统 空闲态处理分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_Engine_IDLE(_MBX_SLAVE *pSlave)
{
    uint8_t getc;
    while(pSlave->Func.Getc(&getc) == MBX_PORT_RETURN_DEFAULT)
    {
        MBxRxBufferPutc(pSlave, getc);
    }
    if(pSlave->Runtime.TimeCnt >= pSlave->Attr.T3_5_Cycs) // 达成3.5字符间隔, 立即流转
    {
        if(pSlave->TxExist.Len > 0)
        {
            pSlave->Runtime.State     = MBX_STATE_WRITE; // 流转至发送态
            pSlave->Runtime.StateFlow = 1;
        }
        else if(pSlave->RxExist.Len > 0)
        {
            pSlave->Runtime.State     = MBX_STATE_READ; // 流转至接收态
            pSlave->Runtime.StateFlow = 1;
        }
        else
        {
            pSlave->Runtime.TimeCnt = pSlave->Attr.T3_5_Cycs; // 无法流转，静止计时器
        }
    }
}
#endif /* MBX_SLAVE_ENABLE */