/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Engine_READ.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机驱动的运行, 接收态处理分支, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Slave_Parse(_MBX_SLAVE *pSlave);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX从机系统 接收态处理分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_Engine_READ(_MBX_SLAVE *pSlave)
{
    uint8_t getc;
    while(pSlave->Func.Getc(&getc) == MBX_PORT_RETURN_DEFAULT)
    {
        MBxRxBufferPutc(pSlave, getc); // 若buffer不够大直接丢数据
        pSlave->Runtime.TimeCnt = 0;   // 接收到数据, 计时清零
    }
    if(pSlave->Runtime.TimeCnt > pSlave->Attr.T1_5_Cycs)
    {
        MBx_Slave_Parse(pSlave);                // 流转此态条件是收到数据, 无需判接收长度, 解析接收即可
        pSlave->Runtime.State = MBX_STATE_IDLE; // 流转回等待态
    }
}
#endif /* MBX_SLAVE_ENABLE */