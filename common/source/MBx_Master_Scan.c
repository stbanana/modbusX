/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Scan.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    整个modbus主机驱动的轮询动作, 不应由用户调用, 由外部接口API周期性调用此函数
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern _MBX_MASTER *MbxMChainRoot;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MBx_Master_Engine(_MBX_MASTER *pMaster, uint32_t Cycle);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX主机系统
 * @param Cycle 调用本函数的周期值us
 */
void MBx_Master_Scan(uint32_t Cycle)
{
    _MBX_MASTER *ChainNow = MbxMChainRoot;

    /* 依次驱动链上所有主机对象 */
    while(ChainNow != NULL)
    {
        // 单主机对象驱动
        MBx_Master_Engine(ChainNow, Cycle);
        ChainNow = ChainNow->Next;
    }
}
#endif /* MBX_MASTER_ENABLE */