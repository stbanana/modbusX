/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Init_Slave_Parse.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX从机解析栈
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
 * @brief 初始化MBX从机解析栈的各个参数
 * @param MBxSlaveParse 指向MBX从机解析栈的指针
 * @param para1 
 * @param para2 
 */
void MBx_Init_Slave_Parse(_MBX_SLAVE_PARSE_VALUE *MBxSlaveParse, uint32_t para1, uint32_t para2)
{
    switch(Model)
    {
    case MBX_MODEL_RTU:
        /* 设置模型 */
        MBxAttr->ModbusModel = MBX_MODEL_RTU;
        /* 计算1.5字符时间 us ,时间计算均以8数据位、无校验位、1停止位为配置 */
        MBxAttr->T1_5_Cycs = (15 * 1000000) / para1;
        /* 计算3.5字符时间 us ,时间计算均以8数据位、无校验位、1停止位为配置 */
        if(para1 >= 19200)
        {
            MBxAttr->T3_5_Cycs = 1750;
        }
        else
        {
            MBxAttr->T3_5_Cycs = (35 * 1000000) / para1;
        }
        break;

    case MBX_MODEL_TCP:
        /* 设置模型 */
        MBxAttr->ModbusModel = MBX_MODEL_TCP;
        MBxAttr->T1_5_Cycs   = 0;
        MBxAttr->T3_5_Cycs   = 0; // 空闲流转依据，置0
        break;

    default:
        break;
    }
}