
/**
 * @file    TCP_Smain.c
 * windows使用TCP server与modbus测试软件进行通信
 * 测试从机功能
 */

/* Includes ------------------------------------------------------------------*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // 包含 inet_pton 函数

#include <MBx_api.h>

// #pragma comment(lib, "Ws2_32.lib")

/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

#define IP          "127.0.0.1"
#define PORT        502
#define MAX_CLIENTS 5

/* Private variables ---------------------------------------------------------*/

WSADATA            wsaData;
SOCKET             serverSocket, clientSocket;
struct sockaddr_in serverAddr, clientAddr;

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

/* 申请从机对象发送及接收buffer */
uint8_t                          SRxBuffer[84];
uint8_t                          STxBuffer[84];

static const _MBX_MAP_LIST_ENTRY MapList[];
/* Private Constants ---------------------------------------------------------*/
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
uint32_t TCPServerSendPort(const void *Data, size_t Len);
uint32_t TCPServerGetcPort(uint8_t *Data);

/* 包装的win32 TCP server驱动 */
static void TCP_Server_Open(void);
static void TCP_Server_Close(void);

/* 分立测试系列 */
void MyTCPSlaveTest(void);

/* Private functions ---------------------------------------------------------*/

int main(int argc, char *argv[])
{
    MBX_UNUSED_PARAM(argc)
    MBX_UNUSED_PARAM(argv)
    /* 打开TCP server*/
    TCP_Server_Open( );

    /* TCP从机测试 */
    MyTCPSlaveTest( );

    /* 关闭TCP server*/
    TCP_Server_Close( );

    return 0;
}

/**
 * @brief TCP从机测试
 * @param  无
 */
void MyTCPSlaveTest(void)
{
    /* 初始化modbus从机1 */
    if(MBx_Slave_TCP_Init(&MBxSlave,         // 从机对象
                          1,                 // 从机ID
                          MapList,           // 地址映射表
                          TCPServerSendPort, // 发送函数
                          TCPServerGetcPort, // 接收函数
                          SRxBuffer,         // 库内接收buffer分配
                          84,                // 接收buffer最大长度
                          STxBuffer,         // 库内发送buffer分配
                          84)                // 发送buffer最大长度
       != MBX_API_RETURN_DEFAULT)
    {
        /* 初始化错误 自行判断返回值差错 */
    }

    /* 假装初始化从机2(如果真的有，把传参填写正常) */
    MBx_Slave_TCP_Init(&MBxSlave2,     // 从机对象
                       2,              // 从机ID
                       MapList,        // 地址映射表
                       MBX_PARA_NULL,  // 发送函数
                       MBX_PARA_NULL,  // 接收函数
                       MBX_PARA_NULL,  // 库内接收buffer分配
                       MBX_PARA_NULL,  // 接收buffer最大长度
                       MBX_PARA_NULL,  // 库内发送buffer分配
                       MBX_PARA_NULL); // 发送buffer最大长度

    /* 初始化测试数据 */
    TestMemInit( );

    int addrLen  = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
    while(1)
    {
        if(clientSocket == INVALID_SOCKET)
        {
            clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
            int error    = WSAGetLastError( );
            if(error == WSAEWOULDBLOCK)
            {
                // 没有连接请求，继续循环
                Sleep(10); // 等待一小段时间再检查
            }
            continue;
        }
        else
        {
            MBx_Ticks(15000);  // 换算为微秒传入MBx驱动 链表自动驱动
            TestMemUpdate(15); // 毫秒传入测试函数
        }
        Sleep(1); // 实际延迟大约是15ms
    }
}

/******************寄存器地址映射表示例******************/

/* 为了实现更快速的查找，库内采用二分法查询地址表
    地址表必须手动以升序排列，由于C11标准不支持动态宏，暂时无法在编译阶段自动检查 */
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
    {.Addr = 0x0300, .Memory = &fMapMem[0],   .Type = MBX_REG_TYPE_U32_H, .Handle = fWriteTest1  }, /* 浮点映射测试 (模拟大端)*/
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

/******************利用TCP服务器驱动编写的mbx port函数******************/
/**
 * @brief 将 MBX_SEND_MODE_BYTES 宏置1后, 可用多字节发送port
 * @param Data 发送buffer指针
 * @param Len 期望发送的长度
 * @return port标准返回
 */
uint32_t TCPServerSendPort(const void *Data, size_t Len)
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
uint32_t TCPServerGetcPort(uint8_t *Data)
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
static void TCP_Server_Open(void)
{
    // 初始化 Winsock
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStaTCPp failed. Error Code: %d\n", WSAGetLastError( ));
        return;
    }

    // 创建 socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == INVALID_SOCKET)
    {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError( ));
        WSACleanup( );
        return;
    }

    // 设置服务器地址
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // 监听所有可用的接口
    serverAddr.sin_port        = htons(PORT); // 转换为网络字节序

    // 指定要绑定的 IP 地址
    const char *ipAddress = IP;
    if(inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported: %s\n", ipAddress);
        closesocket(serverSocket);
        WSACleanup( );
        return;
    }

    // 在创建 socket 之后设置选项以允许地址重用
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    // 绑定 socket
    if(bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError( ));
        closesocket(serverSocket);
        WSACleanup( );
        return;
    }

    // 开始监听
    if(listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR)
    {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError( ));
        closesocket(serverSocket);
        WSACleanup( );
        return;
    }

    printf("Server is listening on port %d...\n", PORT);

    // 接受客户端连接
    u_long mode = 1;
    ioctlsocket(serverSocket, FIONBIO, &mode); // 设置为非阻塞模式
}

static void TCP_Server_Close(void)
{
    // 关闭 sockets
    closesocket(clientSocket); // 关闭客户端套接字
    closesocket(serverSocket); // 关闭服务器套接字
    WSACleanup( );
}