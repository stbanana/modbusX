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

#ifndef MBX_SEND_MODE_BYTES
#define MBX_SEND_MODE_BYTES 1
#endif

#ifndef MBX_ENDIAN_MODE_BIG
#define MBX_ENDIAN_MODE_BIG 0
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* 定义发送port函数的类型 */
#if MBX_SEND_MODE_BYTES
typedef int (*MBX_SEND_PTR)(const void *data, size_t length);
#else
typedef int (*MBX_SEND_PTR)(const uint8_t *data);
#endif

/* 定义接收port函数的类型 */
typedef int (*MBX_GTEC_PTR)(uint8_t *data);

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _MBX_PORT_H_ */
