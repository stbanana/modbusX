/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Init.c
 * Author        : yono
 * Date          : 2024-07-30
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX主机对象
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
_MBX_MASTER *MbxMChainRoot = NULL; // 主机链表头
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 作为RTU 初始化MBX主机对象
 * @param MBxMaster 期望初始化的MBX主机对象指针
 * @param MBxSend 绑定的主机发送port函数
 * @param MBxGetc 绑定的主机接收port函数
 * @param BaudRate 典型波特率
 * @param RxBuffer 绑定的接收buffer区域
 * @param RxBufferSize 绑定的接收buffer最大长度
 * @param TxBuffer 绑定的发送buffer区域
 * @param TxBufferSize 绑定的发送buffer最大长度
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_RTU_Init(_MBX_MASTER *MBxMaster, //
                             MBX_SEND_PTR MBxSend,
                             MBX_GTEC_PTR MBxGetc,
                             uint32_t     BaudRate,
                             uint8_t     *RxBuffer,
                             uint32_t     RxBufferSize,
                             uint8_t     *TxBuffer,
                             uint32_t     TxBufferSize)
{
    uint32_t State = MBX_API_RETURN_DEFAULT;
    /* 传参审查 */
    if((MBxSend == NULL) || (MBxGetc == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBuffer == NULL) || (RxBuffer == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBufferSize < 30U) || (TxBufferSize < 30U)) // buffer收发空间至少为30字节(随便写的值)
        return MBX_API_RETURN_ERR_PARAM;

    /* 加入主机链表 */
    NewMasterChainNode(MbxMChainRoot, MBxMaster);

    /*  传参无关部分    */
    /* 初始化运行时 */
    MBx_Init_Runtime(&MBxMaster->Runtime);
    /* 初始化主机解析栈 */
    MBx_Init_Master_Parse(&MBxMaster->Parse);
    /* 初始化主机错误栈 */
    MBx_Init_Master_Error(&MBxMaster->Error);
    /* 初始化主机请求栈 */
    MBx_Init_Master_Request(&MBxMaster->Request);
    /* 将当前操作的从机号清除 */
    MBxMaster->Config.SlaveID = 0;

    /*  传参有关部分    */
    /* 绑定回调函数部分 */
    MBxMaster->Func.Send = MBxSend;
    MBxMaster->Func.Getc = MBxGetc;
    /* 绑定buffer内存部分 */
    MBxMaster->RxExist.Buffer = RxBuffer;
    MBxMaster->RxExist.LenMAX = RxBufferSize;
    MBxMaster->TxExist.Buffer = TxBuffer;
    MBxMaster->TxExist.LenMAX = TxBufferSize;
    /* 初始化属性 */
    MBx_Init_Attr(&MBxMaster->Attr, MBX_MODEL_RTU, MBX_MODE_MASTER, BaudRate, MBX_PARA_NULL);
    if(State != MBX_API_RETURN_DEFAULT)
        MBx_MODULE_TRACE_ADD_ERR(MBxMaster, State);

    return State;
}

/**
 * @brief 作为TCP 初始化MBX主机对象
 * @param MBxMaster 期望初始化的MBX主机对象指针
 * @param MBxSend 绑定的主机发送port函数
 * @param MBxGetc 绑定的主机接收port函数
 * @param RxBuffer 绑定的接收buffer区域
 * @param RxBufferSize 绑定的接收buffer最大长度
 * @param TxBuffer 绑定的发送buffer区域
 * @param TxBufferSize 绑定的发送buffer最大长度
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_TCP_Init(_MBX_MASTER *MBxMaster, //
                             MBX_SEND_PTR MBxSend,
                             MBX_GTEC_PTR MBxGetc,
                             uint8_t     *RxBuffer,
                             uint32_t     RxBufferSize,
                             uint8_t     *TxBuffer,
                             uint32_t     TxBufferSize)
{
    uint32_t State = MBX_API_RETURN_DEFAULT;
    /* 传参审查 */
    if((MBxSend == NULL) || (MBxGetc == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBuffer == NULL) || (RxBuffer == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBufferSize < 30U) || (TxBufferSize < 30U)) // buffer收发空间至少为30字节(随便写的值)
        return MBX_API_RETURN_ERR_PARAM;

    /* 加入主机链表 */
    NewMasterChainNode(MbxMChainRoot, MBxMaster);

    /*  传参无关部分    */
    /* 初始化运行时 */
    MBx_Init_Runtime(&MBxMaster->Runtime);
    /* 初始化主机解析栈 */
    MBx_Init_Master_Parse(&MBxMaster->Parse);
    /* 初始化主机错误栈 */
    MBx_Init_Master_Error(&MBxMaster->Error);
    /* 初始化主机请求栈 */
    MBx_Init_Master_Request(&MBxMaster->Request);
    /* 将当前操作的从机号清除 */
    MBxMaster->Config.SlaveID = 0;
    /* 将当前累计的事务号清除 */

    /*  传参有关部分    */
    /* 绑定回调函数部分 */
    MBxMaster->Func.Send = MBxSend;
    MBxMaster->Func.Getc = MBxGetc;
    /* 绑定buffer内存部分 */
    MBxMaster->RxExist.Buffer = RxBuffer;
    MBxMaster->RxExist.LenMAX = RxBufferSize;
    MBxMaster->TxExist.Buffer = TxBuffer;
    MBxMaster->TxExist.LenMAX = TxBufferSize;
    /* 初始化属性 */
    MBx_Init_Attr(&MBxMaster->Attr, MBX_MODEL_TCP, MBX_MODE_MASTER, MBX_PARA_NULL, MBX_PARA_NULL);
    if(State != MBX_API_RETURN_DEFAULT)
        MBx_MODULE_TRACE_ADD_ERR(MBxMaster, State);

    return State;
}

#endif /* MBX_MASTER_ENABLE */
