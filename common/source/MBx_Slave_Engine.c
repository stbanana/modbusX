/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Engine.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单从机驱动的运行, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Slave_Engine_IDLE(_MBX_SLAVE *pSlave);
extern void MBx_Slave_Engine_WRITE(_MBX_SLAVE *pSlave);
extern void MBx_Slave_Engine_READ(_MBX_SLAVE *pSlave);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX从机系统
 * @param pSlave MBX从机对象指针
 * @param Cycle 调用本函数的周期值us
 */
void MBx_Slave_Engine(_MBX_SLAVE *pSlave, uint32_t Cycle)
{
    pSlave->Runtime.TimeCnt += Cycle;
    pSlave->Runtime.StateFlow = 1;

    while(pSlave->Runtime.StateFlow == 1)
    {
        pSlave->Runtime.StateFlow = 0; // 清除流转动作标识
        if(pSlave->Runtime.StatePast != pSlave->Runtime.State)
        {
            pSlave->Runtime.StatePast = pSlave->Runtime.State;
            pSlave->Runtime.TimeCnt   = 0; // 状态切换时重置时间计数器
        }

        switch(pSlave->Runtime.State)
        {
        case MBX_STATE_IDLE:
            MBx_Slave_Engine_IDLE(pSlave);
            break;
        // case MBX_STATE_ERROR:
        //     break;
        case MBX_STATE_WRITE:
            MBx_Slave_Engine_WRITE(pSlave);
            break;
        case MBX_STATE_READ:
            MBx_Slave_Engine_READ(pSlave);
            break;
        default:
            pSlave->Runtime.State = MBX_STATE_IDLE; // 意外状态, 自动流转至空闲
            break;
        }
    }
}
#endif /* MBX_SLAVE_ENGINE_C_ */