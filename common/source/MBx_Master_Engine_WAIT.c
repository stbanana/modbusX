/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Engine_WAIT.c
 * Author        : yono
 * Date          : 2024-07-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus单机驱动的运行, 空闲态处理分支, 内部函数, 不应由用户调用
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
 * @brief 驱动modbusX主机系统 等待态处理分支
 * @param pMaster MBX主机对象指针
 */
void MBx_Master_Engine_WAIT(_MBX_MASTER *pMaster)
{
    uint8_t getc;
    /* 等待超时审查 */
    if(pMaster->Runtime.TimeCnt > MBX_MASTER_RESPONSE_TIMEOUT_US)
    {
        MBxMasterErrortAdd(pMaster, MASTER_PARSE_SENDFUNC(pMaster), MBX_EXCEPTION_TIMEOUT, MASTER_PARSE_SENDADDRSTART(pMaster), MASTER_PARSE_SENDREGNUM(pMaster));
        pMaster->Runtime.State = MBX_STATE_IDLE; // 等待接收已超时, 流转空闲态
    }

    /* 等待条件 */
    while(pMaster->Func.Getc(&getc) == MBX_PORT_RETURN_DEFAULT)
    {
        MBxRxBufferPutc(pMaster, getc);
    }
    if(pMaster->RxExist.Len > 0U)
    {
        pMaster->Runtime.State     = MBX_STATE_READ; // 接收数据, 流转接收态
        pMaster->Runtime.StateFlow = 1;
    }
}

#undef MASTER_PARSE_SLAVEID
#undef MASTER_PARSE_SENDFUNC
#undef MASTER_PARSE_SENDADDRSTART
#undef MASTER_PARSE_SENDREGNUM
#undef MASTER_PARSE_SENDVALUE

#endif /* MBX_MASTER_ENABLE */
