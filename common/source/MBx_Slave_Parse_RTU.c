/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Parse_RTU.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机消息解析系统的RTU分支，内部函数，不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define MBX_SLAVE_PARSE_RTU_STEP_FUNC_GET 0
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbusX从机消息解析系统 RTU分支
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_Parse_RTU(_MBX_SLAVE *pSlave)
{
    switch(pSlave->Parse.Step)
    {
    case 0:
        /* code */
        break;

    default:
        break;
    }
}

static inline void MBx_Slave_Parse_RTU_(_MBX_SLAVE *pSlave)
{
}

#endif /* MBX_SLAVE_ENABLE */