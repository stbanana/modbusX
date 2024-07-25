/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Engine_WRITE.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机驱动的运行，发送态处理分支，内部函数，不应由用户调用
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
 * @brief 驱动modbusX从机系统 发送态处理分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_Engine_WRITE(_MBX_SLAVE *pSlave)
{
#if MBX_SEND_MODE_BYTES
    if(pSlave->Func.Send(pSlave->TxExist.Buffer, pSlave->TxExist.Len) == MBX_PORT_RETURN_DEFAULT)
    {
        pSlave->TxExist.Len   = 0;
        pSlave->Runtime.State = MBX_STATE_IDLE; // 成功发送，流转等待态
    }
    else
    {
        // 发送失败，下次轮询继续发送态
    }
#else
    uint32_t SendState = MBX_PORT_RETURN_DEFAULT;
    uint32_t SendNum;
    uint32_t CopyNum;
    for(SendNum = 0; (SendNum < pSlave->TxExist.Len && SendState == MBX_PORT_RETURN_DEFAULT); SendNum++)
        SendState = pSlave->Func.Send(pSlave->TxExist.Buffer[0]);
    if(SendState == MBX_PORT_RETURN_DEFAULT)
    {
        pSlave->TxExist.Len   = 0;
        pSlave->Runtime.State = MBX_STATE_IDLE; // 成功发送，流转等待态
    }
    else
    { // 发送失败，去除已发送内容，其他内容搬运至buffer前，下次轮询继续发送
        // 单字节发送更适用环形buffer，或者使用窗口推移机制，目前没有优化计划
        pSlave->TxExist.Len -= SendNum;
        for(CopyNum = 0; CopyNum < pSlave->TxExist.Len; CopyNum++)
        {
            pSlave->TxExist.Buffer[CopyNum] = pSlave->TxExist.Buffer[CopyNum + SendNum];
        }
    }
#endif
}
#endif /* MBX_SLAVE_ENABLE */