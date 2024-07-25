
/**
 * @file    Win_Smain.c
 * windows使用虚拟串口与modbus测试软件进行通信
 * 测试主从机功能
 * 其中大部分代码来自百度，且懒得格式化
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <windows.h>
#include <string.h>

#include <MBx_api.h>

/* Private types -------------------------------------------------------------*/
_MBX_SLAVE slave;

/* Private Constants ---------------------------------------------------------*/
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

#define EXIT_STR  "exit"
#define I_EXIT    "I exit.\r\n"
#define I_RECEIVE "I receive.\r\n"

/* 测试使用的串口号定义 */
#define COM_PORT_NAME "COM2"
/* 测试使用的串口缓冲区大小 */
#define BUF_SIZE 2048

/* Private variables ---------------------------------------------------------*/

/* 串口句柄 */
HANDLE comHandle = INVALID_HANDLE_VALUE;

/* modbusx slave对象 */
_MBX_SLAVE MBxSlave;

/* Private function prototypes -----------------------------------------------*/

/* 用于绑定的port函数 */
uint32_t SerialSendPort(const void *Data, size_t Len);
uint32_t SerialGetcPort(uint8_t *Data);

/* 包装的win32串口驱动 */
static HANDLE  SerialOpen(const char *com, int baud, int byteSize, int parity, int stopBits);
static WINBOOL SerialClose(HANDLE hObject);

/* Private functions ---------------------------------------------------------*/

int main(int argc, char *argv[])
{
    BOOL b = FALSE;

    /* 打开串口 */
    const char *com = COM_PORT_NAME;
    comHandle       = SerialOpen(com, CBR_9600, 8, NOPARITY, ONESTOPBIT);
    if(INVALID_HANDLE_VALUE == comHandle)
    {
        LOGE("OpenSerial COM2 fail!\r\n");
        return -1;
    }
    LOGD("Open COM2 Successfully!\r\n");

    /* 初始化modbus从机 */
    MBx_Slave_RTU_Init(&MBxSlave, 1, SerialSendPort, SerialGetcPort);

    while(1)
    {
        MBx_Ticks(1000); // 换算为微妙传入
        Sleep(1);
    }

    //关闭串口
    SerialClose(comHandle);

    return 0;
}

/******************利用windows串口驱动编写的mbx port函数******************/
/**
 * @brief 将 MBX_SEND_MODE_BYTES 宏置1后，可用多字节发送port
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
    if(b)
        return MBX_PORT_RETURN_DEFAULT;
    else
        return MBX_PORT_RETURNT_ERR_INDEFINITE;
}

/**
 * @brief 数据接收port，实现功能为取单字节，返回值表示是否取接收成功
 * @param Data 字节指针，取到的字节
 * @return port标准返回
 */
uint32_t SerialGetcPort(uint8_t *Data)
{
    WINBOOL b     = FALSE; // 接收操作标识
    DWORD   wRLen = 0;     // 实际接收数据长度
    /* 尝试接收 */
    b = ReadFile(comHandle, Data, 1, &wRLen, NULL);
}

/******************以下为串口驱动，应当分离为port文件，示例就算了******************/
/**
 * @brief 打开串口
 * @param com 串口名称，如COM1，COM2
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
