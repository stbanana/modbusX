/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file MBx_utilty.h
 *     MBx Component
 *     Utility
 *  一些代码工具, 均为库内调用
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
 *          若已存在链表上, 则不进行操作
 * @param rootNode 根节点指针 _MBX_SLAVE*类型
 * @param newNode 期望新链接的节点指针 _MBX_SLAVE*类型
 */
#define NewSlaveChainNode(rootNode, newNode)                  \
    {                                                         \
        _MBX_SLAVE *ChainNow = rootNode;                      \
        uint8_t     isFound  = 0;                             \
        if(ChainNow == NULL)                                  \
        {                                                     \
            rootNode = newNode;                               \
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
        }                                                     \
    }

/**
 * @brief 向MBX对象的RXbuffer推入一个字节数据
 * @param pMBX  MBX对象指针
 * @param c 期望推入的 字节数据
 */
#define MBxRxBufferPutc(pMBX, c)                                        \
    {                                                                   \
        if((pMBX->RxExist.Len + 1) < pMBX->RxExist.LenMAX)              \
        {                                                               \
            pMBX->RxExist.Buffer[pMBX->RxExist.Len] = c;                \
            pMBX->RxExist.Len++;                                        \
        }                                                               \
        else                                                            \
        {                                                               \
            MBx_MODULE_TRACE_ADD_ERR(pMBX, MBX_API_RETURN_BUFFER_FULL); \
        }                                                               \
    }

/**
 * @brief 向MBX对象的TXbuffer推入一个字节数据
 * @param pMBX  MBX对象指针
 * @param c 期望推入的 字节数据
 */
#define MBxTxBufferPutc(pMBX, c)                                        \
    {                                                                   \
        if((pMBX->TxExist.Len + 1) < pMBX->TxExist.LenMAX)              \
        {                                                               \
            pMBX->TxExist.Buffer[pMBX->RxExist.Len] = c;                \
            pMBX->TxExist.Len++;                                        \
        }                                                               \
        else                                                            \
        {                                                               \
            MBx_MODULE_TRACE_ADD_ERR(pMBX, MBX_API_RETURN_BUFFER_FULL); \
        }                                                               \
    }

/**
 * @brief 清空MBX对象的RXbuffer
 * @param pMBX  MBX对象指针
 */
#define MBxRxBufferEmpty(pMBX) \
    {                          \
        pMBX->RxExist.Len = 0; \
    }

/**
 * @brief 清空MBX对象的TXbuffer
 * @param pMBX  MBX对象指针
 */
#define MBxTxBufferEmpty(pMBX) \
    {                          \
        pMBX->TxExist.Len = 0; \
    }

/* 错误追踪处理添加, 具有重定义避免 */
#if MBX_MODULE_ERR_TRACE_ENABLE
#ifdef MBx_MODULE_TRACE_ADD_ERR
#error "MBx_MODULE_TRACE_ADD_ERR is already defined"
#endif
#define MBx_MODULE_TRACE_ADD_ERR(pMB, err) MBxErrTraceAdd(pMB->Config.SlaveID, pMB->Attr.ModbusMode, pMB->Runtime.State, err)
#else
#define MBx_MODULE_TRACE_ADD_ERR(pMB, err)
#endif

/* 定义map映射查找模式 */
#define MBX_MAP_FIND_MODE_FIRST      0 /* 定义首次查找 */
#define MBX_MAP_FIND_MODE_CONTINUOUS 1 /* 定义继续查找 */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief MBx的错误追踪单条目设定, 于模块源码MBx_utility_err_trace.c中声明错误追踪队列
 */
typedef struct
{
    uint8_t  SlaveID; // 错误出现的从机ID 对于从机是本机从机号, 对于主机是期望与其通信的从机号
    uint8_t  mode :1; // 错误出现的对象的类型 0从机 1主机
    uint8_t  State:4; // 错误出现时对象的运行时状态机
    uint8_t  reg  :3; // 填位
    uint32_t ErrCode; // 错误码 见MBx_api.h的/* API返回集 */定义
} _MBX_ERR_TRACE;

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

extern uint16_t MBx_utility_crc16(uint8_t *data, uint16_t len);

extern void     MBxErrTraceAdd(uint8_t SlaveID, uint8_t mode, uint8_t State, uint32_t ErrCode);

extern uint32_t MBx_utility_map_addr_data_get(_MBX_MAP_LIST_ENTRY *Map, uint16_t MapNum, uint16_t Addr, uint16_t *Data, uint8_t mode);

#ifdef __cplusplus
}
#endif
#endif /* _MBX_UTILITY_H_ */
