
/**
 * @file    Win_Smain.c
 * windows使用虚拟串口与modbus测试软件进行通信
 * 测试主机功能
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

/* modbusx Master对象 */
_MBX_MASTER MBxMaster;
_MBX_MASTER MBxMaster2;

/* modbusx 从机成员对象 */
_MBX_MASTER_TEAM_MEMBER MBxMasterMember1;
_MBX_MASTER_TEAM_MEMBER MBxMasterMember2;

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

/* 示例的写失败时处理 */
static uint32_t u8WriteTest1(void *value);
static uint32_t u8WriteTest2(void *value);
static uint32_t u16WriteTest1(void *value);
static uint32_t u16WriteTest2(void *value);
static uint32_t u32WriteTest1(void *value);
static uint32_t u32WriteTest2(void *value);
static uint32_t u32WriteTest3(void *value);
static uint32_t fWriteTest1(void *value);

/* 示例对指令错误的处理 */
static void TestErrorConsume(void);

/* 测试数据弄一些值的操作 */
static void TestMemUpdate(uint32_t Cycle);

/* 用于绑定的port函数 */
uint32_t SerialSendPort(const void *Data, size_t Len);
uint32_t SerialGetcPort(uint8_t *Data);

/* 包装的win32串口驱动 */
static HANDLE  SerialOpen(const char *com, int baud, int byteSize, int parity, int stopBits);
static WINBOOL SerialClose(HANDLE hObject);

/* 分立测试系列 */
void MyRTUMasterTest(void);

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

    /* RTU主机测试 */
    MyRTUMasterTest( );

    //关闭串口
    SerialClose(comHandle);

    return 0;
}

/**
 * @brief RTU主机测试
 * @param  无
 */
void MyRTUMasterTest(void)
{
    /* 申请主机对象发送及接收buffer */
    uint8_t *SRxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));
    uint8_t *STxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));

    /* 初始化modbus主机1 */
    MBx_Master_RTU_Init(&MBxMaster,     // 主机对象
                        SerialSendPort, // 发送函数
                        SerialGetcPort, // 接收函数
                        9600,           // 波特率
                        SRxBuffer,      // 库内接收buffer分配
                        84,             // 接收buffer最大长度
                        STxBuffer,      // 库内发送buffer分配
                        84);            // 发送buffer最大长度

    /* 添加主机1管理的从机1 */
    if(MBx_Master_Member_Add(&MBxMaster,        // 主机对象
                             &MBxMasterMember1, // 从机成员对象
                             1,                 // 从机ID
                             MapList)           // 该从机对象的映射表
       != MBX_API_RETURN_DEFAULT)
    {
        /* 表明映射表或ID等传参异常 */
    }

    /* 添加主机1管理的从机2(如果真的有，把传参填写正常) */
    if(MBx_Master_Member_Add(&MBxMaster,        // 主机对象
                             &MBxMasterMember2, // 从机成员对象
                             MBX_PARA_NULL,     // 从机ID
                             MBX_PARA_NULL)     // 该从机对象的映射表
       != MBX_API_RETURN_DEFAULT)
    {
        /* 表明映射表或ID等传参异常 */
    }

    /* 假装初始化主机2(如果真的有，把传参填写正常) */
    MBx_Master_RTU_Init(&MBxMaster2,    // 主机对象
                        MBX_PARA_NULL,  // 发送函数
                        MBX_PARA_NULL,  // 接收函数
                        MBX_PARA_NULL,  // 波特率
                        MBX_PARA_NULL,  // 库内接收buffer分配
                        MBX_PARA_NULL,  // 接收buffer最大长度
                        MBX_PARA_NULL,  // 库内发送buffer分配
                        MBX_PARA_NULL); // 发送buffer最大长度

    while(1)
    {
        MBx_Ticks(15000);    // 换算为微秒传入MBx驱动 链表自动驱动
        TestMemUpdate(15);   // 毫秒传入测试函数
        TestErrorConsume( ); // 处理对从指令产生的错误
        Sleep(1);            // 实际延迟大约是15ms
    }
}
/******************测试数据弄一些值的操作******************/
/**
 * @brief 测试值周期变化
 * @param  Cycle 周期 ms
 */
static void TestMemUpdate(uint32_t Cycle)
{
    static uint8_t  u8buffer[10];  // 测试数组
    static uint16_t u16buffer[10]; // 测试数组
    static uint8_t  u8test  = 0;
    static uint16_t u16test = 0;
    static uint32_t u32test = 0;
    static float    ftest   = 0.0;
    static uint32_t i;
    i += Cycle;
    if((i % 500) == 0) // 分频到500ms
    {
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0, 4);     // 请求读取1号从机的0地址的4个寄存器
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0x100, 2); // 请求读取1号从机的0x100地址的2个寄存器
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0x200, 2); // 请求读取1号从机的0x200地址的2个寄存器 拼凑为32位数据
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0x300, 2); // 请求读取1号从机的0x300地址的2个寄存器 拼凑为32位浮点
    }
    if(i >= 2000) // 分频到两秒
    {
        u8test++;
        u16test++;
        u32test++;
        ftest += 1.1;
        memset(&u8buffer[0], u8test, 8);
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0, 4, u8buffer, 8); // 请求写入1号从机的0地址的4个寄存器
        u16buffer[0] = u16test;
        u16buffer[1] = u16test;
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0x100, 2, (uint8_t *)&u16buffer[0], 4); // 请求写入1号从机的0x100地址的2个寄存器 由于定义为输入寄存器，应当会产生失败写入
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0x200, 2, (uint8_t *)&u32test, 4);      // 请求写入1号从机的0x200地址的2个寄存器 无符号32位拼凑
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0x300, 2, (uint8_t *)&ftest, 4);        // 请求写入1号从机的0x300地址的2个寄存器 浮点拼凑
        i = 0;
    }
}

/******************应当自行编写的对请求错误的处理******************/
/**
 * @brief 示例对指令请求错误的处理
 * @param  无
 */
static void TestErrorConsume(void)
{
    uint8_t  FuncCode;
    uint8_t  ErrorCode;
    uint16_t AddrStart;
    uint16_t RegNum;
    while(MBx_Master_Error_Get(&MBxMaster, &FuncCode, &ErrorCode, &AddrStart, &RegNum) == MBX_API_RETURN_DEFAULT)
    {
        switch(FuncCode)
        {
        case MBX_FUNC_READ_COIL:
            printf("在读取线圈时发生错误, 错误码为0x%02X, 起始地址为0x%04X, 寄存器数量为0x%04X\n", ErrorCode, AddrStart, RegNum);
            break;
        case MBX_FUNC_READ_DISC_INPUT:
            printf("在读取离散输入时发生错误, 错误码为0x%02X, 起始地址为0x%04X, 寄存器数量为0x%04X\n", ErrorCode, AddrStart, RegNum);
            break;
        case MBX_FUNC_READ_REG:
            printf("在读取保持寄存器时发生错误, 错误码为0x%02X, 起始地址为0x%04X, 寄存器数量为0x%04X\n", ErrorCode, AddrStart, RegNum);
            break;
        case MBX_FUNC_READ_INPUT_REG:
            printf("在读取输入寄存器时发生错误, 错误码为0x%02X, 起始地址为0x%04X, 寄存器数量为0x%04X\n", ErrorCode, AddrStart, RegNum);
            break;
        case MBX_FUNC_WRITE_COIL:
            printf("在写入单个线圈时时发生错误, 错误码为0x%02X, 地址为0x%04X\n", ErrorCode, AddrStart);
            break;
        case MBX_FUNC_WRITE_REG:
            printf("在写入单个保持寄存器时时发生错误, 错误码为0x%02X, 地址为0x%04X\n", ErrorCode, AddrStart);
            break;
        case MBX_FUNC_WRITE_COIL_MUL:
            printf("在写入多个线圈时发生错误, 错误码为0x%02X, 起始地址为0x%04X, 寄存器数量为0x%04X\n", ErrorCode, AddrStart, RegNum);
            break;
        case MBX_FUNC_WRITE_REG_MUL:
            printf("在写入多个保持寄存器时发生错误, 错误码为0x%02X, 起始地址为0x%04X, 寄存器数量为0x%04X\n", ErrorCode, AddrStart, RegNum);
            break;
        default:
            break;
        }
    }
}

/******************寄存器地址映射表示例******************/

/* 为了实现更快速的查找，库内采用二分法查询地址表
    地址表必须手动以升序排列，由于C11标准不支持动态宏，暂时无法在编译阶段自动检查 */
static const _MBX_MAP_LIST_ENTRY MapList[] = {
    /*  寄存器地址        映射到的内部内存              内部内存数据属性            写时异常立即回调(NULL为忽略写异常)  */
    {.Addr = 0x0000, .Memory = &u8MapMem[10],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest1 },
    {.Addr = 0x0001, .Memory = &u8MapMem[11],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest2 },
    {.Addr = 0x0002, .Memory = &u8MapMem[12],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0003, .Memory = &u8MapMem[13],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0100, .Memory = &u16MapMem[10], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest1},
    {.Addr = 0x0101, .Memory = &u16MapMem[11], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest2},
    {.Addr = 0x0200, .Memory = &u36MapMem[10], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest1}, /* 多寄存器组合映射同一个内存变量，写入异常回调应该是同一个(硬性要求) 模拟大端内存(ABCD排列 基于传输协议，这是最合适的) */
    {.Addr = 0x0201, .Memory = &u36MapMem[10], .Type = MBX_REG_TYPE_U32_L, .Handle = u32WriteTest1},
    {.Addr = 0x0300, .Memory = &fMapMem[10],   .Type = MBX_REG_TYPE_U32_H, .Handle = fWriteTest1  }, /* 浮点映射测试 (只能模拟大端)*/
    {.Addr = 0x0301, .Memory = &fMapMem[10],   .Type = MBX_REG_TYPE_U32_L, .Handle = fWriteTest1  },

    MBX_MAP_LIST_END
};

/******************供绑定的写失败时处理函数示例******************/
/**
 * @brief uint8_t 映射的写失败时处理
 * @param value 库内传参，对于uint8_t的映射将传入(uint8_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u8WriteTest1(void *value)
{
    uint8_t ValueGet = (*(uint8_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    printf("向0x000写入 %d 失败\n", ValueGet);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint8_t 映射的写失败时处理
 * @param value 库内传参，对于uint8_t的映射将传入(uint8_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u8WriteTest2(void *value)
{
    uint8_t ValueGet = (*(uint8_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    printf("向0x001写入 %d 失败\n", ValueGet);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint16_t 映射的写失败时处理
 * @param value 库内传参，对于uint16_t的映射将传入(uint16_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u16WriteTest1(void *value)
{
    uint16_t ValueGet = (*(uint16_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    printf("向0x100写入 %d 失败\n", ValueGet);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint16_t 映射的写失败时处理
 * @param value 库内传参，对于 uint16_t 的映射将传入(uint16_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u16WriteTest2(void *value)
{
    uint16_t ValueGet = (*(uint16_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    printf("向0x101写入 %d 失败\n", ValueGet);
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint32_t 映射的写失败时处理
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u32WriteTest1(void *value)
{
    uint32_t ValueGet = (*(uint32_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint32_t 映射的写失败时处理
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u32WriteTest2(void *value)
{
    uint32_t ValueGet = (*(uint32_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief uint32_t 映射的写失败时处理
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t u32WriteTest3(void *value)
{
    uint32_t ValueGet = (*(uint32_t *)value);
    /* 期望写入ValueGet而没有成功写入 */
    return MBX_API_RETURN_DEFAULT;
}

/**
 * @brief float 映射的写失败时处理
 * @param value 库内传参，对于 uint32_t 的映射将传入(uint32_t *)类型
 * @return 标准返回，请依照 MBx_api.h 的 “API返回集” 部分编写
*/
static uint32_t fWriteTest1(void *value)
{
    float ValueGet = (*(float *)value);
    /* 期望写入ValueGet而没有成功写入 */
    return MBX_API_RETURN_DEFAULT;
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
