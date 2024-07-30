
/**
 * @file    Win_Smain.c
 * windows使用虚拟串口与modbus测试软件进行通信
 * 测试主从机功能
 * 其中大部分代码来自百度, 且懒得格式化
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <MBx_api.h>

/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#if 1 //开启DEBUG打印
#define LOGD(...) printf(__VA_ARGS__)
#else //关闭DEBUG打印
#define LOGD(...)
#endif

#if 1 //开启ERROR打印
#define LOGE(...) printf(__VA_ARGS__)
#else //关闭ERROR打印
#define LOGE(...)
#endif

/* 测试使用的串口号定义 */
#define COM_PORT_NAME "COM2"
/* 测试使用的串口缓冲区大小 */
#define BUF_SIZE 2048

/* Private variables ---------------------------------------------------------*/

/* 串口句柄 */
HANDLE comHandle = INVALID_HANDLE_VALUE;

/* modbusx slave对象 */
_MBX_SLAVE MBxSlave;
_MBX_SLAVE MBxSlave2;

/* 供映射的内存区域 */
uint8_t  u8MapMem[64];
uint16_t u16MapMem[64];
uint32_t u36MapMem[64];
uint64_t u64MapMem[64];
float    fMapMem[64]; // 32位数据模型
double   dMapMem[64]; // 64位数据模型

/* Private Constants ---------------------------------------------------------*/
static const _MBX_MAP_LIST_ENTRY MapList[];
/* Private function prototypes -----------------------------------------------*/

/* 示例的写时处理 */
static uint32_t u8WriteTest1(void *value);
static uint32_t u8WriteTest2(void *value);
static uint32_t u16WriteTest1(void *value);
static uint32_t u16WriteTest2(void *value);
static uint32_t u32WriteTest1(void *value);
static uint32_t u32WriteTest2(void *value);
static uint32_t u32WriteTest3(void *value);
static uint32_t fWriteTest1(void *value);

/* 测试数据弄一些值的操作 */
static void TestMemInit(void);
static void TestMemUpdate(uint32_t Cycle);

/* 用于绑定的port函数 */
uint32_t SerialSendPort(const void *Data, size_t Len);
uint32_t SerialGetcPort(uint8_t *Data);

/* 包装的win32串口驱动 */
static HANDLE  SerialOpen(const char *com, int baud, int byteSize, int parity, int stopBits);
static WINBOOL SerialClose(HANDLE hObject);

/* 分立测试系列 */
void MyRTUSlaveTest(void);

/* Private functions ---------------------------------------------------------*/

int main(int argc, char *argv[])
{
    /* 打开串口 */
    const char *com = COM_PORT_NAME;
    comHandle       = SerialOpen(com, CBR_9600, 8, NOPARITY, ONESTOPBIT);
    if(INVALID_HANDLE_VALUE == comHandle)
    {
        LOGE("OpenSerial %s fail!\r\n", COM_PORT_NAME);
        return -1;
    }
    LOGD("Open %s Successfully!\r\n", COM_PORT_NAME);

    /* RTU从机测试 */
    MyRTUSlaveTest( );

    //关闭串口
    SerialClose(comHandle);

    return 0;
}

/**
 * @brief RTU从机测试
 * @param  无
 */
void MyRTUSlaveTest(void)
{
    /* 申请从机对象发送及接收buffer */
    uint8_t *SRxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));
    uint8_t *STxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));

    /* 初始化modbus从机1 */
    MBx_Slave_RTU_Init(&MBxSlave,      // 从机对象
                       1,              // 从机ID
                       MapList,        // 地址映射表
                       SerialSendPort, // 发送函数
                       SerialGetcPort, // 接收函数
                       9600,           // 波特率
                       SRxBuffer,      // 库内接收buffer分配
                       84,             // 接收buffer最大长度
                       STxBuffer,      // 库内发送buffer分配
                       84);            // 发送buffer最大长度

    /* 假装初始化从机2(如果真的有，把传参填写正常) */
    MBx_Slave_RTU_Init(&MBxSlave2, // 从机对象
                       2,          // 从机ID
                       MapList,    // 地址映射表
                       NULL,       // 发送函数
                       NULL,       // 接收函数
                       NULL,       // 波特率
                       NULL,       // 库内接收buffer分配
                       NULL,       // 接收buffer最大长度
                       NULL,       // 库内发送buffer分配
                       NULL);      // 发送buffer最大长度

    /* 初始化测试数据 */
    TestMemInit( );
    while(1)
    {
        MBx_Ticks(15000);  // 换算为微秒传入MBx驱动 链表自动驱动
        TestMemUpdate(15); // 毫秒传入测试函数
        Sleep(1);          // 实际延迟大约是15ms
    }
}
/******************寄存器地址映射表示例******************/

/* 为了实现更快速的查找，库内采用二分法查询地址表
    地址表必须手动以升序排列，由于C标准不支持动态宏，暂时无法在编译阶段自动检查 */
static const _MBX_MAP_LIST_ENTRY MapList[] = {
    /*  寄存器地址        映射到的内部内存              内部内存数据属性            写时回调(NULL为只读寄存器)  */
    {.Addr = 0x0000, .Memory = &u8MapMem[0],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest1 },
    {.Addr = 0x0001, .Memory = &u8MapMem[1],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest2 },
    {.Addr = 0x0002, .Memory = &u8MapMem[2],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0003, .Memory = &u8MapMem[3],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0100, .Memory = &u16MapMem[0], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest1},
    {.Addr = 0x0101, .Memory = &u16MapMem[1], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest2},
    {.Addr = 0x0200, .Memory = &u36MapMem[0], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest1}, /* 多寄存器组合映射同一个内存变量，写入处理函数应该是同一个(硬性要求) 模拟大端内存(ABCD排列 基于传输协议，这是最合适的) */
    {.Addr = 0x0201, .Memory = &u36MapMem[0], .Type = MBX_REG_TYPE_U32_L, .Handle = u32WriteTest1},
    {.Addr = 0x0202, .Memory = &u36MapMem[1], .Type = MBX_REG_TYPE_U32_L, .Handle = u32WriteTest2}, /* 多寄存器拼接模仿小端16位字节交换映射 CDAB排列 因为传输协议要求每个寄存器高8位在前，不允许完全模拟小端(完全模拟将导致单寄存器操作混乱)*/
    {.Addr = 0x0203, .Memory = &u36MapMem[1], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest2},
    {.Addr = 0x0204, .Memory = &u36MapMem[0], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest1}, /* 多寄存器乱序插叙 瞎几把自由映射*/
    {.Addr = 0x0205, .Memory = &u36MapMem[2], .Type = MBX_REG_TYPE_U32_L, .Handle = u32WriteTest3},
    {.Addr = 0x0206, .Memory = &u36MapMem[0], .Type = MBX_REG_TYPE_U32_L, .Handle = u32WriteTest1},
    {.Addr = 0x0207, .Memory = &u36MapMem[2], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest3},
    {.Addr = 0x0300, .Memory = &fMapMem[0],   .Type = MBX_REG_TYPE_U32_H, .Handle = fWriteTest1  }, /* 浮点映射测试 (大端)*/
    {.Addr = 0x0301, .Memory = &fMapMem[0],   .Type = MBX_REG_TYPE_U32_L, .Handle = fWriteTest1  },

    MBX_MAP_LIST_END
};

/******************供绑定的写时处理函数示例******************/
/**
 * @brief uint8_t 映射的写时处理，展示了如何检查数据上下限
 * @param value 库内传参，对于uint8_t的映射将传入(uint8_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u8WriteTest1(void *value)
{
    uint8_t ValueGet = (*(uint8_t *)value);
    // if(ValueGet < 10) // 假设下限是 10
    // {
    //     return MBX_API_RETURN_DATA_BELOW_LIMIT;
    // }
    // if(ValueGet > 50) // 假设上限是 50
    // {
    //     return MBX_API_RETURN_DATA_ABOVE_LIMIT;
    // }

    u8MapMem[0] = ValueGet;
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint8_t 映射的写时处理，展示了如何无条件写值
 * @param value 库内传参，对于uint8_t的映射将传入(uint8_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u8WriteTest2(void *value)
{
    u8MapMem[1] = (*(uint8_t *)value);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint16_t 映射的写时处理
 * @param value 库内传参，对于uint16_t的映射将传入(uint16_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u16WriteTest1(void *value)
{
    u16MapMem[0] = (*(uint16_t *)value);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint16_t 映射的写时处理，展示了如何无条件写值
 * @param value 库内传参，对于 uint16_t 的映射将传入(uint16_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u16WriteTest2(void *value)
{
    u16MapMem[1] = (*(uint16_t *)value);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint32_t 映射的写时处理，展示了如何无条件写值
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u32WriteTest1(void *value)
{
    /* 可添加上下限检查及其他处理 */
    u36MapMem[0] = (*(uint32_t *)value);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint32_t 映射的写时处理，展示了如何无条件写值
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u32WriteTest2(void *value)
{
    /* 可添加上下限检查及其他处理 */
    u36MapMem[1] = (*(uint32_t *)value);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint32_t 映射的写时处理，展示了如何无条件写值
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u32WriteTest3(void *value)
{
    /* 可添加上下限检查及其他处理 */
    u36MapMem[2] = (*(uint32_t *)value);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief float 映射的写时处理，展示了如何无条件写值
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t fWriteTest1(void *value)
{
    /* 可添加上下限检查及其他处理 */
    fMapMem[0] = (*(float *)value);
    return MBX_API_RETURN_DEFAULT;
}

/******************测试数据弄一些值的操作******************/

/**
 * @brief 初始化测试bufferr为一些值
 * @param  
 */
static void TestMemInit(void)
{
    uint64_t i;
    for(i = 0; i < sizeof(u8MapMem); i++)
    {
        u8MapMem[i]  = i;
        u16MapMem[i] = i << 8 | i;
        u36MapMem[i] = (i + 0x10) << 24 | (i + 0x10) << 16 | i << 8 | i;
        u64MapMem[i] = (i + 0x30) << 56 | (i + 0x30) << 48 | (i + 0x20) << 40 | (i + 0x20) << 32 | (i + 0x10) << 24 | (i + 0x10) << 16 | i << 8 | i;
        fMapMem[i]   = i * 10.0 + (float)i / 10.0;
        dMapMem[i]   = i * 10.0 + (float)i / 100.0;
    }
}

/**
 * @brief 测试值周期变化
 * @param  Cycle 周期 ms
 */
static void TestMemUpdate(uint32_t Cycle)
{
    static uint32_t i;
    i += Cycle;
    if(i >= 2000) // 分频到两秒
    {
        for(i = 0; i < sizeof(u8MapMem); i++)
        {
            u8MapMem[i]  = u8MapMem[i] + 1ULL;
            u16MapMem[i] = u16MapMem[i] + (1ULL << 8 | 1ULL);
            u36MapMem[i] = u36MapMem[i] + (1ULL << 24 | 1ULL << 16 | 1ULL << 8 | 1ULL);
            u64MapMem[i] = u36MapMem[i] + ((1ULL << 56) | 1ULL << 48 | 1ULL << 40 | 1ULL << 32 | 1ULL << 24 | 1ULL << 16 | 1ULL << 8 | 1ULL);
            fMapMem[i]   = fMapMem[i] + ((float)1ULL * 10.0 + (float)1ULL / 10.0);
            dMapMem[i]   = dMapMem[i] + ((float)1ULL * 10.0 + (float)1ULL / 100.0);
        }
        i = 0;
    }
}

/******************利用windows串口驱动编写的mbx port函数******************/
/**
 * @brief 将 MBX_SEND_MODE_BYTES 宏置1后, 可用多字节发送port
 * @param Data 发送buffer指针
 * @param Len 期望发送的长度
 * @return port标准返回
 */
uint32_t SerialSendPort(const void *Data, size_t Len)
{
    WINBOOL b     = FALSE; // 发送操作标识
    DWORD   wWLen = 0;     // 实际发送数据长度
    /* 尝试发送 */
    b = WriteFile(comHandle, Data, Len, &wWLen, NULL);
    if(b && wWLen == Len)
        return MBX_PORT_RETURN_DEFAULT;
    else
        return MBX_PORT_RETURNT_ERR_INDEFINITE;
}

/**
 * @brief 数据接收port, 实现功能为取单字节, 返回值表示是否取接收成功
 * @param Data 字节指针, 取到的字节
 * @return port标准返回
 */
uint32_t SerialGetcPort(uint8_t *Data)
{
    WINBOOL b     = FALSE; // 接收操作标识
    DWORD   wRLen = 0;     // 实际接收数据长度
    /* 尝试接收 */
    b = ReadFile(comHandle, Data, 1, &wRLen, NULL);
    if(b == TRUE && wRLen == 1)
    {
        return MBX_PORT_RETURN_DEFAULT;
    }
    else
    {
        return MBX_PORT_RETURNT_ERR_INDEFINITE;
    }
}

/******************以下为串口驱动, 应当分离为port文件, 示例就算了******************/
/**
 * @brief 打开串口
 * @param com 串口名称, 如COM1, COM2
 * @param baud 波特率：常用取值：CBR_9600、CBR_19200、CBR_38400、CBR_115200、CBR_230400、CBR_460800
 * @param byteSize 数位大小：可取值7、8；
 * @param parity 校验方式：可取值NOPARITY、ODDPARITY、EVENPARITY、MARKPARITY、SPACEPARITY
 * @param stopBits 停止位：ONESTOPBIT、ONE5STOPBITS、TWOSTOPBITS；
 * @return 返回生成的串口对象
 */
HANDLE SerialOpen(const char *com, int baud, int byteSize, int parity, int stopBits)
{
    DCB          dcb;
    BOOL         b = FALSE;
    COMMTIMEOUTS CommTimeouts;
    HANDLE       comHandle = INVALID_HANDLE_VALUE;

    //打开串口
    comHandle = CreateFile(com,                          //串口名称
                           GENERIC_READ | GENERIC_WRITE, //可读、可写
                           0,                            // No Sharing
                           NULL,                         // No Security
                           OPEN_EXISTING,                // Open existing port only
                           FILE_ATTRIBUTE_NORMAL,        // Non Overlapped I/O
                           NULL);                        // Null for Comm Devices

    if(INVALID_HANDLE_VALUE == comHandle)
    {
        LOGE("CreateFile fail\r\n");
        return comHandle;
    }

    // 设置读写缓存大小
    b = SetupComm(comHandle, BUF_SIZE, BUF_SIZE);
    if(!b)
    {
        LOGE("SetupComm fail\r\n");
    }

    //设定读写超时
    CommTimeouts.ReadIntervalTimeout         = MAXDWORD;                                  //读间隔超时
    CommTimeouts.ReadTotalTimeoutMultiplier  = 0;                                         //读时间系数
    CommTimeouts.ReadTotalTimeoutConstant    = 0;                                         //读时间常量
    CommTimeouts.WriteTotalTimeoutMultiplier = 1;                                         //写时间系数
    CommTimeouts.WriteTotalTimeoutConstant   = 1;                                         //写时间常量
    b                                        = SetCommTimeouts(comHandle, &CommTimeouts); //设置超时
    if(!b)
    {
        LOGE("SetCommTimeouts fail\r\n");
    }

    //设置串口状态属性
    GetCommState(comHandle, &dcb);                //获取当前
    dcb.BaudRate = baud;                          //波特率
    dcb.ByteSize = byteSize;                      //每个字节有位数
    dcb.Parity   = parity;                        //无奇偶校验位
    dcb.StopBits = stopBits;                      //一个停止位
    b            = SetCommState(comHandle, &dcb); //设置
    if(!b)
    {
        LOGE("SetCommState fail\r\n");
    }

    return comHandle;
}

/**
 * @brief 关闭串口
 * @param hObject 
 * @return 
 */
WINBOOL SerialClose(HANDLE hObject)
{
    WINBOOL b = FALSE;
    /* 尝试关闭串口 */
    b = CloseHandle(hObject);
    if(!b)
    {
        LOGE("CloseHandle fail\r\n");
    }

    LOGD("Program Exit.\r\n");
    return b;
}
