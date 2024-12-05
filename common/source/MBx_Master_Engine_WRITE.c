/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Engine_WRITE.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单主机驱动的运行, 发送态处理分支, 内部函数, 不应由用户调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动modbusX主机系统 发送态处理分支
 * @param pMaster MBX主机对象指针
 */
void MBx_Master_Engine_WRITE(_MBX_MASTER *pMaster)
{
#if MBX_SEND_MODE_BYTES
    if(pMaster->Func.Send(pMaster->TxExist.Buffer, pMaster->TxExist.Len) == MBX_PORT_RETURN_DEFAULT)
    {
        pMaster->TxExist.Len   = 0;
        pMaster->Runtime.State = MBX_STATE_WAIT; // 成功发送, 流转等待态
    }
    else
    {
        if(pMaster->Runtime.TimeCnt > MBX_SENDERR_TIMEOUT_US)
        {
            pMaster->TxExist.Len = 0;
            MBxMasterErrortAdd(pMaster, pMaster->Parse.SendFunc, MBX_EXCEPTION_SENDERR, pMaster->Parse.SendAddrStart, pMaster->Parse.SendRegNum);
            pMaster->Runtime.State = MBX_STATE_IDLE; // 未成功发送且超时, 流转空闲态
        }
        // 发送失败, 下次轮询继续发送态
    }
#else
    uint32_t SendState = MBX_PORT_RETURN_DEFAULT;
    uint32_t SendNum;
    uint32_t CopyNum;
    for(SendNum = 0; (SendNum < pMaster->TxExist.Len && SendState == MBX_PORT_RETURN_DEFAULT); SendNum++)
        SendState = pMaster->Func.Send(pMaster->TxExist.Buffer[0]);
    if(SendState == MBX_PORT_RETURN_DEFAULT)
    {
        pMaster->TxExist.Len   = 0;
        pMaster->Runtime.State = MBX_STATE_WAIT; // 成功发送, 流转等待态
    }
    else
    { // 发送失败, 去除已发送内容, 其他内容搬运至buffer前, 下次轮询继续发送
        // 单字节发送更适用环形buffer, 或者使用窗口推移机制, 目前没有优化计划
        pMaster->TxExist.Len -= SendNum;
        for(CopyNum = 0; CopyNum < pMaster->TxExist.Len; CopyNum++)
        {
            pMaster->TxExist.Buffer[CopyNum] = pMaster->TxExist.Buffer[CopyNum + SendNum];
        }
    }
#endif
}
#endif /* MBX_MASTER_ENABLE */