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
 * MBX对于寄存器数据处理的大小端模式 0表示数据是小端存储 >0表示是大端
 * (暂不支持依据大小端自动处理, 仍采用手动定义每个地址的属性)
 */
#define MBX_ENDIAN_MODE_BIG 0

#endif /* _MBX_USER_H_ */