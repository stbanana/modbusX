
/**
 * @file    RTU_Mmain.c
 * windows使用 TCP client 与modbus测试软件进行通信
 * 测试主机功能
 */

/* Includes ------------------------------------------------------------------*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // 包含 inet_pton 函数

#include <MBx_api.h>

/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IP   "127.0.0.1"
#define PORT 502

/* Private variables ---------------------------------------------------------*/

SOCKET clientSocket;

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
static uint32_t fWriteTest1(void *value);

/* 示例对指令错误的处理 */
static void TestErrorConsume(void);

/* 测试数据弄一些值的操作 */
static void TestMemUpdate(uint32_t Cycle);

/* 用于绑定的port函数 */
uint32_t TCPClientSendPort(const void *Data, size_t Len);
uint32_t TCPClientGetcPort(uint8_t *Data);

/* 包装的win32 TCP Client驱动 */
static void TCP_Client_Open(void);
static void TCP_Client_Close(void);

/* 分立测试系列 */
void MyRTUMasterTest(void);

/* Private functions ---------------------------------------------------------*/

int main(int argc, char *argv[])
{
    MBX_UNUSED_PARAM(argc)
    MBX_UNUSED_PARAM(argv)
    /* 打开TCP Client*/
    TCP_Client_Open( );

    /* RTU主机测试 */
    MyRTUMasterTest( );

    /* 关闭TCP Client*/
    TCP_Client_Close( );

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
    if(MBx_Master_TCP_Init(&MBxMaster,        // 主机对象
                           TCPClientSendPort, // 发送函数
                           TCPClientGetcPort, // 接收函数
                           SRxBuffer,         // 库内接收buffer分配
                           84,                // 接收buffer最大长度
                           STxBuffer,         // 库内发送buffer分配
                           84)                // 发送buffer最大长度
       != MBX_API_RETURN_DEFAULT)
    {
        /* 初始化错误 自行判断返回值差错 */
    }

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
    MBx_Master_TCP_Init(&MBxMaster2,    // 主机对象
                        MBX_PARA_NULL,  // 发送函数
                        MBX_PARA_NULL,  // 接收函数
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
    static uint16_t u16buffer[10]; // 测试数组
    static uint16_t u16test   = 0;
    static uint32_t u32test   = 0;
    static float    ftest     = 0.0;
    static uint32_t ftest_cpy = 0;
    static uint32_t i;
    i += Cycle;
    if((i % 500) == 0) // 分频到500ms
    {
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0, 4);           // 请求读取1号从机的0地址的4个寄存器
        MBx_Master_Read_Input_Reg_Request(&MBxMaster, 1, 0x100, 2); // 请求读取1号从机的0x100地址的2个寄存器 (作为输入寄存器只读)
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0x200, 2);       // 请求读取1号从机的0x200地址的2个寄存器 拼凑为32位数据
        MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0x300, 2);       // 请求读取1号从机的0x300地址的2个寄存器 拼凑为32位浮点
    }
    if(i >= 2000) // 分频到两秒
    {
        u16test++;
        u32test++;
        ftest += 1.1;
        u16buffer[0] = u16test;
        u16buffer[1] = u16test + 1;
        u16buffer[2] = u16test + 2;
        u16buffer[3] = u16test + 3;
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0, 4, (uint8_t *)&u16buffer[0], 8); // 请求写入1号从机的0地址的4个寄存器，写成功时自动同步进映射内存区
        u16buffer[0] = u16test;
        u16buffer[1] = u16test;
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0x100, 2, (uint8_t *)&u16buffer[0], 4); // 请求写入1号从机的0x100地址的2个寄存器 由于定义为输入寄存器，应当会产生失败写入
        u16buffer[0] = (u32test >> 16) & 0xFFFF;
        u16buffer[1] = u32test & 0xFFFF;
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0x200, 2, (uint8_t *)&u16buffer[0], 4); // 请求写入1号从机的0x200地址的2个寄存器 无符号32位拼凑
        /* 取真实内存值的原版写法，但会带来警告 */
        // u16buffer[0] = (*(uint32_t *)(&ftest) >> 16) & 0xFFFF;
        // u16buffer[1] = (*(uint32_t *)(&ftest)) & 0xFFFF;
        /* 增加开销的写法，但能消除警告 */
        memcpy(&ftest_cpy, &ftest, sizeof(float));
        u16buffer[0] = (ftest_cpy >> 16) & 0xFFFF;
        u16buffer[1] = ftest_cpy & 0xFFFF;
        MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0x300, 2, (uint8_t *)&u16buffer[0], 4); // 请求写入1号从机的0x300地址的2个寄存器 浮点拼凑
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
    {.Addr = 0x0300, .Memory = &fMapMem[10],   .Type = MBX_REG_TYPE_U32_H, .Handle = fWriteTest1  }, /* 浮点映射测试 模拟大端内存(ABCD排列 基于传输协议，这是最合适的)*/
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
    MBX_UNUSED_PARAM(value)
    // (*(uint32_t *)value);
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
    MBX_UNUSED_PARAM(value)
    // (*(float *)value);
    /* 期望写入ValueGet而没有成功写入 */
    return MBX_API_RETURN_DEFAULT;
}

/******************利用TCP服务器驱动编写的mbx port函数******************/
/**
 * @brief 将 MBX_SEND_MODE_BYTES 宏置1后, 可用多字节发送port
 * @param Data 发送buffer指针
 * @param Len 期望发送的长度
 * @return port标准返回
 */
uint32_t TCPClientSendPort(const void *Data, size_t Len)
{
    int wWLen = 0; // 实际发送数据长度
    /* 尝试发送 */
    wWLen = send(clientSocket, Data, Len, 0);
    if(wWLen == SOCKET_ERROR || // 失败
       wWLen != (int)Len)
    {
        return MBX_PORT_RETURNT_ERR_INDEFINITE;
    }
    else
    {
        return MBX_PORT_RETURN_DEFAULT;
    }
}

/**
 * @brief 数据接收port, 实现功能为取单字节, 返回值表示是否取接收成功
 * @param Data 字节指针, 取到的字节
 * @return port标准返回
 */
uint32_t TCPClientGetcPort(uint8_t *Data)
{
    /* 尝试接收 */
    int bytesReceived = recv(clientSocket, Data, 1, 0);
    if(bytesReceived == 1)
    {
        return MBX_PORT_RETURN_DEFAULT;
    }
    else
    {
        return MBX_PORT_RETURNT_ERR_INDEFINITE;
    }
}

/******************以下为TCP服务器驱动, 应当分离为port文件, 示例就算了******************/
static void TCP_Client_Open(void)
{
    WSADATA wsaData;

    // 初始化 Winsock
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError( ));
        return;
    }

    // 创建 socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == INVALID_SOCKET)
    {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError( ));
        WSACleanup( );
        return;
    }

    // 设置服务器地址
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(PORT);
    inet_pton(AF_INET, IP, &serverAddr.sin_addr);

    // 连接到服务器
    while(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Connection failed. Error Code: %d. Retrying...\n", WSAGetLastError( ));
        Sleep(1000); // 等待1秒后重试连接
    }

    // 设置套接字为非阻塞模式
    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode); // 设置为非阻塞模式

    printf("Connected to server %s:%d\n", IP, PORT);
}

static void TCP_Client_Close(void)
{
    // 关闭 sockets
    closesocket(clientSocket); // 关闭客户端套接字
    WSACleanup( );
}