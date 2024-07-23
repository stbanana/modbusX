/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file MBx_api.h
 *     MBx Component
 *     Application Interface (API)
 * 一个功能并不齐全的modbus主从库，但以如下特性为目标
 * 不阻塞不占线程不占中断、多主多从、寄存器地址表任意映射。是一个纯粹的协议栈，只需要修改数据流入流出即可驱动。
 */

#ifndef _MBX_API_H_
#define _MBX_API_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <MBx_port.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* modbus协议模式定义 */
#define MBX_MODEL_RTU (1) /*!< 使用modbus RTU协议 */
#define MBX_MODEL_TCP (2) /*!< 使用modbus TCP协议 */
// #define MBX_MODEL_ASCII (3) /*!< 使用modbus ASCII协议 愚蠢的协议，或许永远不会添加支持 */

/* 寄存器类型定义 */
#define MBX_REG_HOLDING  (1) /*!< 保持寄存器 */
#define MBX_REG_INPUT    (2) /*!< 输入寄存器 */
#define MBX_REG_COIL     (3) /*!< 线圈寄存器 */
#define MBX_REG_DISCRETE (4) /*!< 离散输入寄存器 */

/* 功能码定义 */
#define MBX_FUNC_READ_COIL        (1)   /*!< 01 0x01功能码，读取一组线圈. */
#define MBX_FUNC_READ_INPUT       (2)   /*!< 02 0x02功能码，读取一组离散输入. */
#define MBX_FUNC_READ_REG         (3)   /*!< 03 0x03功能码，读取一个或多个保持寄存器. */
#define MBX_FUNC_READ_INPUT_REG   (4)   /*!< 04 0x04功能码，读取一个或多个输入寄存器. */
#define MBX_FUNC_WRITE_COIL       (5)   /*!< 05 0x05功能码，写单个线圈. */
#define MBX_FUNC_WRITE_REG        (6)   /*!< 06 0x06功能码，写单个保持寄存器. */
#define MBX_FUNC_WRITE_COIL_MUL   (15)  /*!< 15 0x0F功能码，写多个线圈. */
#define MBX_FUNC_WRITE_REG_MUL    (16)  /*!< 16 0x10功能码，写入多个保持寄存器. */
#define MBX_FUNC_EXCEPTION_OFFSET (128) /*!< 128 0x80功能码，是错误帧回复的功能码附加值，需要正常功能码添加上这个附加值表示异常. */

/* 错误码定义 */
#define MBX_EXCEPTION_NONE   (0) /*!< 00 0x00错误码，无错误. */
#define MBX_EXCEPTION_UNFUNC (1) /*!< 01 0x01错误码，未知指令错误. */
#define MBX_EXCEPTION_UNADDR (2) /*!< 02 0x02错误码，未知数据地址错误. */
#define MBX_EXCEPTION_DATA   (3) /*!< 03 0x03错误码，数据不合法错误. */
#define MBX_EXCEPTION_LEN    (4) /*!< 04 0x04错误码，数据长度不合法错误. */
// #define MBX_EXCEPTION_ACKTIME         5  /*!< 05 0x05错误码，其实并非错误，而是收到长耗时指令，表明已收到并开始处理.(不常用暂不支持) */
#define MBX_EXCEPTION_BUSY   (7) /*!< 07 0x07错误码，正在处理耗时命令在忙 */
#define MBX_EXCEPTION_MASTER (8) /*!< 08 0x08错误码，主控口错误(通常是奇偶校验不通过). */
// #define MBX_EXCEPTION_UNGATEWAY       (10) /*!< 10 0x0A错误码，网关不可达.(不常用暂不支持) */
// #define MBX_EXCEPTION_UNTARGETGATEWAY (11) /*!< 11 0x0B错误码，网关目标无响应.(不常用暂不支持) */
#define MBX_EXCEPTION_SAFE (32) /*!< 32 0x20错误码，保护报警错误，正在报警时对控制类及在线调节命令返回该代码. */
#define MBX_EXCEPTION_CRC  (64) /*!< 64 0x40错误码，CRC校验错误. */

/* 寄存器的类型 */
#define MBX_REG_TYPE_U8    (1) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U16   (2) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U32_H (3) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U32_L (4) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U64_3 (5) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U64_2 (6) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U64_1 (7) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */
#define MBX_REG_TYPE_U64_0 (8) /*!< 1 0x01类型定义，该地址映射的数据是8位的. */

/* Exported types ------------------------------------------------------------*/

/***********主机从机共用的定义***********/
/**
 * @brief buffer定义 
 * 线性buffer 采用指针便于动态申请内存
 * 发送、接收buffer的定义共用，主机、从机的定义共用
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
    uint32_t T3_5_Cycs; // 3.5个字符应当间隔的轮询周期 us(由于采用轮询驱动，轮询周期通常至少1ms，此值意义不大，如果一定要保证间隔，应当以微妙级定时器运行驱动轮询)
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
    uint32_t TimeCnt; // 周期累加，用于驱动modbus协议
    uint8_t  State;   // 运行时状态机
} _MBX_COMMON_RUNTIME;

/***********从机使用的定义***********/
typedef struct
{
    uint8_t  SlaveID;  //从机号绑定
    uint16_t NoComNum; //无通信计数 周期累加，收到有效消息清空
} _MBX_SLAVE_CONFIG;

/**
 * @brief 定义modbus从机协议对象 
 */
typedef struct
{
    _MBX_EXIST           TxExist;
    _MBX_EXIST           RxExist;
    _MBX_COMMON_FUNCTION Func;

    _MBX_SLAVE_CONFIG    Config;
} _MBX_SLAVE;
/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _MBX_API_H_ */
