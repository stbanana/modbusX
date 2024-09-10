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
        不会覆盖旧纪录, 仅追踪最初数条, 使用本源码中的宏控制最大记录条目。
        使用本模块时, 请确保MBX_MODULE_ERR_TRACE_ENABLE宏已定义。
        本源码文件中应当有宏定义控制最大记录的条目数
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MODULE_ERR_TRACE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

_MBX_ERR_TRACE MBxErrTraceStack[MBX_ERR_TRACE_MAX_NUM];
uint8_t        MBxErrTraceTop = 0;

/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 对trace栈进行添加
 * @param SlaveID   从机号, 对于从机是本机从机号, 对于主机是期望与其通信的从机号
 * @param mode 错误出现的对象的类型 0从机 1主机 见Mbx_api.h 的 "modbus主从模式定义" 
 * @param State 对象的运行时状态机 
 * @param ErrCode 错误码 见 MBx_api.h 的 "API返回集" 定义 
*/
void MBxErrTraceAdd(uint8_t SlaveID, uint8_t mode, uint8_t State, uint32_t ErrCode)
{
    if(MBxErrTraceTop >= MBX_ERR_TRACE_MAX_NUM)
    {
        return;
    }
    MBxErrTraceStack[MBxErrTraceTop].SlaveID = SlaveID;
    MBxErrTraceStack[MBxErrTraceTop].mode    = mode;
    MBxErrTraceStack[MBxErrTraceTop].State   = State;
    MBxErrTraceStack[MBxErrTraceTop].ErrCode = ErrCode;
    MBxErrTraceTop++;
}

/**
 * @brief 对trace栈进行一次取出
 * @param SlaveID   从机号, 对于从机是本机从机号, 对于主机是期望与其通信的从机号
 * @param mode 错误出现的对象的类型 0从机 1主机 见Mbx_api.h 的 "modbus主从模式定义" 
 * @param State 对象的运行时状态机
 * @param ErrCode 错误码 见 MBx_api.h 的 API返回集 定义
 * @return 标准返回
*/
uint32_t MBxErrTraceGet(uint8_t *SlaveID, uint8_t *mode, uint8_t *State, uint32_t *ErrCode)
{
    /* 审查错误追踪buffer是否是空 */
    if(MBxErrTraceTop <= 0)
        return MBX_API_RETURN_BUFFER_EMPTY;

    MBxErrTraceTop--;
    *SlaveID = MBxErrTraceStack[MBxErrTraceTop].SlaveID;
    *mode    = MBxErrTraceStack[MBxErrTraceTop].mode;
    *State   = MBxErrTraceStack[MBxErrTraceTop].State;
    *ErrCode = MBxErrTraceStack[MBxErrTraceTop].ErrCode;

    return MBX_API_RETURN_DEFAULT;
}

#endif /* MBX_MODULE_ERR_TRACE_ENABLE */
