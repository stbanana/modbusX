/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Scan.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    整个modbus从机驱动的轮询动作, 不应由用户调用, 由外部接口API周期性调用此函数
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern _MBX_SLAVE *MbxSChainRoot;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Slave_Engine(_MBX_SLAVE *pSlave, uint32_t Cycle);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX从机系统
 * @param Cycle 调用本函数的周期值us
 */
void MBx_Slave_Scan(uint32_t Cycle)
{
    _MBX_SLAVE *ChainNow = MbxSChainRoot;

    /* 依次驱动链上所有从机对象 */
    while(ChainNow != NULL)
    {
        // 单从机对象驱动
        MBx_Slave_Engine(ChainNow, Cycle);
        ChainNow = ChainNow->Next;
    }
}
#endif /* MBX_SLAVE_ENABLE */
