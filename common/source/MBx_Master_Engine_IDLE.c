/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Engine_IDLE.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单机驱动的运行, 空闲态处理分支, 内部函数, 不应由用户调用
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
 * @brief 驱动modbusX主机系统 空闲态处理分支
 * @param pMaster MBX主机对象指针
 */
void MBx_Master_Engine_IDLE(_MBX_MASTER *pMaster)
{
    uint8_t getchar;
    while(pMaster->Func.Getc(&getchar) == MBX_PORT_RETURN_DEFAULT)
    {
        MBxRxBufferPutc(pMaster, getchar);
    }
    if(pMaster->Runtime.TimeCnt >= pMaster->Attr.T3_5_Cycs) // 达成3.5字符间隔, 立即流转
    {
        if(!MBxMasterRequestEmptyQ(pMaster))
        {
            MBxMasterRequestToTx(pMaster);                // 填充一条请求给发送buffer
            pMaster->Runtime.State     = MBX_STATE_WRITE; // 流转至发送态
            pMaster->Runtime.StateFlow = 1;
        }
        else if(pMaster->RxExist.Len > 0U)
        {
            MBxRxBufferEmpty(pMaster); // 空闲态接收数据非法，清空接收buffer
        }
        else
        {
            pMaster->Runtime.TimeCnt = pMaster->Attr.T3_5_Cycs; // 无法流转，静止计时器
        }
    }
}

#endif /* MBX_MASTER_ENABLE */
