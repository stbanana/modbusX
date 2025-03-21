/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Engine_READ.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单主机驱动的运行, 接收态处理分支, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Master_Parse(_MBX_MASTER *pMaster);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX主机系统 接收态处理分支
 * @param pMaster MBX主机对象指针
 */
void MBx_Master_Engine_READ(_MBX_MASTER *pMaster)
{
    uint8_t getc;
    while(pMaster->Func.Getc(&getc) == MBX_PORT_RETURN_DEFAULT)
    {
        MBxRxBufferPutc(pMaster, getc);
        pMaster->Runtime.TimeCnt = 0; // 接收到数据, 计时清零
    }
    if(pMaster->Runtime.TimeCnt > pMaster->Attr.T1_5_Cycs)
    {
        MBx_Master_Parse(pMaster);                   // 流转此态条件是收到数据, 无需判接收长度, 解析接收即可
        pMaster->Runtime.State     = MBX_STATE_IDLE; // 流转回等待态
        pMaster->Runtime.StateFlow = 1;
    }
}
#endif /* MBX_MASTER_ENABLE */