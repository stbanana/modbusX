/**
 * 一个用户配置文件的示例模板
 * 你可以复制一份并且改名为 MBx_user.h 加入你的工程
 */

#ifndef _MBX_USER_H_
#define _MBX_USER_H_

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// <h> 功能配置

// <e> 从机功能是否开启
// <o> 从机功能是否开启
//   <0=> 裁剪去掉从机功能
//   <1=> 从机启用
// <i> Default: 从机启用
#define MBX_SLAVE_ENABLE 1

// </e>

// <e> 主机功能是否开启
// <o> 主机功能是否开启
//   <0=> 裁剪去掉主机功能
//   <1=> 主机启用
// <i> Default: 主机启用
#define MBX_MASTER_ENABLE 1

// <o> TCP主机功能是否开启
//   <0=> 裁剪掉TCP主机功能
//   <1=> TCP主机功能启用
// <i> Default: 裁剪掉TCP主机功能
#define MBX_MODULE_TCP_MASTER_ENABLE 0

//   <o> 主机请求队列的最大深度 <32-255> <f.d>
//   <i> 是队列中的最大寄存条数，用于连续读写请求，寄存并自动发送处理。是8位存储故而不超过255
//   <i> Default: 40
#define MBX_MASTER_REQUEST_QUEUE_MAX 40

//   <o> 主机错误响应队列的最大深度 <1-255> <f.d>
//   <i> 是队列中的最大寄存条数，用于寄存从机的错误响应，并在主应用中取出处理。是8位存储故而不超过255
//   <i> Default: 10
#define MBX_MASTER_ERROR_QUEUE_MAX 10

//   <o> 主机读写多个寄存器的最大个数 <1-127> <f.d>
//   <i> 由于从机回复"字节数"段是一个字节，所以配置最大值为0x7F
//   <i> Default: 127
#define MBX_MASTER_MULTI_REG_MAX 127

//   <o> 主机响应超时的判定阈值[us] <10000-1000000000> <f.d>
//   <i> 若主机在设定时间内未收到从机响应，则判定为超时
//   <i> Default: 100000
#define MBX_MASTER_RESPONSE_TIMEOUT_US 100000

// </e>

// <e> 错误追踪模组是否开启
// <o> 错误追踪模组是否开启
//   <0=> 裁剪掉错误追踪模组
//   <1=> 错误追踪模组启用
// <i> Default: 错误追踪模组启用
#define MBX_MODULE_ERR_TRACE_ENABLE 1

//   <o> 错误追踪记录的条目数 <5-255> <f.d>
//   <i> 整个系统追踪栈的最大深度。是8位存储故而不超过255
//   <i> Default: 20
#define MBX_ERR_TRACE_MAX_NUM 20
// </e>

// <o> 拓展回调(需拓展定义表，造成明显消耗)
//   <0=> 关闭
//   <1=> 启用
// <i> Default: 关闭
#define MBX_EXTEN_REG_HANDLE_ENABLE 0

//   <o> 发送失败的时间判定阈值[us] <1000-5000000> <f.d>
//   <i> 若连续这段时间都无法正常发送帧，则判定为通信不良
//   <i> Default: 10000
#define MBX_SENDERR_TIMEOUT_US 10000

// </h>

// <h> 属性配置

// <o> 发送port函数是否为多字节发送
//   <0=> 单字节发送port
//   <1=> 多字节发送port
// <i> Default: 多字节发送port
#define MBX_SEND_MODE_BYTES 1

// <o> 拓展的API支持
//   <0=> 关闭
//   <1=> 启用
// <i> Default: 关闭
#define MBX_EXTEN_API_ENABLE 0

// <o> CRC使用库内工具还是外部挂载函数生成
//   <0=> 使用库内工具
//   <1=> 使用外部挂载
// <i> 若使用外部挂载则需 自行实现 uint16_t MBx_utility_crc16(uint8_t* data, uint16_t len); 用于实现CRC计算
// <i> MBx_utility_crc16.c 中有库内CRC生成算法及其传参定义
// <i> Default: 使用库内工具
#define MBX_BUILT_UTILTY_CRC_ENABLE 0

// <o> CPU的大小端
//   <0=> 小端
//   <1=> 大端
// <i> 多寄存器组合拼凑依赖此设置
// <i> 暂不支持完全依据大小端自动处理, 仍采用手动定义每个寄存器地址的属性
// <i> Default: 小端
#define MBX_ENDIAN_MODE_BIG 0

// </h>

//------------- <<< end of configuration section >>> -----------------------

#endif /* _MBX_USER_H_ */