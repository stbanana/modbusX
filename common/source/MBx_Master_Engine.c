/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Engine.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单主机驱动的运行, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Master_Engine_IDLE(_MBX_MASTER *pMaster);
extern void MBx_Master_Engine_WAIT(_MBX_MASTER *pMaster);
extern void MBx_Master_Engine_WRITE(_MBX_MASTER *pMaster);
extern void MBx_Master_Engine_READ(_MBX_MASTER *pMaster);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX主机系统
 * @param pMaster MBX主机对象指针
 * @param Cycle 调用本函数的周期值us
 */
void MBx_Master_Engine(_MBX_MASTER *pMaster, uint32_t Cycle)
{
    pMaster->Runtime.TimeCnt += Cycle;
    pMaster->Runtime.StateFlow = 1;

    while(pMaster->Runtime.StateFlow == 1U)
    {
        pMaster->Runtime.StateFlow = 0; // 清除流转动作标识
        if(pMaster->Runtime.StatePast != pMaster->Runtime.State)
        {
            pMaster->Runtime.StatePast = pMaster->Runtime.State;
            pMaster->Runtime.TimeCnt   = 0; // 状态切换时重置时间计数器
        }

        switch(pMaster->Runtime.State)
        {
        case MBX_STATE_IDLE:
            MBx_Master_Engine_IDLE(pMaster);
            break;
        case MBX_STATE_ERROR:
            break;
        case MBX_STATE_WAIT:
            MBx_Master_Engine_WAIT(pMaster);
            break;
        case MBX_STATE_WRITE:
            MBx_Master_Engine_WRITE(pMaster);
            break;
        case MBX_STATE_READ:
            MBx_Master_Engine_READ(pMaster);
            break;
        default:
            pMaster->Runtime.State = MBX_STATE_IDLE; // 意外状态, 自动流转至空闲
            break;
        }
    }
}
#endif /* MBX_SLAVE_ENGINE_C_ */