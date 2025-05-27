/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Init_Attr.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX属性
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
 * @brief 初始化MBX属性的各个参数
 * @param MBxAttr 指向MBX属性结构体的指针
 * @param Model 以何种modbus协议模型进行初始化 见 MBx_api.h modbus协议模式定义 部分 
 * @param Mode 以主或从机进行初始化 见 MBx_api.h modbus主从模式定义 部分 
 * @param para1 传参1 
 *              MBX_MODEL_RTU: 此传参为波特率
 * @param para2 传参2
 *              冗余设计 暂未使用
 */
void MBx_Init_Attr(_MBX_COMMON_CONFIG *MBxAttr, uint8_t Model, uint8_t Mode, uint32_t para1, uint32_t para2)
{
    MBX_UNUSED_PARAM(para2)
    MBxAttr->ModbusMode = Mode;

    switch(Model)
    {
    case MBX_MODEL_RTU:
        /* 设置模型 */
        MBxAttr->ModbusModel = MBX_MODEL_RTU;
        /* 计算1.5字符时间 us ,时间计算均以8数据位、无校验位、1停止位为配置 */
        MBxAttr->T1_5_Cycs = (15U * 1000000U) / para1;
        /* 计算3.5字符时间 us ,时间计算均以8数据位、无校验位、1停止位为配置 */
        if(para1 >= 19200U)
        {
            MBxAttr->T3_5_Cycs = 1750;
        }
        else
        {
            MBxAttr->T3_5_Cycs = (35U * 1000000U) / para1;
        }
        break;

    case MBX_MODEL_TCP:
        /* 设置模型 */
        MBxAttr->ModbusModel = MBX_MODEL_TCP;
        MBxAttr->T1_5_Cycs   = 0;
        MBxAttr->T3_5_Cycs   = 0; // 空闲流转依据, 置0
        break;

    default:
        break;
    }
}