/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file MBx_utilty.h
 *     MBx Component
 *     Utility
 * 一些代码工具
 */

#ifndef _MBX_UTILITY_H_
#define _MBX_UTILITY_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/**
 * @brief 链表结构工具
 *          将新的从机节点链接到从机链表上
 *          若已存在链表上，则不进行操作
 * @param rootNode 根节点指针 _MBX_SLAVE*类型
 * @param newNode 期望新链接的节点指针 _MBX_SLAVE*类型
 */
#define NewSlaveChainNode(rootNode, newNode)              \
    _MBX_SLAVE *ChainNow = rootNode;                      \
    uint8_t     isFound  = 0;                             \
    if(ChainNow == NULL)                                  \
    {                                                     \
        ChainNow = newNode;                               \
    }                                                     \
    else                                                  \
    {                                                     \
        while((ChainNow->Next != NULL) && (isFound == 0)) \
        {                                                 \
            if(ChainNow == newNode)                       \
            {                                             \
                isFound = 1;                              \
            }                                             \
            ChainNow = ChainNow->Next;                    \
        }                                                 \
        if(isFound == 0)                                  \
        {                                                 \
            memset(newNode, 0, sizeof(_MBX_SLAVE));       \
            ChainNow->Next = newNode;                     \
        }                                                 \
    }

/**
 * @brief 向从机的RXbuffer推入一个字节数据
 * @param pMBX  从机对象指针
 * @param c 期望推入的 字节数据
 * @return uint32_t 标准返回
 */
static inline uint32_t MBxSlaveRxBufferPutc(_MBX_SLAVE *pMBX, uint8_t c)
{
    if((pMBX->RxExist.Len + 1) < pMBX->RxExist.LenMAX)
    {
        pMBX->RxExist.Buffer[pMBX->RxExist.Len] = c;
        pMBX->RxExist.Len++;
        return MBX_API_RETURN_DEFAULT;
    }
    else
    {
#if MBX_MODULE_ERR_TRACE_ENABLE
        MBxErrTraceAdd(pMBX->Config.SlaveID, 0, pMBX->Runtime.State, MBX_API_RETURN_BUFFER_FULL);
#endif
        return MBX_API_RETURN_BUFFER_FULL;
    }
}

/* Exported types ------------------------------------------------------------*/

/**
 * @brief MBx的错误追踪单条目设定，于模块源码MBx_utility_err_trace.c中声明错误追踪队列
 */
typedef struct
{
    uint8_t  SlaveID; // 错误出现的从机ID 对于从机是本机从机号，对于主机是期望与其通信的从机号
    uint8_t  mode :1; // 错误出现的对象的类型 0从机 1主机
    uint8_t  State:4; // 错误出现时对象的运行时状态机
    uint8_t  reg  :3; // 填位
    uint32_t ErrCode; // 错误码 见MBx_api.h的/* API返回集 */定义
} _MBX_ERR_TRACE;

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _MBX_UTILITY_H_ */
