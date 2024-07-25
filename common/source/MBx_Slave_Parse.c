/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机消息解析系统的运行，内部函数，不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Slave_Parse_RTU(_MBX_SLAVE *pSlave);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief modbusX从机消息解析系统
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_Parse(_MBX_SLAVE *pSlave)
{
    switch(pSlave->Attr.ModbusModel)
    {
    case MBX_MODEL_RTU:
        MBx_Slave_Parse_RTU(pSlave);
        break;
    case MBX_MODEL_TCP:
        // MBx_Slave_Parse_TCP( );
        break;
    default:
        break;
    }
}

#endif /* MBX_SLAVE_ENABLE */