/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file MBx_api.h
 *     MBx Component
 *     Application Interface (API)
 * 一个功能并不齐全的modbus主从库, 但以如下特性为目标
 * 不阻塞不占线程不占中断、多主多从、寄存器地址表任意映射。是一个纯粹的协议栈, 只需要修改数据流入流出即可驱动。
 */

#ifndef _MBX_API_H_
#define _MBX_API_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <MBx_port.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* modbus协议模式定义 */
#define MBX_MODEL_RTU (1) /*!< 使用modbus RTU协议 */
#define MBX_MODEL_TCP (2) /*!< 使用modbus TCP协议 */
// #define MBX_MODEL_ASCII (3) /*!< 使用modbus ASCII协议 愚蠢的协议, 或许永远不会添加支持 */

/* modbus主从模式定义 */
#define MBX_MODE_SLAVE  (0) /*!< 从站模式 */
#define MBX_MODE_MASTER (1) /*!< 主站模式 */

/* 寄存器类型定义 */
#define MBX_REG_HOLDING  (1) /*!< 保持寄存器 */
#define MBX_REG_INPUT    (2) /*!< 输入寄存器 */
#define MBX_REG_COIL     (3) /*!< 线圈寄存器 */
#define MBX_REG_DISCRETE (4) /*!< 离散输入寄存器 */

/* 功能码定义 */
#define MBX_FUNC_READ_COIL        (1)   /*!< 01 0x01功能码, 读取一组线圈. */
#define MBX_FUNC_READ_DISC_INPUT  (2)   /*!< 02 0x02功能码, 读取一组离散输入. */
#define MBX_FUNC_READ_REG         (3)   /*!< 03 0x03功能码, 读取一个或多个保持寄存器. */
#define MBX_FUNC_READ_INPUT_REG   (4)   /*!< 04 0x04功能码, 读取一个或多个输入寄存器. */
#define MBX_FUNC_WRITE_COIL       (5)   /*!< 05 0x05功能码, 写单个线圈. */
#define MBX_FUNC_WRITE_REG        (6)   /*!< 06 0x06功能码, 写单个保持寄存器. */
#define MBX_FUNC_WRITE_COIL_MUL   (15)  /*!< 15 0x0F功能码, 写多个线圈. */
#define MBX_FUNC_WRITE_REG_MUL    (16)  /*!< 16 0x10功能码, 写入多个保持寄存器. */
#define MBX_FUNC_EXCEPTION_OFFSET (128) /*!< 128 0x80功能码, 是错误帧回复的功能码附加值, 需要正常功能码添加上这个附加值表示异常. */

/* 错误码定义 */
#define MBX_EXCEPTION_NONE   (0) /*!< 00 0x00错误码, 无错误. */
#define MBX_EXCEPTION_UNFUNC (1) /*!< 01 0x01错误码, 未知指令错误. */
#define MBX_EXCEPTION_UNADDR (2) /*!< 02 0x02错误码, 未知数据地址错误. */
#define MBX_EXCEPTION_DATA   (3) /*!< 03 0x03错误码, 数据不合法错误. 实际代表数据段的长度不合法 */
#define MBX_EXCEPTION_FAULT  (4) /*!< 04 0x04错误码, 从站设备故障. 实际代表数据处理(写入或读取时)出现异常，超出支持的数据范围等*/
// #define MBX_EXCEPTION_ACKTIME         5  /*!< 05 0x05错误码, 其实并非错误, 而是收到长耗时指令, 表明已收到并开始处理.(不常用暂不支持) */
#define MBX_EXCEPTION_BUSY   (7) /*!< 07 0x07错误码, 正在处理耗时命令在忙 */
#define MBX_EXCEPTION_PARITY (8) /*!< 08 0x08错误码, 存储奇偶性差错 (指示扩展文件区不能通过一致性校验). */
// #define MBX_EXCEPTION_UNGATEWAY       (10) /*!< 10 0x0A错误码, 网关不可达.(不常用暂不支持) */
// #define MBX_EXCEPTION_UNTARGETGATEWAY (11) /*!< 11 0x0B错误码, 网关目标无响应.(不常用暂不支持) */

/* 非标准错误码，协助开发以及拓展功能 */
#define MBX_EXCEPTION_SAFE    (32)  /*!< 32 0x20错误码, 保护报警错误, 正在报警时对控制类及在线调节命令返回该代码. */
#define MBX_EXCEPTION_CRC     (64)  /*!< 64 0x40错误码, CRC校验错误. */
#define MBX_EXCEPTION_TIMEOUT (128) /*!< 128 0x80错误码, 超时错误. */
#define MBX_EXCEPTION_SENDERR (129) /*!< 129 0x81错误码, 发送失败错误. */

/* 寄存器映射的数据类型 */
#define MBX_REG_TYPE_U8           (1)   /*!< 类型定义, 该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U16          (2)   /*!< 类型定义, 该地址映射的数据是16位的. */
#define MBX_REG_TYPE_U32_L        (3)   /*!< 类型定义, 该地址映射的数据是32位的，映射低16位. */
#define MBX_REG_TYPE_U32_H        (4)   /*!< 类型定义, 该地址映射的数据是32位的，映射高16位. */
#define MBX_REG_TYPE_U64_0        (5)   /*!< 类型定义, 该地址映射的数据是64位的，映射最低的16位. */
#define MBX_REG_TYPE_U64_1        (6)   /*!< 类型定义, 该地址映射的数据是64位的，映射第1个16位. */
#define MBX_REG_TYPE_U64_2        (7)   /*!< 类型定义, 该地址映射的数据是64位的，映射第2个16位. */
#define MBX_REG_TYPE_U64_3        (8)   /*!< 类型定义, 该地址映射的数据是64位的，映射第3个16位. */
#define MBX_REG_TYPE_BIT_ONLY     (15)  /*!< 类型定义, 该地址映射的数据是独立的bit位 */
#define MBX_REG_TYPE_BIT_U8_BASE  (16)  /*!< 类型定义, 该地址映射的数据是8位的，映射最低第0个bit.若映射第1个bit，则使用类型 MBX_REG_TYPE_BIT_U8_BASE+1 */
#define MBX_REG_TYPE_BIT_U16_BASE (32)  /*!< 类型定义, 该地址映射的数据是16位的，映射最低第0个bit.若映射第1个bit，则使用类型 MBX_REG_TYPE_BIT_U16_BASE+1 */
#define MBX_REG_TYPE_BIT_U32_BASE (64)  /*!< 类型定义, 该地址映射的数据是32位的，映射最低第0个bit.若映射第1个bit，则使用类型 MBX_REG_TYPE_BIT_U32_BASE+1 */
#define MBX_REG_TYPE_BIT_U64_BASE (128) /*!< 类型定义, 该地址映射的数据是64位的，映射最低第0个bit.若映射第1个bit，则使用类型 MBX_REG_TYPE_BIT_U64_BASE+1 */

/* modbusx状态机常数 */
#define MBX_STATE_IDLE  (0) /*!< 0 0x00状态定义, 空闲状态. */
#define MBX_STATE_ERROR (1) /*!< 1 0x01状态定义, 错误状态. */
#define MBX_STATE_WAIT  (2) /*!< 2 0x02状态定义, 等待状态. */
#define MBX_STATE_WRITE (3) /*!< 3 0x03状态定义, 发送状态. */
#define MBX_STATE_READ  (4) /*!< 4 0x04状态定义, 接收状态. */

/* 寄存器地址表的结尾 */
#define MBX_MAP_LIST_END {.Addr = 0, .Memory = NULL, .Type = 0, .Handle = NULL}

/* MBX空参数定义 */
#define MBX_PARA_NULL 0

/* API返回集 */
/* 通用返回 */
#define MBX_API_RETURN_DEFAULT        0x00 //默认的无错误
#define MBX_API_RETURN_ERR_INDEFINITE 0x01 //默认的未指定错误
#define MBX_API_RETURN_ERR_PARAM      0x02 //传递参数错误
/* 数据相关返回 */
#define MBX_API_RETURN_DATA_ABOVE_LIMIT 0x10 //数据超出上限
#define MBX_API_RETURN_DATA_BELOW_LIMIT 0x11 //数据超出下限
#define MBX_API_RETURN_DATA_LOSS        0x12 //数据丢失
/* BUFFER相关返回 */
#define MBX_API_RETURN_BUFFER_FULL  0x20 //BUFFER满
#define MBX_API_RETURN_BUFFER_EMPTY 0x21 //BUFFER空
/* 映射表相关返回 */
#define MBX_API_RETURN_MAP_UNFORMAT 0x101 // MAP格式错误
#define MBX_API_RETURN_MAP_UNFIND   0x102 // MAP未查找到错误

/* Exported types ------------------------------------------------------------*/

/***********地址映射表的定义***********/
typedef uint32_t (*MBX_MAP_REG_HANDLE)(void *value); //地址映射表中的写时处理函数 类型定义
/**
 * 定义MBx寄存器地址查询表的单条结构
 */
typedef struct
{
    uint32_t           Addr;   // 此条寄存器地址
    void              *Memory; // 内存区域头地址
    uint8_t            Type;   // 内存区域映射的数据类型
    MBX_MAP_REG_HANDLE Handle; // 写时处理函数
} _MBX_MAP_LIST_ENTRY;

/***********提供主机从机对象 共用的定义***********/
/**
 * @brief buffer定义 
 * 线性buffer 采用指针便于动态申请内存
 * 发送、接收buffer的定义共用, 主机、从机的定义共用
 */
typedef struct
{
    uint8_t *Buffer; // 数据buffer绑定
    uint16_t Len;    // 数据长度变量
    uint16_t LenMAX; // 数据buffer的最大长度
} _MBX_EXIST;

/**
 * @brief modbus对象特性配置
 * 主机、从机的定义共用
 */
typedef struct
{
    uint8_t  ModbusModel:7; // modbus协议栈模型 见 /* modbus协议模式定义 */
    uint8_t  ModbusMode :1; // modbus主从模式 见 /* modbus主从模式定义 */
    uint32_t T1_5_Cycs;     // 1.5个字符应当间隔的轮询周期 us(由于采用轮询驱动, 轮询周期通常至少1ms, 此值意义不大, 如果一定要保证间隔, 应当以微秒级定时器运行驱动轮询)
    uint32_t T3_5_Cycs;     // 3.5个字符应当间隔的轮询周期 us(由于采用轮询驱动, 轮询周期通常至少1ms, 此值意义不大, 如果一定要保证间隔, 应当以微秒级定时器运行驱动轮询)

} _MBX_COMMON_CONFIG;

/**
 * @brief modbus通信发送接收函数的配置
 * 主机、从机的定义共用
 */
typedef struct
{
    MBX_SEND_PTR Send; // 数据发送函数绑定
    MBX_GTEC_PTR Getc; // 数据接收函数绑定
} _MBX_COMMON_FUNCTION;

/**
 * @brief modbus驱动的运行时变量
 */
typedef struct
{
    uint32_t TimeCnt;     // 周期累加, 用于驱动modbus协议
    uint32_t NoComNum :4; // 无通信计数 周期累加, 收到有效消息清空
    uint32_t State    :4; // 运行时状态机
    uint32_t StatePast:4; // 运行时状态机前一态寄存, 状态流转时可以处理数据
    uint32_t StateFlow:1; // 状态机立即流转动作 1立即流转 0按流程流转
    // uint32_t StateWait:1; // 状态机保持等待标识 1保持等待 0流转出去
    uint32_t reg:19; // 补位
} _MBX_COMMON_RUNTIME;

/***********提供从机对象使用的定义***********/
/**
 * @brief 从机特有的配置结构体
 */
typedef struct
{
    uint8_t                    SlaveID; // 从机号绑定
    const _MBX_MAP_LIST_ENTRY *Map;     // 地址映射表头
    uint16_t                   MapNum;  // 地址映射数量，自动遍历map产生
} _MBX_SLAVE_CONFIG;

/**
 * @brief 从机解析栈
 */
typedef struct
{
    uint8_t  Func;      // 解析到的功能码
    uint16_t RegData;   // 待处理的寄存器值
    uint16_t AddrStart; // 待处理的寄存器地址起始
    uint16_t RegNum;    // 待解析的寄存器数量
} _MBX_SLAVE_PARSE_VALUE;

/***********提供主机对象使用的定义***********/
/**
 * @brief 主机特有的配置结构体
 */
typedef struct
{
    uint8_t SlaveID; // 当前操作的从机号
} _MBX_MASTER_CONFIG;

/**
 * @brief 主机下管理的从机的配置结构体
 */
typedef struct _MBX_MASTER_TEAM_MEMBER
{
#if MBX_MODULE_TCP_MASTER_ENABLE
    uint16_t TransID; // modbusTCP专用, 当前累计的事务号
#endif
    uint8_t                         SlaveID; // 从机号绑定
    const _MBX_MAP_LIST_ENTRY      *Map;     // 地址映射表头
    uint16_t                        MapNum;  // 地址映射数量，自动遍历map产生
    struct _MBX_MASTER_TEAM_MEMBER *Next;    // 下一个从机
} _MBX_MASTER_TEAM_MEMBER;

/**
 * @brief 主机解析栈
 */
typedef struct
{
    uint8_t  Func;      // 解析到的功能码
    uint16_t RegData;   // 待处理的寄存器值
    uint16_t AddrStart; // 待处理的寄存器地址起始
    uint16_t RegNum;    // 待解析的寄存器数量

#if MBX_MODULE_TCP_MASTER_ENABLE
    /* 具有TCP主机功能，需要寄存所有的发送请求，需要寄存事务号 */
    uint16_t TransNum[MBX_MASTER_REQUEST_QUEUE_MAX];                                // 发送时的事务号
    uint8_t  SlaveID[MBX_MASTER_REQUEST_QUEUE_MAX];                                 // 发送时的从机ID
    uint8_t  SendFunc[MBX_MASTER_REQUEST_QUEUE_MAX];                                // 发送时的功能码
    uint16_t SendAddrStart[MBX_MASTER_REQUEST_QUEUE_MAX];                           // 发送时的地址起始
    uint16_t SendRegNum[MBX_MASTER_REQUEST_QUEUE_MAX];                              // 发送时的寄存器数量
    uint8_t  SendValue[MBX_MASTER_REQUEST_QUEUE_MAX][MBX_MASTER_MULTI_REG_MAX * 2]; // 发送时的设置值域
    uint8_t  Head;                                                                  // 环形队列的头指针(入)
    uint8_t  Tail;                                                                  // 环形队列的尾指针(出)
#else
    uint8_t  SlaveID;                                 // 发送时的从机ID
    uint8_t  SendFunc;                                // 发送时的功能码
    uint16_t SendAddrStart;                           // 发送时的地址起始
    uint16_t SendRegNum;                              // 发送时的寄存器数量
    uint8_t  SendValue[MBX_MASTER_MULTI_REG_MAX * 2]; // 发送时的设置值域
#endif
} _MBX_MASTER_PARSE_VALUE;

/**
 * @brief 主机对从指令失败的环形栈
 */
typedef struct
{
    uint8_t  SendFunc;      // 产生错误的帧 发送时的功能码
    uint8_t  ErrorCode;     // 产生错误的帧 错误码
    uint16_t SendAddrStart; // 产生错误的帧 发送时的地址起始
    uint16_t SendRegNum;    // 产生错误的帧 发送时的操作的寄存器数量
} _MBX_MASTER_ERROR_RING_NODE;

typedef struct
{
    _MBX_MASTER_ERROR_RING_NODE Queue[MBX_MASTER_ERROR_QUEUE_MAX]; // 错误环形队列
    uint8_t                     Head;                              // 环形队列的头指针(入)
    uint8_t                     Tail;                              // 环形队列的尾指针(出)
} _MBX_MASTER_ERROR_RING;

/**
 * @brief 主机对从指令待发送请求的环形栈
 */
typedef struct
{
    uint8_t  SlaveID;                             // 待发送的从机ID
    uint8_t  Func;                                // 待发送的功能码
    uint16_t AddrStart;                           // 待发送的地址起始
    uint16_t RegNum;                              // 待发送的寄存器数量
    uint8_t  Value[MBX_MASTER_MULTI_REG_MAX * 2]; // 待发送的设置值域
} _MBX_MASTER_REQUEST_RING_NODE;

typedef struct
{
    _MBX_MASTER_REQUEST_RING_NODE Queue[MBX_MASTER_REQUEST_QUEUE_MAX]; // 待发送请求队列
    uint8_t                       Head;                                // 环形队列的头指针(入)
    uint8_t                       Tail;                                // 环形队列的尾指针(出)
} _MBX_MASTER_REQUEST_RING;

/***********主从机对象的定义***********/
/**
 * @brief 定义modbus从机协议对象 
 */
typedef struct _MBX_SLAVE
{
    /* 初始化时需赋固定值的部分 */
    _MBX_COMMON_RUNTIME Runtime; // 运行时变量
    _MBX_COMMON_CONFIG  Attr;    // 属性配置
    /* 需传入初始化函数进行配置的部分 */
    _MBX_EXIST           TxExist; // 供发送的buffer空间
    _MBX_EXIST           RxExist; // 供接收的buffer空间
    _MBX_COMMON_FUNCTION Func;    // 函数绑定
    _MBX_SLAVE_CONFIG    Config;  // 从机配置
    /* 解析过程使用 保持对象独立性 */
    _MBX_SLAVE_PARSE_VALUE Parse; // 解析栈
    /* 自动从机驱动, 链表支持 */
    struct _MBX_SLAVE *Next; // 从机链表指针
} _MBX_SLAVE;

/**
 * @brief 定义modbus主机协议对象 
 */
typedef struct _MBX_MASTER
{
    /* 初始化时需赋固定值的部分 */
    _MBX_COMMON_RUNTIME      Runtime;        // 运行时变量
    _MBX_COMMON_CONFIG       Attr;           // 属性配置
    _MBX_MASTER_CONFIG       Config;         // 主机配置
    _MBX_MASTER_TEAM_MEMBER *SlaveChainRoot; // 从机链头
    /* 需传入初始化函数进行配置的部分 */
    _MBX_EXIST           TxExist; // 供发送的buffer空间
    _MBX_EXIST           RxExist; // 供接收的buffer空间
    _MBX_COMMON_FUNCTION Func;    // 函数绑定
    /* 解析过程使用 保持对象独立性 */
    _MBX_MASTER_PARSE_VALUE  Parse;   // 解析栈
    _MBX_MASTER_ERROR_RING   Error;   // 错误栈
    _MBX_MASTER_REQUEST_RING Request; // 请求栈
    /* 自动主机驱动, 链表支持 */
    struct _MBX_MASTER *Next; // 主机链表指针
} _MBX_MASTER;

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/***** 用户实用API *****/
extern void MBx_Ticks(uint32_t Cycle);
/*从机API*/
extern uint32_t MBx_Slave_RTU_Init(_MBX_SLAVE *MBxSlave, uint8_t SlaveID, const _MBX_MAP_LIST_ENTRY *MAP, MBX_SEND_PTR MBxSend, MBX_GTEC_PTR MBxGetc, uint32_t BaudRate, uint8_t *RxBuffer, uint32_t RxBufferSize, uint8_t *TxBuffer, uint32_t TxBufferSize);
extern uint32_t MBx_Slave_TCP_Init(_MBX_SLAVE *MBxSlave, uint8_t SlaveID, const _MBX_MAP_LIST_ENTRY *MAP, MBX_SEND_PTR MBxSend, MBX_GTEC_PTR MBxGetc, uint8_t *RxBuffer, uint32_t RxBufferSize, uint8_t *TxBuffer, uint32_t TxBufferSize);
/*主机API*/
extern uint32_t MBx_Master_RTU_Init(_MBX_MASTER *MBxMaster, MBX_SEND_PTR MBxSend, MBX_GTEC_PTR MBxGetc, uint32_t BaudRate, uint8_t *RxBuffer, uint32_t RxBufferSize, uint8_t *TxBuffer, uint32_t TxBufferSize);
extern uint32_t MBx_Master_TCP_Init(_MBX_MASTER *MBxMaster, MBX_SEND_PTR MBxSend, MBX_GTEC_PTR MBxGetc, uint8_t *RxBuffer, uint32_t RxBufferSize, uint8_t *TxBuffer, uint32_t TxBufferSize);
extern uint32_t MBx_Master_Member_Add(_MBX_MASTER *MBxMaster, _MBX_MASTER_TEAM_MEMBER *MBxMember, uint8_t SlaveID, const _MBX_MAP_LIST_ENTRY *MAP);
extern uint32_t MBx_Master_Error_Get(_MBX_MASTER *pMaster, uint8_t *Func, uint8_t *Error, uint16_t *AddrStart, uint16_t *RegNum);
extern uint32_t MBx_Master_Read_Coil_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t StartAddr, uint16_t ReadNum);
extern uint32_t MBx_Master_Read_Disc_Input_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t StartAddr, uint16_t ReadNum);
extern uint32_t MBx_Master_Read_Reg_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t StartAddr, uint16_t ReadNum);
extern uint32_t MBx_Master_Read_Input_Reg_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t StartAddr, uint16_t ReadNum);
extern uint32_t MBx_Master_Write_Coil_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t Addr, uint16_t Value);
extern uint32_t MBx_Master_Write_Reg_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t Addr, uint16_t Value);
extern uint32_t MBx_Master_Write_Coil_Mul_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t StartAddr, uint16_t WriteNum, uint8_t *Data, uint16_t DataLen);
extern uint32_t MBx_Master_Write_Reg_Mul_Request(_MBX_MASTER *pMaster, uint8_t SlaveID, uint16_t StartAddr, uint16_t WriteNum, uint8_t *Data, uint16_t DataLen);
/*地址映射表API*/

/* 便于拓展应用的开发, 用户无需调用 */
extern void     MBx_Init_Runtime(_MBX_COMMON_RUNTIME *MBxRuntime);
extern void     MBx_Init_Attr(_MBX_COMMON_CONFIG *MBxAttr, uint8_t Model, uint8_t mode, uint32_t para1, uint32_t para2);
extern void     MBx_Init_Slave_Parse(_MBX_SLAVE_PARSE_VALUE *MBxSlaveParse);
extern uint32_t MBx_Init_Slave_Config(_MBX_SLAVE_CONFIG *MBxSlaveConfig, uint8_t ID, const _MBX_MAP_LIST_ENTRY *MAP);
extern void     MBx_Init_Master_Parse(_MBX_MASTER_PARSE_VALUE *MBxMasterParse);
extern void     MBx_Init_Master_Error(_MBX_MASTER_ERROR_RING *MBxMasterError);
extern void     MBx_Init_Master_Request(_MBX_MASTER_REQUEST_RING *MBxMasterRequest);
extern uint32_t MBx_Init_Master_Config(_MBX_MASTER_TEAM_MEMBER *MBxMasterConfig, uint8_t ID, const _MBX_MAP_LIST_ENTRY *MAP);

/* Include MBX utility and system file.  */
#include "MBx_utility.h"

#ifdef __cplusplus
}
#endif
#endif /* _MBX_API_H_ */
