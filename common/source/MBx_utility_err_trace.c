/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_utility_err_trace.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbusX错误追踪模块
        不会覆盖旧纪录，仅追踪最初数条，使用本源码中的宏控制最大记录条目。
        使用本模块时，请确保MBX_MODULE_ERR_TRACE_ENABLE宏已定义。
        本源码文件中应当有宏定义控制最大记录的条目数
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MODULE_ERR_TRACE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* 定义最大记录的条目数 */
#define MBX_ERR_TRACE_MAX_NUM 20

/* Private variables ---------------------------------------------------------*/

_MBX_ERR_TRACE MBxErrTraceList[MBX_ERR_TRACE_MAX_NUM];
uint8_t        MBxErrTraceTop = 0;

/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 对trace列表进行添加
 * @param SlaveID   从机号，对于从机是本机从机号，对于主机是期望与其通信的从机号
 * @param mode 错误出现的对象的类型 0从机 1主机
 * @param State 对象的运行时状态机
 * @param ErrCode 错误码 见 MBx_api.h 的 API返回集 定义
*/
void MBxErrTraceAdd(uint8_t SlaveID, uint8_t mode, uint8_t State, uint32_t ErrCode)
{
    if(MBxErrTraceTop >= MBX_ERR_TRACE_MAX_NUM)
    {
        return;
    }
    MBxErrTraceList[MBxErrTraceTop].SlaveID = SlaveID;
    MBxErrTraceList[MBxErrTraceTop].mode    = mode;
    MBxErrTraceList[MBxErrTraceTop].State   = State;
    MBxErrTraceList[MBxErrTraceTop].ErrCode = ErrCode;
    MBxErrTraceTop++;
}

#endif /* MBX_MODULE_ERR_TRACE_ENABLE */
