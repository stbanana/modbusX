/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_RTU_Handle.c
 * Author        : yono
 * Date          : 2024-07-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbus RTU从机消息系统的底层消息处理, 内部函数, 不应由用户调用
     此时应当已经提取了完整指令
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief modbus RTU从机消息系统的底层消息处理 读取一组线圈
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_READ_COIL_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 读取一组线圈
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_READ_INPUTL_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 读取一个或多个保持寄存器
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_READ_REG_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 读取一个或多个输入寄存器
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_READ_INPUT_REG_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 写单个线圈
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_WRITE_COIL_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 写单个保持寄存器
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_WRITE_REG_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 写多个线圈
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_WRITE_COIL_MUL_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

/**
 * @brief modbus RTU从机消息系统的底层消息处理 写入多个保持寄存器
 * @param pSlave MBX从机对象指针
 */
void MBx_Slave_RTU_WRITE_REG_MUL_Handle(_MBX_SLAVE *pSlave)
{
    // 当前无处理
}

#endif /* MBX_SLAVE_ENABLE */