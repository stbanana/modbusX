/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Init_Runtime.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX运行时
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
 * @brief 初始化MBX运行时的各个参数
 * @param MBxRuntime 指向MBX运行时结构体的指针
 */
void MBx_Init_Runtime(_MBX_COMMON_RUNTIME *MBxRuntime)
{
    MBxRuntime->TimeCnt   = 0U;
    MBxRuntime->NoComNum  = 0U;
    MBxRuntime->State     = MBX_STATE_IDLE;
    MBxRuntime->StatePast = MBX_STATE_IDLE;
    MBxRuntime->StateFlow = 0U;
}
