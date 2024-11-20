/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Init_Master_Parse.c
 * Author        : yono
 * Date          : 2024-07-30
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX主机解析栈
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
 * @brief 初始化MBX主机解析栈的各个参数
 * @param MBxMasterParse 指向MBX主机解析栈的指针
 */
void MBx_Init_Master_Parse(_MBX_MASTER_PARSE_VALUE *MBxMasterParse)
{
    MBX_UNUSED_PARAM(MBxMasterParse)
#if MBX_MODULE_TCP_MASTER_ENABLEHead
    MBxMasterParse.Head = 0;
    MBxMasterParse.Tail = 0;
#endif
}