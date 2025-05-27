/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Init_Master_Config.c
 * Author        : yono
 * Date          : 2024-07-30
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX主机配置
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
 * @param MBxMasterConfig 指向MBX从机解析栈的指针
 * @param ID 期望配置成的从机ID
 * @param MAP 指向地址映射表头的指针
 * @return 标准返回
 */
uint32_t MBx_Init_Master_Config(_MBX_MASTER_TEAM_MEMBER *MBxMasterConfig, uint8_t ID, const _MBX_MAP_LIST_ENTRY *MAP)
{
    uint32_t state   = MBX_API_RETURN_DEFAULT;
    uint16_t i       = 0; // 遍历map并审查
    uint32_t AddrNow = 0; // 当前审查的条目 mb寄存器地址

    /* 基础配置 */
    MBxMasterConfig->SlaveID = ID;
    MBxMasterConfig->Map     = MAP;

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
            MBxErrTraceAdd(ID, MBX_MODE_MASTER, 0, MBX_API_RETURN_MAP_UNFORMAT);
#endif
        }
        AddrNow = MAP[i].Addr;
    }
    MBxMasterConfig->MapNum = i; // 记录条目总数量

    return (state);
}