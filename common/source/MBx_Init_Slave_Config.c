/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Init_Slave_Config.c
 * Author        : yono
 * Date          : 2024-07-27
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX从机配置
    应当是库内调用
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 初始化MBX从机配置的各个参数
 * @param MBxSlaveConfig 指向MBX从机配置结构体的指针
 * @param ID 期望配置成的从机ID
 * @param MAP 指向地址映射表头的指针
 * @return 标准返回
 */
uint32_t MBx_Init_Slave_Config(_MBX_SLAVE_CONFIG *MBxSlaveConfig, uint8_t ID, const _MBX_MAP_LIST_ENTRY *MAP)
{
    uint32_t state   = MBX_API_RETURN_DEFAULT;
    uint16_t i       = 0; // 遍历map并审查
    uint32_t AddrNow = 0; // 当前审查的条目 mb寄存器地址

    /* 基础配置 */
    MBxSlaveConfig->SlaveID = ID;
    MBxSlaveConfig->Map     = MAP;

    /* 审查定义的映射表*/
    for(i = 0;                   // 从0开始遍历
        (i < 0xFFFEU) &&          // 条件1 防止意外无限循环卡死
        (MAP[i].Memory != NULL); // 条件2 映射内存为空，认为不可读，即无效条
        i++)                     // 推进
    {
        if(AddrNow > MAP[i].Addr)
        {
            state = MBX_API_RETURN_MAP_UNFORMAT;
#if MBX_MODULE_ERR_TRACE_ENABLE
            MBxErrTraceAdd(ID, MBX_MODE_SLAVE, 0, MBX_API_RETURN_MAP_UNFORMAT);
#endif
        }
        AddrNow = MAP[i].Addr;
    }
    MBxSlaveConfig->MapNum = i; // 记录条目总数量

    return (state);
}