/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Ticks.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    暂定为整个modbus驱动的轮询动作, 需要周期性调用此函数
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Slave_Scan(uint32_t Cycle);
extern void MBx_Master_Scan(uint32_t Cycle);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 必须周期调用, 驱动modbusX系统的关键函数
 * @param Cycle 调用本函数的周期值us
 */
void MBx_Ticks(uint32_t Cycle)
{
#if MBX_SLAVE_ENABLE
    MBx_Slave_Scan(Cycle);
#endif
#if MBX_MASTER_ENABLE
    MBx_Master_Scan(Cycle);
#endif
}