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
    modbusX请求栈的操作函数(主机专有请求栈)
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
 * @brief MBX主机提取一条请求，填充到Txbuffer
 *          调用前需使用MBxMasterRequestEmptyQ()检查请求栈是否为空
 * @param pMBX  MBX对象指针
 */
void MBxMasterRequestToTx(_MBX_MASTER *pMaster)
{
    _MBX_CRC16 crc;
    MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].SlaveID);
    pMaster->Config.SlaveID = pMaster->Request.Queue[pMaster->Request.Tail].SlaveID;
    pMaster->Parse.SlaveID  = pMaster->Request.Queue[pMaster->Request.Tail].SlaveID;
    MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Func);
    pMaster->Parse.SendFunc = pMaster->Request.Queue[pMaster->Request.Tail].Func;
    MBxTxBufferPutReg(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].AddrStart);
    pMaster->Parse.SendAddrStart = pMaster->Request.Queue[pMaster->Request.Tail].AddrStart;
    switch(pMaster->Request.Queue[pMaster->Request.Tail].Func)
    {
    case MBX_FUNC_READ_COIL:
    case MBX_FUNC_READ_DISC_INPUT:
    case MBX_FUNC_READ_REG:
    case MBX_FUNC_READ_INPUT_REG:
        MBxTxBufferPutReg(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].RegNum);
        pMaster->Parse.SendRegNum = pMaster->Request.Queue[pMaster->Request.Tail].RegNum;
        break;
    case MBX_FUNC_WRITE_COIL:
        MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Value[0]);
        MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Value[1]);
        pMaster->Parse.SendValue[0] = pMaster->Request.Queue[pMaster->Request.Tail].Value[0];
        pMaster->Parse.SendValue[1] = pMaster->Request.Queue[pMaster->Request.Tail].Value[1];
        break;
    case MBX_FUNC_WRITE_REG:
        MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Value[0]);
        MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Value[1]);
        pMaster->Parse.SendValue[0] = pMaster->Request.Queue[pMaster->Request.Tail].Value[0];
        pMaster->Parse.SendValue[1] = pMaster->Request.Queue[pMaster->Request.Tail].Value[1];
        break;
    case MBX_FUNC_WRITE_COIL_MUL:
        MBxTxBufferPutReg(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].RegNum);
        pMaster->Parse.SendRegNum = pMaster->Request.Queue[pMaster->Request.Tail].RegNum;
        MBxTxBufferPutReg(pMaster, ((pMaster->Request.Queue[pMaster->Request.Tail].RegNum >> 3) + (pMaster->Request.Queue[pMaster->Request.Tail].RegNum % 8 ? 1 : 0)));
        for(crc.Val = 0; crc.Val < ((pMaster->Request.Queue[pMaster->Request.Tail].RegNum >> 3) + (pMaster->Request.Queue[pMaster->Request.Tail].RegNum % 8 ? 1 : 0)); crc.Val++) // 借助crc变量遍历，节省一个临时变量
        {
            MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Value[crc.Val]);
            pMaster->Parse.SendValue[crc.Val] = pMaster->Request.Queue[pMaster->Request.Tail].Value[crc.Val];
        }
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        MBxTxBufferPutReg(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].RegNum);
        pMaster->Parse.SendRegNum = pMaster->Request.Queue[pMaster->Request.Tail].RegNum;
        MBxTxBufferPutc(pMaster, (pMaster->Request.Queue[pMaster->Request.Tail].RegNum << 1));
        for(crc.Val = 0; crc.Val < (pMaster->Request.Queue[pMaster->Request.Tail].RegNum << 1); crc.Val++) // 借助crc变量遍历，节省一个临时变量
        {
            MBxTxBufferPutc(pMaster, pMaster->Request.Queue[pMaster->Request.Tail].Value[crc.Val]);
            pMaster->Parse.SendValue[crc.Val] = pMaster->Request.Queue[pMaster->Request.Tail].Value[crc.Val];
        }
        break;
    default:
        break;
    }

    /* 计算CRC填充 */
    crc.Val = MBx_utility_crc16((uint8_t *)(pMaster->TxExist.Buffer), pMaster->TxExist.Len); // 计算CRC校验码
    MBxTxBufferPutc(pMaster, crc.H_L.L8);                                                    // CRC低8位
    MBxTxBufferPutc(pMaster, crc.H_L.H8);                                                    // CRC高8位

    pMaster->Request.Tail = (pMaster->Request.Tail + 1) % MBX_MASTER_REQUEST_QUEUE_MAX;
}

/**
 * @brief MBX主站请求队列中添加一个请求
 * @param pMaster  MBX对象指针
 * @param SlaveID 请求从机ID
 * @param Func 请求功能码
 * @param AddrStart 请求地址起始
 * @param RegNum 请求寄存器数量
 * @param Value 请求数据指针头
 * @param ValueLen 请求数据长度
 * @return 
 */
uint32_t MBxMasterRequestAdd(_MBX_MASTER *pMaster, uint8_t SlaveID, uint8_t Func, uint16_t AddrStart, uint16_t RegNum, uint8_t *Value, uint16_t ValueLen)
{
    /* 审查队列是否已满 */
    if(MBxMasterRequestFullQ(pMaster))
        return MBX_API_RETURN_BUFFER_FULL;

    pMaster->Request.Queue[pMaster->Request.Head].SlaveID   = SlaveID;
    pMaster->Request.Queue[pMaster->Request.Head].Func      = Func;
    pMaster->Request.Queue[pMaster->Request.Head].AddrStart = AddrStart;

#if !MBX_ENDIAN_MODE_BIG /* 小端，内存非天然模拟 */
    switch(Func)
    {
    case MBX_FUNC_READ_COIL:
    case MBX_FUNC_READ_DISC_INPUT:
    case MBX_FUNC_READ_REG:
    case MBX_FUNC_READ_INPUT_REG:
        pMaster->Request.Queue[pMaster->Request.Head].RegNum = RegNum;
        break;
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
        pMaster->Request.Queue[pMaster->Request.Head].Value[1] = Value[0];
        pMaster->Request.Queue[pMaster->Request.Head].Value[0] = Value[1];
        break;
    case MBX_FUNC_WRITE_COIL_MUL:
        pMaster->Request.Queue[pMaster->Request.Head].RegNum = RegNum;
        for(int i = 0; i < ValueLen; i++)
        {
            pMaster->Request.Queue[pMaster->Request.Head].Value[i] = (i & 0x1) ? (Value[i - 1]) : (Value[i + 1]);
        }
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        pMaster->Request.Queue[pMaster->Request.Head].RegNum = RegNum;
        for(int i = 0; i < ValueLen; i++)
        {
            pMaster->Request.Queue[pMaster->Request.Head].Value[i] = (i & 0x1) ? (Value[i - 1]) : (Value[i + 1]);
        }
        break;
    default:
        break;
    }
#else
    switch(Func)
    {
    case MBX_FUNC_READ_COIL:
    case MBX_FUNC_READ_DISC_INPUT:
    case MBX_FUNC_READ_REG:
    case MBX_FUNC_READ_INPUT_REG:
        pMaster->Request.Queue[pMaster->Request.Head].RegNum = RegNum;
        break;
    case MBX_FUNC_WRITE_COIL:
    case MBX_FUNC_WRITE_REG:
        pMaster->Request.Queue[pMaster->Request.Head].Value[0] = Value[0];
        pMaster->Request.Queue[pMaster->Request.Head].Value[1] = Value[1];
        break;
    case MBX_FUNC_WRITE_COIL_MUL:
        pMaster->Request.Queue[pMaster->Request.Head].RegNum = RegNum;
        memcpy(pMaster->Request.Queue[pMaster->Request.Head].Value, Value, ValueLen);
        break;
    case MBX_FUNC_WRITE_REG_MUL:
        pMaster->Request.Queue[pMaster->Request.Head].RegNum = RegNum;
        memcpy(pMaster->Request.Queue[pMaster->Request.Head].Value, Value, ValueLen);
        break;
    default:
        break;
    }
#endif

    pMaster->Request.Head = (pMaster->Request.Head + 1) % MBX_MASTER_REQUEST_QUEUE_MAX;
    return MBX_API_RETURN_DEFAULT;
}

#endif /* MBX_MASTER_ENABLE */
