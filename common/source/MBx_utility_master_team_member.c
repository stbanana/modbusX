/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_utility_master_request.c
 * Author        : yono
 * Date          : 2024-07-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    modbusX主机成员的操作函数(主机有从机成员)
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 寻找与解析从机号对应的唯一成员对象
 * @param pMaster MBX主机对象指针
 * @param SlaveID 期望查找的从机号
 * @return 成员对象指针
 */
_MBX_MASTER_TEAM_MEMBER *MBx_Master_Member_Find(_MBX_MASTER *pMaster, uint8_t SlaveID)
{
    _MBX_MASTER_TEAM_MEMBER *pMember = pMaster->SlaveChainRoot;
    while((pMember != NULL) &&         // 未遍历到链尾
          pMember->SlaveID != SlaveID) // 未找到成员
    {
        pMember = pMember->Next;
    }

    return pMember;
}

#endif /* MBX_MASTER_ENABLE */
