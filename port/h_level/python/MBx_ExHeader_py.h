/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file MBx_ExHeader_py.h
 *     MBx Ex Component
 *     Extra header file (ExHeader)
 */
#ifndef _MBX_PY_EXHEADER_H_
#define _MBX_PY_EXHEADER_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Exported types ------------------------------------------------------------*/
/* _ 符号在python中认为是匿名，对于api所需的类型进行额外兼容层命名 */
typedef _MBX_MASTER             MBX_MASTER;
typedef _MBX_SLAVE              MBX_SLAVE;
typedef _MBX_MASTER_TEAM_MEMBER MBX_MASTER_TEAM_MEMBER;
typedef _MBX_MAP_LIST_ENTRY     MBX_MAP_LIST_ENTRY;

#ifdef __cplusplus
}
#endif
#endif /* _MBX_PY_EXHEADER_H_ */
