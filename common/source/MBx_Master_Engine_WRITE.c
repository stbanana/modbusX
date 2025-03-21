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
#if MBX_MODULE_TCP_MASTER_ENABLE
#define MASTER_PARSE_SLAVEID(pMB)       pMB->Parse.SlaveID[pMB->Parse.Tail]
#define MASTER_PARSE_SENDFUNC(pMB)      pMB->Parse.SendFunc[pMB->Parse.Tail]
#define MASTER_PARSE_SENDADDRSTART(pMB) pMB->Parse.SendAddrStart[pMB->Parse.Tail]
#define MASTER_PARSE_SENDREGNUM(pMB)    pMB->Parse.SendRegNum[pMB->Parse.Tail]
#define MASTER_PARSE_SENDVALUE(pMB)     pMB->Parse.SendValue[pMB->Parse.Tail]
#else
#define MASTER_PARSE_SLAVEID(pMB)       pMB->Parse.SlaveID
#define MASTER_PARSE_SENDFUNC(pMB)      pMB->Parse.SendFunc
#define MASTER_PARSE_SENDADDRSTART(pMB) pMB->Parse.SendAddrStart
#define MASTER_PARSE_SENDREGNUM(pMB)    pMB->Parse.SendRegNum
#define MASTER_PARSE_SENDVALUE(pMB)     pMB->Parse.SendValue
#endif
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
        pMaster->TxExist.Len       = 0;
        pMaster->Runtime.State     = MBX_STATE_WAIT; // 成功发送, 流转等待态
        pMaster->Runtime.StateFlow = 1;
    }
    else
    {
        if(pMaster->Runtime.TimeCnt > MBX_SENDERR_TIMEOUT_US)
        {
            pMaster->TxExist.Len = 0;
            MBxMasterErrortAdd(pMaster, MASTER_PARSE_SENDFUNC(pMaster), MBX_EXCEPTION_SENDERR, MASTER_PARSE_SENDADDRSTART(pMaster), MASTER_PARSE_SENDREGNUM(pMaster));
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

#undef MASTER_PARSE_SLAVEID
#undef MASTER_PARSE_SENDFUNC
#undef MASTER_PARSE_SENDADDRSTART
#undef MASTER_PARSE_SENDREGNUM
#undef MASTER_PARSE_SENDVALUE

#endif /* MBX_MASTER_ENABLE */