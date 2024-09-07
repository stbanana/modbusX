/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_utility_master_request.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbusX错误栈的操作函数(主机专有错误栈)
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief MBX主站错误队列中添加一个错误
 * @param pMaster  MBX对象指针
 * @param Func 错误发送功能码
 * @param Error 实际收到的错误码
 * @param AddrStart 错误发送地址起始
 * @param RegNum 错误发送寄存器数量
 * @return uint32_t 标准返回
 */
uint32_t MBxMasterErrortAdd(_MBX_MASTER *pMaster, //
                            uint8_t      Func,
                            uint8_t      Error,
                            uint16_t     AddrStart,
                            uint16_t     RegNum)
{
    /* 审查队列是否已满 */
    if(MBxMasterErrorFullQ(pMaster))
        return MBX_API_RETURN_BUFFER_FULL;

    /* 填充一条错误 */
    pMaster->Error.Queue[pMaster->Error.Head].SendFunc      = Func;
    pMaster->Error.Queue[pMaster->Error.Head].ErrorCode     = Error;
    pMaster->Error.Queue[pMaster->Error.Head].SendAddrStart = AddrStart;
    pMaster->Error.Queue[pMaster->Error.Head].SendRegNum    = RegNum;

    /* 推进入栈指针 */
    pMaster->Error.Head = (pMaster->Error.Head + 1) % MBX_MASTER_ERROR_QUEUE_MAX;
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief MBX主站错误队列中获得一个错误
 * @param pMaster  MBX对象指针
 * @param Func 错误发送功能码
 * @param Error 实际收到的错误码
 * @param AddrStart 错误发送地址起始
 * @param RegNum 错误发送寄存器数量
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Error_Get(_MBX_MASTER *pMaster, //
                              uint8_t     *Func,
                              uint8_t     *Error,
                              uint16_t    *AddrStart,
                              uint16_t    *RegNum)
{
    /* 审查队列是否为空 */
    if(MBxMasterErrorEmptyQ(pMaster))
        return MBX_API_RETURN_BUFFER_EMPTY;

    /* 获得一条错误 */
    *Func      = pMaster->Error.Queue[pMaster->Error.Tail].SendFunc;
    *Error     = pMaster->Error.Queue[pMaster->Error.Tail].ErrorCode;
    *AddrStart = pMaster->Error.Queue[pMaster->Error.Tail].SendAddrStart;
    *RegNum    = pMaster->Error.Queue[pMaster->Error.Tail].SendRegNum;

    /* 推进出栈指针 */
    pMaster->Error.Tail = (pMaster->Error.Tail + 1) % MBX_MASTER_ERROR_QUEUE_MAX;
    return MBX_API_RETURN_DEFAULT;
}

#endif /* MBX_MASTER_ENABLE */
