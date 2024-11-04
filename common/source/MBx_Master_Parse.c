/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单主机消息解析系统的运行, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Master_RTU_Parse(_MBX_MASTER *pMaster);
extern void MBx_Master_TCP_Parse(_MBX_MASTER *pMaster);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief modbusX从机消息解析系统
 * @param pMaster MBX从机对象指针
 */
void MBx_Master_Parse(_MBX_MASTER *pMaster)
{
    switch(pMaster->Attr.ModbusModel)
    {
    case MBX_MODEL_RTU:
        MBx_Master_RTU_Parse(pMaster);
        break;
    case MBX_MODEL_TCP:
        MBx_Master_TCP_Parse(pMaster);
        break;
    default:
        break;
    }
}

#endif /* MBX_MASTER_ENABLE */