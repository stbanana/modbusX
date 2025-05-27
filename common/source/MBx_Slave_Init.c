/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Init.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX从机对象
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
_MBX_SLAVE *MbxSChainRoot = NULL; // 从机链表头
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 作为RTU 初始化MBX从机对象
 * @param MBxSlave 期望初始化的MBX从机对象指针
 * @param SlaveID 绑定的从机ID 1~247
 * @param MAP 指向地址映射表头的指针
 * @param MBxSend 绑定的从机发送port函数
 * @param MBxGetc 绑定的从机接收port函数
 * @param BaudRate 典型波特率
 * @param RxBuffer 绑定的接收buffer区域
 * @param RxBufferSize 绑定的接收buffer最大长度
 * @param TxBuffer 绑定的发送buffer区域
 * @param TxBufferSize 绑定的发送buffer最大长度
 * @return uint32_t 标准返回
 */
uint32_t MBx_Slave_RTU_Init(_MBX_SLAVE                *MBxSlave, //
                            uint8_t                    SlaveID,
                            const _MBX_MAP_LIST_ENTRY *MAP,
                            MBX_SEND_PTR               MBxSend,
                            MBX_GTEC_PTR               MBxGetc,
                            uint32_t                   BaudRate,
                            uint8_t                   *RxBuffer,
                            uint32_t                   RxBufferSize,
                            uint8_t                   *TxBuffer,
                            uint32_t                   TxBufferSize)
{
    uint32_t State = MBX_API_RETURN_DEFAULT;
    /* 传参审查 */
    if((SlaveID < 1) || (SlaveID > 247))
        return MBX_API_RETURN_ERR_PARAM;
    if((MBxSend == NULL) || (MBxGetc == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBuffer == NULL) || (RxBuffer == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBufferSize < 30) || (TxBufferSize < 30)) // buffer收发空间至少为30字节(随便写的值)
        return MBX_API_RETURN_ERR_PARAM;

    /* 加入从机链表 */
    NewSlaveChainNode(MbxSChainRoot, MBxSlave);

    /*  传参无关部分    */
    /* 初始化运行时 */
    MBx_Init_Runtime(&MBxSlave->Runtime);
    /* 初始化从机解析栈 */
    MBx_Init_Slave_Parse(&MBxSlave->Parse);

    /*  传参有关部分    */
    /* 绑定回调函数部分 */
    MBxSlave->Func.Send = MBxSend;
    MBxSlave->Func.Getc = MBxGetc;
    /* 绑定buffer内存部分 */
    MBxSlave->RxExist.Buffer = RxBuffer;
    MBxSlave->RxExist.LenMAX = RxBufferSize;
    MBxSlave->TxExist.Buffer = TxBuffer;
    MBxSlave->TxExist.LenMAX = TxBufferSize;
    /* 初始化属性 */
    MBx_Init_Attr(&MBxSlave->Attr, MBX_MODEL_RTU, MBX_MODE_SLAVE, BaudRate, MBX_PARA_NULL);
    /* 初始化从机配置 */
    State = MBx_Init_Slave_Config(&MBxSlave->Config, SlaveID, MAP);
    if(State != MBX_API_RETURN_DEFAULT)
        MBx_MODULE_TRACE_ADD_ERR(MBxSlave, State);

    return (State);
}

/**
 * @brief 作为TCP 初始化MBX从机对象
 * @param MBxSlave 期望初始化的MBX从机对象指针
 * @param SlaveID 绑定的从机ID 1~247
 * @param MAP 指向地址映射表头的指针
 * @param MBxSend 绑定的从机发送port函数
 * @param MBxGetc 绑定的从机接收port函数
 * @param RxBuffer 绑定的接收buffer区域
 * @param RxBufferSize 绑定的接收buffer最大长度
 * @param TxBuffer 绑定的发送buffer区域
 * @param TxBufferSize 绑定的发送buffer最大长度
 * @return uint32_t 标准返回
 */
uint32_t MBx_Slave_TCP_Init(_MBX_SLAVE                *MBxSlave, //
                            uint8_t                    SlaveID,
                            const _MBX_MAP_LIST_ENTRY *MAP,
                            MBX_SEND_PTR               MBxSend,
                            MBX_GTEC_PTR               MBxGetc,
                            uint8_t                   *RxBuffer,
                            uint32_t                   RxBufferSize,
                            uint8_t                   *TxBuffer,
                            uint32_t                   TxBufferSize)
{
    uint32_t State = MBX_API_RETURN_DEFAULT;
    /* 传参审查 */
    if((SlaveID < 1) || (SlaveID > 247))
        return MBX_API_RETURN_ERR_PARAM;
    if((MBxSend == NULL) || (MBxGetc == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBuffer == NULL) || (RxBuffer == NULL))
        return MBX_API_RETURN_ERR_PARAM;
    if((RxBufferSize < 30) || (TxBufferSize < 30)) // buffer收发空间至少为30字节(随便写的值)
        return MBX_API_RETURN_ERR_PARAM;

    /* 加入从机链表 */
    NewSlaveChainNode(MbxSChainRoot, MBxSlave);

    /*  传参无关部分    */
    /* 初始化运行时 */
    MBx_Init_Runtime(&MBxSlave->Runtime);
    /* 初始化从机解析栈 */
    MBx_Init_Slave_Parse(&MBxSlave->Parse);

    /*  传参有关部分    */
    /* 绑定回调函数部分 */
    MBxSlave->Func.Send = MBxSend;
    MBxSlave->Func.Getc = MBxGetc;
    /* 绑定buffer内存部分 */
    MBxSlave->RxExist.Buffer = RxBuffer;
    MBxSlave->RxExist.LenMAX = RxBufferSize;
    MBxSlave->TxExist.Buffer = TxBuffer;
    MBxSlave->TxExist.LenMAX = TxBufferSize;
    /* 初始化属性 */
    MBx_Init_Attr(&MBxSlave->Attr, MBX_MODEL_TCP, MBX_MODE_SLAVE, MBX_PARA_NULL, MBX_PARA_NULL);
    /* 初始化从机配置 */
    State = MBx_Init_Slave_Config(&MBxSlave->Config, SlaveID, MAP);
    if(State != MBX_API_RETURN_DEFAULT)
        MBx_MODULE_TRACE_ADD_ERR(MBxSlave, State);

    return (State);
}

#endif /* MBX_SLAVE_ENABLE */
