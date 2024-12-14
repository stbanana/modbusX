#include <MBx_api.h>

// static const _MBX_MAP_LIST_ENTRY MapList[];
_MBX_MASTER MBxMaster;

//
uint32_t SendPort(const void *Data, size_t Len)
{
    return MBX_PORT_RETURN_DEFAULT;
}

uint32_t GetcPort(uint8_t *Data)
{
    return MBX_PORT_RETURN_DEFAULT;
}

int main( )
{
    /* 初始化modbus主机1 */
    if(MBx_Master_RTU_Init(&MBxMaster, // 主机对象
                           SendPort,   // 发送函数
                           GetcPort,   // 接收函数
                           9600,       // 波特率
                           NULL,       // 库内接收buffer分配
                           84,         // 接收buffer最大长度
                           NULL,       // 库内发送buffer分配
                           84)         // 发送buffer最大长度
       != MBX_API_RETURN_DEFAULT)
    {
        /* 初始化错误 自行判断返回值差错 */
    }

    MBx_Ticks(1);
    return 0;
}

// static const _MBX_MAP_LIST_ENTRY MapList[] = {

//     {.Addr = 0x0001, .Memory = &(MBxSlave.Config.MapNum), .Type = MBX_REG_TYPE_U16, .Handle = NULL, .Para = &DefaultPara},

//     MBX_MAP_LIST_END
// };
