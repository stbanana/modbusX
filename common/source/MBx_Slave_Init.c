/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Slave_Init.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化MBX从机对象
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_SLAVE_ENABLE
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
_MBX_SLAVE *MbxSChainRoot = NULL; // 从机链表头
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 作为RTU 初始化MBX从机对象
 * @param MBxSlave 期望初始化的MBX从机对象指针
 * @param SlaveID 绑定的从机ID 1~247
 * @param MBxSend 绑定的从机发送port函数
 * @param MBxGetc 绑定的从机接收port函数
 * @return uint32_t 标准返回
 */
uint32_t MBx_Slave_RTU_Init(_MBX_SLAVE *MBxSlave, uint8_t SlaveID, MBX_SEND_PTR MBxSend, MBX_GTEC_PTR MBxGetc, uint32_t BaudRate)
{
    /* 传参审查 */
    if(SlaveID < 1 || SlaveID > 247)
        return MBX_API_RETURN_ERR_PARAM;
    if(MBxSend == NULL || MBxGetc == NULL)
        return MBX_API_RETURN_ERR_PARAM;

    /* 加入链表 */
    NewSlaveChainNode(MbxSChainRoot, MBxSlave);
    /* 绑定回调函数部分 */
    MBxSlave->Func.Send = MBxSend;
    MBxSlave->Func.Getc = MBxGetc;
    /* 初始化运行时 */
    MBx_Init_Runtime(&MBxSlave->Runtime);
    /* 初始化属性 */
    MBx_Init_Attr(&MBxSlave->Attr, MBX_MODEL_RTU, MBX_MODE_SLAVE, BaudRate, MBX_PARA_NULL);
    /* 初始化从机解析栈 */
    MBx_Init_Slave_Parse(&MBxSlave->Parse);

    return MBX_API_RETURN_DEFAULT;
}

#endif /* MBX_SLAVE_ENABLE */