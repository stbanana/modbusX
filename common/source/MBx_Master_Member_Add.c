/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : MBx_Master_Member_Add.c
 * Author        : yono
 * Date          : 2024-07-30
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    向MBX主机对象添加期望管理的从机成员对象
*/

/* Includes ------------------------------------------------------------------*/
#include <MBx_api.h>
#if MBX_MASTER_ENABLE
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 添加一个期望管理的从机成员对象给主机对象
 * @param MBxMaster 期望添加到的MBX主机对象指针
 * @param MBxMember 期望添加的从机成员对象指针
 * @param SlaveID 期望添加的从机成员的从机ID
 * @param MAP 期望添加的从机成员的映射表
 * @return uint32_t 标准返回
 */
uint32_t MBx_Master_Member_Add(_MBX_MASTER               *MBxMaster, //
                               _MBX_MASTER_TEAM_MEMBER   *MBxMember,
                               uint8_t                    SlaveID,
                               const _MBX_MAP_LIST_ENTRY *MAP)
{
    uint32_t                 state    = MBX_API_RETURN_DEFAULT;
    _MBX_MASTER_TEAM_MEMBER *ChainNow = NULL;
    uint32_t                 isFound  = 0;
    uint16_t                 i; // 遍历map并审查
#if MBX_MODULE_ERR_TRACE_ENABLE
    uint32_t AddrNow; // 当前审查的条目 mb寄存器地址
    AddrNow = 0;
#endif

    /* 传参审查 */
    if(MBxMaster == NULL || MBxMember == NULL || MAP == NULL)
        return MBX_API_RETURN_ERR_PARAM;

    /* 添加成员 */
    ChainNow = MBxMaster->SlaveChainRoot;
    if(ChainNow == NULL)
    {
        MBxMaster->SlaveChainRoot = MBxMember;
    }
    else
    {
        while((ChainNow->Next != NULL) && (isFound == 0U))
        {
            if(ChainNow == MBxMember)
            {
                isFound |= 0x01U;
            }
            if(ChainNow->SlaveID == SlaveID)
            {
                isFound |= 0x02U;
            }
            ChainNow = ChainNow->Next;
        }
        if(isFound == 0U)
        {
            memset(MBxMember, 0, sizeof(_MBX_MASTER_TEAM_MEMBER));
            ChainNow->Next = MBxMember;
        }
    }

    if((isFound & 0x02U) != 0U) // 表明从机号重复
    {
        return MBX_API_RETURN_ERR_PARAM;
    }

    /* 设置成员 */
    MBxMember->SlaveID = SlaveID;
    MBxMember->Map     = MAP;

    /* 审查定义的映射表*/
    for(i = 0;                   // 从0开始遍历
        (i < 0xFFFEU) &&          // 条件1 防止意外无限循环卡死
        (MAP[i].Memory != NULL); // 条件2 映射内存为空，认为不可读，即无效条
        i++)                     // 推进
    {
#if MBX_MODULE_ERR_TRACE_ENABLE
        if(AddrNow > MAP[i].Addr)
        {
            state = MBX_API_RETURN_MAP_UNFORMAT;
        }
        AddrNow = MAP[i].Addr;
#endif
    }
    MBxMember->MapNum = i; // 记录条目总数量

    return state;
}

#endif /* MBX_MASTER_ENABLE */
