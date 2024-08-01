/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file MBx_port.h
 *     MBx Component
 *     Port Specific
 * Generic
 */

#ifndef _MBX_PORT_H_
#define _MBX_PORT_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <assert.h>

#ifdef MBX_INCLUDE_USER_DEFINE_FILE
/* Yes, include the user defines in MBx_user.h. The defines in this file may 
   alternately be defined on the command line.  
   Maybe you can find MBx_user_sample.h*/
#include "MBx_user.h"
#endif

#ifndef MBX_SLAVE_ENABLE
#define MBX_SLAVE_ENABLE 1
#endif

#ifndef MBX_MASTER_ENABLE
#define MBX_MASTER_ENABLE 1
#endif

#ifndef MBX_MODULE_ERR_TRACE_ENABLE
#define MBX_MODULE_ERR_TRACE_ENABLE 1
#endif

#ifndef MBX_SEND_MODE_BYTES
#define MBX_SEND_MODE_BYTES 1
#endif

#ifndef MBX_BUILT_UTILTY_CRC_ENABLE
#define MBX_BUILT_UTILTY_CRC_ENABLE 1
#endif

#ifndef MBX_ENDIAN_MODE_BIG
#define MBX_ENDIAN_MODE_BIG 0
#endif

#ifndef MBX_MASTER_REQUEST_QUEUE_MAX
#define MBX_MASTER_REQUEST_QUEUE_MAX 20
#endif
#if MBX_MASTER_REQUEST_QUEUE_MAX > 0xFF
#error "MBX_MASTER_REQUEST_QUEUE_MAX must be not greater than 0xFF"
#endif

#ifndef MBX_MASTER_ERROR_QUEUE_MAX
#define MBX_MASTER_ERROR_QUEUE_MAX 10
#endif
#if MBX_MASTER_ERROR_QUEUE_MAX > 0xFF
#error "MBX_MASTER_ERROR_QUEUE_MAX must be not greater than 0xFF"
#endif

#ifndef MBX_MASTER_MULTI_REG_MAX
#define MBX_MASTER_MULTI_REG_MAX 0x7F
#endif
#if MBX_MASTER_MULTI_REG_MAX > 0x7F
#error "MBX_MASTER_MULTI_REG_MAX must be not greater than 0x7F"
#endif

#ifndef MBX_MASTER_RESPONSE_TIMEOUT_US
#define MBX_MASTER_RESPONSE_TIMEOUT_US 100000
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* port函数允许返回的操作状态值 */
#define MBX_PORT_RETURN_DEFAULT         0x00 // 默认的无错误
#define MBX_PORT_RETURNT_ERR_INDEFINITE 0x01 // 默认的未定义错误(供临时使用或极简函数返回错误)
#define MBX_PORT_RETURN_BUFFER_FULL     0x20 // BUFFER满
#define MBX_PORT_RETURN_BUFFER_EMPTY    0x21 // BUFFER空
#define MBX_PORT_RETURN_CABLE_BUSY      0x41 // 线路繁忙
#define MBX_PORT_RETURN_CABLE_LOST      0x41 // 线路掉线

/* Exported types ------------------------------------------------------------*/

/* 定义发送port函数的类型 */
#if MBX_SEND_MODE_BYTES
typedef uint32_t (*MBX_SEND_PTR)(const void *Data, size_t Len);
#else
typedef uint32_t (*MBX_SEND_PTR)(const uint8_t *Data);
#endif

/* 定义接收port函数的类型 */
typedef uint32_t (*MBX_GTEC_PTR)(uint8_t *Data);

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _MBX_PORT_H_ */
