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
    modbus单从机驱动的运行，接收态处理分支，内部函数，不应由用户调用
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
 * @brief 驱动modbusX从机系统 接收态处理分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_Engine_READ(_MBX_SLAVE *pSlave)
{
    uint8_t getc;
    while(pSlave->Func.Getc(&getc) == MBX_PORT_RETURN_DEFAULT)
    {
        MBxSlaveRxBufferPutc(pSlave, getc); // 不判断返回值，若buffer不够大直接丢数据
    }
    if(pSlave->RxExist.Len > 0)
    {
    }
}
#endif /* MBX_SLAVE_ENABLE */