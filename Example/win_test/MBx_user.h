/**
 * 一个用户配置文件的示例模板
 * 你可以复制一份并且改名为 MBx_user.h 加入你的工程
 */

#ifndef _MBX_USER_H_
#define _MBX_USER_H_

/**
 * 从机功能是否开启的配置项 0则为裁剪掉从机功能 >0则启用
 */
#define MBX_SLAVE_ENABLE 1

/**
 * 主机功能是否开启的配置项 0则为裁剪掉主机功能 >0则启用
 */
#define MBX_MASTER_ENABLE 1

/**
 * MBX的错误追踪模组是否启用 0则为裁剪掉错误追踪模组功能 >0则启用
 */
#define MBX_MODULE_ERR_TRACE_ENABLE 1

/**
 * MBX的发送port函数是否为多字节发送 0则需要绑定单字节发送port >0则需绑定多字节发送port
 */
#define MBX_SEND_MODE_BYTES 1

/**
 * MBX的CRC使用库内工具生成还是外部挂载 0则使用库内工具生成 >0则使用外部挂载
 * 若使用外部挂载则需 自行实现 uint16_t MBx_utility_crc16(uint8_t* data, uint16_t len); 用于实现CRC计算
 *                      MBx_utility_crc16.c 中有库内CRC生成算法及其传参定义
 */
#define MBX_BUILT_UTILTY_CRC_ENABLE 1

/**
 * MBX对于寄存器数据处理的大小端模式 0表示数据是小端存储 >0表示是大端
 *      多寄存器组合拼凑依赖此设置
 *      暂不支持完全依据大小端自动处理, 仍采用手动定义每个寄存器地址的属性
 */
#define MBX_ENDIAN_MODE_BIG 0

#endif /* _MBX_USER_H_ */