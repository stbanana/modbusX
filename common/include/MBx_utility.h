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
#define NewSlaveChainNode(rootNode, newNode)                   \
    do                                                         \
    {                                                          \
        _MBX_SLAVE *ChainNow = (rootNode);                     \
        uint8_t     isFound  = 0;                              \
        if(ChainNow == NULL)                                   \
        {                                                      \
            (rootNode) = (newNode);                            \
        }                                                      \
        else                                                   \
        {                                                      \
            while((ChainNow->Next != NULL) && (isFound == 0U)) \
            {                                                  \
                if(ChainNow == (newNode))                      \
                {                                              \
                    isFound = 1;                               \
                }                                              \
                ChainNow = ChainNow->Next;                     \
            }                                                  \
            if(isFound == 0)                                   \
            {                                                  \
                memset((newNode), 0, sizeof(_MBX_SLAVE));      \
                ChainNow->Next = (newNode);                    \
            }                                                  \
        }                                                      \
    }                                                          \
    while(0)

/**
 * @brief 链表结构工具
 *          将新的主机节点链接到主机链表上
 *          若已存在链表上, 则不进行操作
 * @param rootNode 根节点指针 _MBX_MASTER*类型
 * @param newNode 期望新链接的节点指针 _MBX_MASTER*类型
 */
#define NewMasterChainNode(rootNode, newNode)                  \
    do                                                         \
    {                                                          \
        _MBX_MASTER *ChainNow = (rootNode);                    \
        uint8_t      isFound  = 0;                             \
        if(ChainNow == NULL)                                   \
        {                                                      \
            (rootNode) = (newNode);                            \
        }                                                      \
        else                                                   \
        {                                                      \
            while((ChainNow->Next != NULL) && (isFound == 0U)) \
            {                                                  \
                if(ChainNow == (newNode))                      \
                {                                              \
                    isFound = 1;                               \
                }                                              \
                ChainNow = ChainNow->Next;                     \
            }                                                  \
            if(isFound == 0U)                                  \
            {                                                  \
                memset((newNode), 0, sizeof(_MBX_MASTER));     \
                ChainNow->Next = (newNode);                    \
            }                                                  \
        }                                                      \
    }                                                          \
    while(0)

/**
 * @brief 向MBX对象的RXbuffer推入一个字节数据
 * @param pMBX  MBX对象指针
 * @param c 期望推入的 字节数据
 */
#define MBxRxBufferPutc(pMBX, c)                                          \
    do                                                                    \
    {                                                                     \
        if(((pMBX)->RxExist.Len + 1U) < (pMBX)->RxExist.LenMAX)           \
        {                                                                 \
            (pMBX)->RxExist.Buffer[(pMBX)->RxExist.Len] = (c);            \
            (pMBX)->RxExist.Len++;                                        \
        }                                                                 \
        else                                                              \
        {                                                                 \
            MBx_MODULE_TRACE_ADD_ERR((pMBX), MBX_API_RETURN_BUFFER_FULL); \
        }                                                                 \
    }                                                                     \
    while(0)

/**
 * @brief MBX对象的RXbuffer删除部分长度
 * @param pMBX  MBX对象指针
 * @param rLen 期望删除的 字节长度
 */
#define MBxRxBufferRemove(pMBX, rLen)                                                                          \
    do                                                                                                         \
    {                                                                                                          \
        (pMBX)->RxExist.Len -= (rLen);                                                                         \
        if((pMBX)->RxExist.Len > 0U)                                                                           \
        {                                                                                                      \
            MBx_utility_MemMove((pMBX)->RxExist.Buffer, (pMBX)->RxExist.Buffer + (rLen), (pMBX)->RxExist.Len); \
        }                                                                                                      \
    }                                                                                                          \
    while(0)

/**
 * @brief 向MBX对象的TXbuffer推入一个字节数据
 * @param pMBX  MBX对象指针
 * @param c 期望推入的 字节数据
 */
#define MBxTxBufferPutc(pMBX, c)                                          \
    do                                                                    \
    {                                                                     \
        if(((pMBX)->TxExist.Len + 1) < (pMBX)->TxExist.LenMAX)            \
        {                                                                 \
            (pMBX)->TxExist.Buffer[(pMBX)->TxExist.Len] = (c);            \
            (pMBX)->TxExist.Len++;                                        \
        }                                                                 \
        else                                                              \
        {                                                                 \
            MBx_MODULE_TRACE_ADD_ERR((pMBX), MBX_API_RETURN_BUFFER_FULL); \
        }                                                                 \
    }                                                                     \
    while(0)

/**
 * @brief 向MBX对象的TXbuffer推入一个寄存器的数据(16位)
 * @param pMBX  MBX对象指针
 * @param reg 期望推入的 寄存器的数据(16位)
 */
#define MBxTxBufferPutReg(pMBX, reg)                                                            \
    do                                                                                          \
    {                                                                                           \
        if(((pMBX)->TxExist.Len + 2) < (pMBX)->TxExist.LenMAX)                                  \
        {                                                                                       \
            (pMBX)->TxExist.Buffer[(pMBX)->TxExist.Len]     = (uint8_t)(((reg) >> 8) & 0x00FF); \
            (pMBX)->TxExist.Buffer[(pMBX)->TxExist.Len + 1] = (uint8_t)((reg)&0x00FF);          \
            (pMBX)->TxExist.Len += 2;                                                           \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            MBx_MODULE_TRACE_ADD_ERR((pMBX), MBX_API_RETURN_BUFFER_FULL);                       \
        }                                                                                       \
    }                                                                                           \
    while(0)

/**
 * @brief 清空MBX对象的RXbuffer
 * @param pMBX  MBX对象指针
 */
#define MBxRxBufferEmpty(pMBX)   \
    do                           \
    {                            \
        (pMBX)->RxExist.Len = 0; \
    }                            \
    while(0)

/**
 * @brief 清空MBX对象的TXbuffer
 * @param pMBX  MBX对象指针
 */
#define MBxTxBufferEmpty(pMBX)   \
    do                           \
    {                            \
        (pMBX)->TxExist.Len = 0; \
    }                            \
    while(0)

/**
 * @brief 查询MBX主机错误栈是否为空
 * @param pMBX  MBX对象指针
 */
#define MBxMasterErrorEmptyQ(pMBX) ((pMBX)->Error.Head == (pMBX)->Error.Tail)

/**
 * @brief 查询MBX主机错误栈是否为满
 * @param pMBX  MBX对象指针
 */
#define MBxMasterErrorFullQ(pMBX) ((((pMBX)->Error.Head + 1) % MBX_MASTER_ERROR_QUEUE_MAX) == (pMBX)->Error.Tail)

/**
 * @brief 查询MBX主机请求栈是否为空
 * @param pMBX  MBX对象指针
 */
#define MBxMasterRequestEmptyQ(pMBX) ((pMBX)->Request.Head == (pMBX)->Request.Tail)

/**
 * @brief 查询MBX主机请求栈是否为满
 * @param pMBX  MBX对象指针
 */
#define MBxMasterRequestFullQ(pMBX) ((((pMBX)->Request.Head + 1) % MBX_MASTER_REQUEST_QUEUE_MAX) == (pMBX)->Request.Tail)

/* 错误追踪处理添加 */
#if MBX_MODULE_ERR_TRACE_ENABLE
#ifdef MBx_MODULE_TRACE_ADD_ERR
#error "MBx_MODULE_TRACE_ADD_ERR is already defined"
#endif
#define MBx_MODULE_TRACE_ADD_ERR(pMB, err) MBxErrTraceAdd((pMB)->Config.SlaveID, (pMB)->Attr.ModbusMode, (pMB)->Runtime.State, (err))
#else
#define MBx_MODULE_TRACE_ADD_ERR(pMB, err)
#endif

/* 定义map映射查找模式 */
#define MBX_MAP_FIND_MODE_FIRST      0 /* 定义首次查找 */
#define MBX_MAP_FIND_MODE_CONTINUOUS 1 /* 定义继续查找 */

/* 未使用参数标识 去警告 */
#define MBX_UNUSED_PARAM(x) ((void)(x));

/* Exported types ------------------------------------------------------------*/
/**
 * @brief 一个方便的CRC16结构
 */
typedef struct
{
    uint8_t L8; // 低8位
    uint8_t H8; // 高8位
} _MBX_CRC16_H_L;
typedef union _MBX_CRC16_R_
{
    uint16_t       Val;
    _MBX_CRC16_H_L H_L;
} _MBX_CRC16;

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
#if MBX_MODULE_ERR_TRACE_ENABLE
extern _MBX_ERR_TRACE MBxErrTraceStack[];
#endif
/* Exported functions ---------------------------------------------------------*/

extern uint16_t                 MBx_utility_crc16(uint8_t *data, uint16_t len);

extern void                     MBx_utility_MemMove(void *dest, const void *src, size_t n);

extern void                     MBxErrTraceAdd(uint8_t SlaveID, uint8_t mode, uint8_t State, uint32_t ErrCode);
extern uint32_t                 MBxErrTraceGet(uint8_t *SlaveID, uint8_t *mode, uint8_t *State, uint32_t *ErrCode);

extern uint32_t                 MBx_utility_map_addr_data_read(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapNum, uint16_t Addr, uint16_t *Data, uint8_t mode);
extern uint32_t                 MBx_utility_map_r_continuity_review(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t AddrStart, uint16_t RegNum);
extern uint32_t                 MBx_utility_map_addr_data_write(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t Addr, uint16_t Data, uint8_t mode);
extern uint32_t                 MBx_utility_map_addr_data_write_cast(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t Addr, uint16_t Data, uint8_t mode);
extern uint32_t                 MBx_utility_map_w_continuity_review(const _MBX_MAP_LIST_ENTRY *Map, uint16_t MapMax, uint16_t AddrStart, uint16_t RegNum);
extern uint32_t                 MBx_utility_map_w_cooperate_review(void);

extern uint32_t                 MBxMasterErrortAdd(_MBX_MASTER *pMaster, uint8_t Func, uint8_t Error, uint16_t AddrStart, uint16_t RegNum);

extern void                     MBxMasterRequestToTx(_MBX_MASTER *pMaster);
extern uint32_t                 MBxMasterRequestAdd(_MBX_MASTER *pMaster, uint8_t SlaveID, uint8_t Func, uint16_t AddrStart, uint16_t RegNum, uint8_t *Value, uint16_t ValueLen);

extern _MBX_MASTER_TEAM_MEMBER *MBx_Master_Member_Find(_MBX_MASTER *pMaster, uint8_t SlaveID);

#ifdef __cplusplus
}
#endif
#endif /* _MBX_UTILITY_H_ */
