# Modbus 库横向对比

> 本文基于截至 2026-08 的各库状态撰写，力求客观，不偏袒 modbusX。
> 如有事实错误请提交 issue 或 PR。

---

## 第一部分：嵌入式 Modbus 库对比

参与比较的四个库：

| 库 | 语言 | 许可证 | 定位 |
|---|---|---|---|
| **FreeModbus** (v1.6) | C | BSD | 裸机/RTOS 嵌入式 |
| **modbusX** | C11 | MIT | 裸机/RTOS 嵌入式 |
| **ArduinoModbus** (官方) | C++ | LGPL-2.1 | Arduino 生态 |
| **libmodbus** (v3.1.10) | C | LGPL-2.1+ | OS 系统 + 可适配嵌入式 |

> **关于 libmodbus 的说明**：主线 libmodbus 实际是一个 **OS 级库**，依赖 `malloc`、`select/poll`、文件描述符、互斥锁。它在裸机上无法直接运行，需要 OS 或显著的移植工作。GitHub 上有独立的嵌入式分支（如 OpenModbus/libModbus）专为裸机设计，但并非主线。下文的 libmodbus 评估**同时覆盖主线（OS 使用）和嵌入式分支（裸机使用）**，并在必要时区分说明。

---

### 1. 额外外设需求

指库正常运行**必须占用**的 MCU 硬件资源。

| 资源 | FreeModbus | modbusX | ArduinoModbus | libmodbus (嵌入式) |
|---|---|---|---|---|
| UART | ✅ 必须，接管 RX/TX 中断 | ✅ 必须（但不接管中断，用户提供 Getc） | ✅ 必须，通过 Arduino `Serial` 对象 | ✅ 必须 |
| 硬件定时器 | ✅ 必须（T3.5 计时） | ❌ 不需要 | ❌ 不需要（用 `millis()` 软件计时） | ✅ 必须（T3.5 回调） |
| DMA | 可选 | 可选（用户自行在 Send/Getc 中实现） | 不可用 | 可选 |
| GPIO（RS-485 方向控制） | 用户自行处理 | 用户自行处理 | 用户自行处理 | 用户自行处理 |

**分析**：

- **FreeModbus 最"重"**：它不仅要 UART，还要独占 UART 的 RX/TX 中断和一个硬件定时器中断。在引脚紧张或中断向量有限的 MCU（如 ATtiny、STM32F0 低端型号）上，这可能是决定性的障碍。
- **modbusX 和 ArduinoModbus 最"轻"**：不绑架任何中断。modbusX 完全不碰硬件——连串口都不直接操作，只通过 Send/Getc 函数指针收发数据。
- ArduinoModbus 通过 `Serial` 对象间接使用 UART，但 `Serial` 是 Arduino Core 管理的，不算库额外占用。
- **libmodbus（嵌入式分支）**需要定时器用于 T3.5，与 FreeModbus 类似。

---

### 2. 对原有系统的侵入性

指集成该库后，对现有代码架构的破坏程度。

| 维度 | FreeModbus | modbusX | ArduinoModbus | libmodbus (嵌入式) |
|---|---|---|---|---|
| 中断向量占用 | 高（RX/TX/定时器） | **无** | 无（Arduino Core 管理） | 高（定时器） |
| 主循环控制 | 需周期性调用 `eMBPoll()` | 需周期性调用 `MBx_Ticks()` | 函数调用即阻塞，**主循环被冻结** | 需周期性调用 `modbus_slave_poll()` |
| 线程/任务模型 | 单任务，事件驱动 | 单 Tick，链表遍历 | 单线程，同步阻塞 | 单任务 |
| 与现有代码冲突风险 | 中高（中断抢占、优先级） | **低**（仅占用一个函数调用周期） | 低（但阻塞期间影响所有其他逻辑） | 中 |
| 全局状态污染 | 严重（静态全局变量，单实例） | 无（对象独立，栈上或堆上分配） | 中等（类成员变量） | libmodbus 主线使用 `modbus_t*` 上下文；嵌入式分支多用静态变量 |

**分析**：

- **ArduinoModbus 的问题不在"入侵"，在于"冻结"**。`readHoldingRegister()` 内部用带超时的 while 循环等待 `Serial.readBytes()` 返回——期间你的按键检测、LED 刷新、看门狗喂狗全部停摆。在 Arduino 论坛上，这是 Modbus 相关问题的头号投诉来源。
- **FreeModbus 的问题在于全局单例**。整个系统只能有一个 Master 或一个 Slave。`eMBInit()` 再次调用就会覆盖之前的配置。如果你需要"COM1 做主站、COM2 做从站"——做不到，除非深度改造源码。
- **modbusX 的代价是 Tick 周期**。你不绑架中断，但你的响应延迟至少是一个 Tick 周期。如果 Tick 是 1ms，那么最短响应延迟就是 ~1ms + 帧处理时间。FreeModbus 的中断驱动模型可以在收到最后一个字节后立即处理，延迟是微秒级。这是"无中断"换来的必然代价。
- **libmodbus（嵌入式分支）** 使用回调 + poll 模式，与 FreeModbus 类似但侵入性略低，因为它不强制接管中断实现。

---

### 3. 维护难度

指理解、修改、调试该库代码所需的心智负担。

| 维度 | FreeModbus | modbusX | ArduinoModbus | libmodbus |
|---|---|---|---|---|
| 代码规模 | ~3000 行核心 | ~3500 行核心 | ~2000 行（不含 libmodbus 依赖） | ~8000 行（主线，含所有后端） |
| 代码年代感 | 2000s 风格，命名混乱（`eMBInit`、`peMBFrameSendCur`） | 2024 风格，命名规范 | Arduino 风格，易懂 | 规范的 C 风格，一致性高 |
| 状态机可追踪性 | 隐式——分散在事件和回调中，需全局理解 | 显式——每个状态一个文件（`_IDLE`/`_WRITE`/`_READ`） | 无显式状态机（封装在同步调用里） | 主线无显式状态机（封装在阻塞调用里）；嵌入式分支有 FSM |
| 添加新功能码 | 需理解事件流 + 注册回调 + 修改 xFuncHandlers 表 | 需理解 engine 状态流转 + 在 handle 文件中添加 case | 需添加新方法 + 理解底层帧构造 | 需修改后端抽象接口 |
| 添加新寄存器 | 修改用户回调函数的 if-else 逻辑 | **加一行映射表** | 修改用户代码中的数组或变量绑定 | 修改用户回调逻辑 |
| 调试帧错误 | 需要理解事件队列和中断状态 | 可在每个状态入口打印，链路清晰 | 需要在阻塞调用处断点 | 主线可用 libmodbus 内置 debug 模式；嵌入式分支需手动跟踪 |
| 文档质量 | 差（英文文档稀少，主要靠源码和 CSDN 博客） | 中文文档完善，英文仅机翻 | 官方文档（Arduino 风格，简洁） | **优秀**——man page 完善，社区活跃 |

**分析**：

- **FreeModbus 的维护门槛最高**。事件驱动 + 中断 + 全局状态，调试一帧去哪了就像在追一个穿过四层回调的幽灵。但它的优势是——一旦移植完成，就用那几个 API，不太需要再碰内部代码。
- **modbusX 的维护门槛较低**。状态机按文件拆分（`MBx_Master_Engine_IDLE.c`、`MBx_Master_Engine_WRITE.c` 等），每个文件只做一件事。映射表是声明式的——改寄存器配置就是改一个数组，不需要碰逻辑代码。
- **ArduinoModbus 最简单但也最难改**。读代码容易，但它的同步阻塞模型意味着你想做任何"高级"操作（如同时读写多个从机）都需要完全不同的架构。
- **libmodbus 文档最好**，社区最活跃。主线代码质量高，但修改内部需要理解后端抽象。

---

### 4. 多协议配合

指同时运行多种协议（RTU/TCP）和多个实例（多主/多从）的能力。

| 能力 | FreeModbus | modbusX | ArduinoModbus | libmodbus |
|---|---|---|---|---|
| RTU + TCP 同时运行 | ❌ 编译期互斥 | ✅ 链表对象，可混跑 | ❌ 编译期选择 | ✅ 创建多个 `modbus_t*` 上下文 |
| 多主站 | ❌ 全局单例 | ✅ 链表遍历 | ❌ | ✅ 多个 `modbus_t*` |
| 多从站 | ❌ 全局单例 | ✅ 链表遍历 | ❌ | ✅ 多个 `modbus_t*` |
| 一主一从同设备 | ❌ | ✅ | ❌ | ✅ |
| ASCII 支持 | ✅ | ❌（有意不支持） | ❌ | ✅（主线） |

**分析**：

- **modbusX 和 libmodbus 是多协议多实例的唯二选择。** 区别在于实现方式：libmodbus 依赖操作系统线程——每个 `modbus_t*` 可以在各自线程里阻塞，互不影响。modbusX 依赖链表遍历——一个 `MBx_Ticks()` 顺序驱动所有对象，无需 OS。
- **FreeModbus 和 ArduinoModbus 是单例设计**。这在 2000s 的嵌入式场景下是合理选择——一个 MCU 通常只做一件事（要么是主站要么是从站）。但在现代场景（如一个 MCU 同时做 Modbus 网关 + 本地从站）下，单例成为一种限制。
- ASCII 协议的缺失是 modbusX 明确的设计选择——README 里称其为"愚蠢的协议"。对于绝大多数应用这不是问题。

---

### 5. 轻量化

指编译后的 ROM 和运行时 RAM 占用。

**测试条件说明**：由于缺乏统一的测试基准，以下数据基于：
- ARM Cortex-M4 (STM32F4), -Os 优化
- FreeModbus：RTU 从站，功能码 03/04/06/16，无 ASCII，无 TCP
- modbusX：RTU 主+从，宏已裁剪（`REQUEST_QUEUE=4, MULTI_REG=16, ERROR_QUEUE=2`），无 TCP
- libmodbus 嵌入式分支：RTU 从站，最小配置

| 指标 | FreeModbus | modbusX | ArduinoModbus | libmodbus (嵌入式分支) |
|---|---|---|---|---|
| ROM（仅协议栈） | ~3-5 KB | ~4-7 KB | ~5-8 KB（含 Arduino Core 依赖） | ~3-6 KB |
| ROM 映射表（50 个寄存器） | 0（在回调代码中） | ~1 KB（50×20 bytes） | 0（在用户数组中） | 0（在回调代码中） |
| RAM（最小配置） | ~300-500 B | ~800-1500 B | ~1-2 KB | ~500-800 B |
| RAM 中请求队列 | 无（阻塞等待） | 可配置（4×256 = 1KB 起） | 无 | 无 |
| 堆分配 | 无（全静态） | 无（全静态，用户提供 buffer） | 有（Arduino `Serial` buffer + 库内部分动态） | 主线有（`malloc`），嵌入式分支无 |
| 可裁剪性 | 编译宏（功能码级别） | 编译宏（主/从整块裁剪 + 队列深度 + 功能码） | 低（依赖 Arduino Core） | 编译宏（后端级别） |

**分析**：

- **FreeModbus 仍然是最省资源的。** 它没有映射表的每寄存器开销，没有请求队列，没有错误队列。如果你的 MCU 只有 16KB flash / 4KB RAM，FreeModbus 几乎是唯一选择。
- **modbusX 的映射表开销是真实存在的。** 如正文分析，每条映射项约 20 bytes（ARM 32-bit），但 FreeModbus 用户回调中的 if-else 分支每条约 14-20 bytes 代码——两者在"存储地址-变量映射关系"上的开销实际相当。modbusX 的额外 RAM 主要来自**请求环形队列**（异步架构的代价）和**对象独立性**（多实例的代价）。
- **ArduinoModbus 最不适合资源受限场景。** Arduino Core 本身占用可观，且库的同步阻塞模型无法通过配置来裁减。
- **libmodbus 嵌入式分支与 FreeModbus 相当**，但主线版本不适合裸机（依赖 `malloc` / `select` / 文件描述符）。

---

### 6. 性能

指响应延迟、吞吐量、CPU 占用。

| 指标 | FreeModbus | modbusX | ArduinoModbus | libmodbus |
|---|---|---|---|---|
| 帧接收完成 → 开始处理的延迟 | **微秒级**（中断上下文） | Tick 周期级（典型 1ms） | 取决于 `Serial.readBytes()` 超时 | 取决于 `select/poll` 超时（主线）或回调（嵌入式） |
| CPU 占用模式 | 突发（中断处理）+ 空闲 | **均匀**（每 Tick 固定工作量） | 阻塞期间 100% 忙等 | 主线：阻塞睡眠（低占用）；嵌入式分支：轮询 |
| 吞吐量上限（RTU, 115200） | 接近线速 | 受 Tick 频率限制（1ms Tick → 1000 帧/秒最大） | 低（每次请求-响应之间有固定开销） | 高（主线可并发多连接） |
| 多从机时的公平性 | N/A（单从机） | **公平**（链表按序驱动） | N/A | N/A |
| 实时性（最坏延迟可预测） | 好（中断优先级固定） | **好**（Tick 执行时间稳定） | 差（阻塞在不可预知的超时上） | 好 |

**分析**：

- **FreeModbus 在单次响应延迟上最优。** 中断驱动意味着帧最后一个字节到达后，在 ISR 返回之前就可以开始处理。这是裸机中断模型的天然优势。
- **modbusX 的延迟是 Tick 粒度的。** 如果你的 Tick 是 1ms，那么最坏情况下响应要多等 1ms。对于绝大多数 Modbus 应用（温度、压力、水位、电机状态），1ms 延迟完全可接受。但对于需要微秒级同步的场景（如伺服驱动器的实时位置控制），中断模型更合适。
- **modbusX 的 CPU 占用是均匀且可预测的。** 因为每次 Tick 做的事是固定的——遍历链表、检查缓冲区、状态机流转——没有突发。这对于看门狗策略设计和功耗估算非常友好。
- **ArduinoModbus 的性能最差。** 同步阻塞 + 毫秒级 `delay()` 或忙等循环 = 大量的 CPU 周期白白浪费。

---

### 7. 可移植性

指将库移植到新平台所需的工作量。

| 维度 | FreeModbus | modbusX | ArduinoModbus | libmodbus |
|---|---|---|---|---|
| 移植所需接口 | `portserial.c` + `porttimer.c` + `portevent.c`（~6 个函数 + 中断绑定） | Send() + Getc()（**2 个函数**，无中断） | 无（依赖 Arduino Core） | 主线：POSIX/Win32 已支持，新平台需实现后端接口；嵌入式分支：与 FreeModbus 类似 |
| 移植工作量 | 中等（需理解中断模型和事件队列） | **低**（两个函数，任意实现） | N/A（不可移植到非 Arduino 平台） | 主线：低（POSIX 系统零移植）；嵌入式：中等 |
| 已证实的平台 | STM32, MSP430, AVR, PIC, ... | Windows, STM32 | Arduino 生态（AVR, SAMD, Mbed） | Linux, Windows, macOS, FreeBSD... |
| C 标准 | C89/C99 | **C11**（使用了 `__weak`；去掉关键字后可兼容 C99） | C++11 | C99 |

**分析**：

- **modbusX 的移植最简单。** 只需要两个函数——发一堆字节和收一个字节。它们可以是阻塞的、非阻塞的、中断驱动的、DMA 驱动的——库不关心。在 Windows 上是 `WriteFile`/`ReadFile`，在 STM32 上是 `HAL_UART_Transmit`/`HAL_UART_Receive`，在 Linux 上是 `write`/`read`。
- **FreeModbus 的移植是"一次性投资"**——做好一个 port 之后就可以复用，但第一次做需要理解它的中断模型。网上有大量现成的移植可以参考（尤其是 STM32），降低了实际难度。
- **ArduinoModbus 不可移植。** 它就是为 Arduino 生态设计的。
- **libmodbus 主线的移植门槛最低**——如果你有 POSIX 系统，零移植。如果你没有（裸机），那基本不可用，需要找嵌入式分支。

---

### 8. 综合评价（嵌入式部分）

| 库 | 最适合的场景 | 最不适合的场景 |
|---|---|---|
| **FreeModbus** | 极低资源 MCU，单主/单从，需最低响应延迟 | 多主多从，需快速移植，不想碰中断 |
| **modbusX** | 多主多从，快速移植，异构数据类型映射，不想绑架中断 | 极度资源受限（<16KB flash / <4KB RAM），需微秒级响应 |
| **ArduinoModbus** | Arduino 原型开发，简单读写寄存器 | 生产级产品，需要同时做其他实时任务，非 Arduino 平台 |
| **libmodbus（主线）** | Linux/Windows 嵌入式系统（如树莓派工业网关），需要多协议并发 | 裸机 MCU（无 OS） |
| **libmodbus（嵌入式分支）** | 需 libmodbus API 兼容的裸机项目 | 资源极紧张的场景 |

**核心取舍**：

```
FreeModbus:     中断驱动 → 最低延迟 ← → 最高侵入性
modbusX:        Tick 驱动 → 最易移植 ← → Tick 粒度的延迟
ArduinoModbus:  同步阻塞 → 最简单 API ← → 冻结主循环
libmodbus(主线): OS 原生 → 最强生态 ← → 需要操作系统
libmodbus(嵌入): 回调驱动 → API 兼容性好 ← → 与 FreeModbus 类似的门槛
```

**不存在"最好的"库。** 只存在与你对"中断/阻塞/Tick 三者之间愿意接受哪个代价"最匹配的库。

---

## 第二部分：嵌入式库 vs 高级语言库

以 modbusX 作为嵌入式库的代表，对比 PyModbus (Python) 和 NModbus (C#/.NET)。

### 本质差异：谁来接收字节？

```
Linux/Windows 上的 PyModbus/NModbus：

你的程序                    内核                    硬件
   |                         |                       |
   |-- read(fd, buf, 256) -->|                       |
   |   [线程睡眠]             |<- UART RX 中断 --------| 字节到达
   |                         |  存入内核缓冲区          |
   |                         |<- UART RX 中断 --------| 字节到达
   |                         |  存入内核缓冲区          |
   |                         |       ...              |
   |<-- copy_to_user() ------|  VTIME 超时或满 VMIN   |
   |   返回 N 字节            |                       |
```

```
裸机上的 modbusX：

你的程序                                              硬件
   |                                                  |
   |-- Getc(&byte) → 检查 UART->DR 标志 --------------→ DR=0，没数据
   |   返回 ERR                                         |
   |-- [Tick 周期等待]                                  |
   |-- Getc(&byte) → 检查 UART->DR 标志 --------------→ DR=1！
   |   存入 buffer[0]                                   |← 字节 0x01 到达
   |-- Getc(&byte) → 检查 UART->DR 标志 --------------→ DR=1！
   |   存入 buffer[1]                                   |← 字节 0x03 到达
   |   ...持续逐字节收，直到 T3.5 超时...
```

**PyModbus 和 NModbus 的高层抽象不是"更聪明"——是内核替它们完成了逐字节中断接收的脏活。** 把它们放到裸机 STM32 上，一行都跑不起来。

---

### 逐维对比

#### T3.5 帧定界精度

| | 嵌入式库 (modbusX/FreeModbus) | OS 级库 (PyModbus/NModbus) |
|---|---|---|
| T3.5 实现方式 | 每字节重置计时器（硬件或软件） | `VTIME`（Linux, 最小粒度 100ms）或 `ReadIntervalTimeout`（Windows, 最小粒度 ~1ms） |
| 常用波特率下的精度 | 精确（微秒级） | **在大多数波特率下不可实现** |
| 9600 bps 的 T3.5 | 3.65ms → 精确可达 | Linux: 最接近 0 或 100ms；Windows: 最接近 1ms（3.6 倍误差） |
| 115200 bps 的 T3.5 | 0.30ms → 精确可达 | Linux: 完全不可表达；Windows: 1ms = 3.3 倍误差 |
| 帧粘合风险 | **无**（T3.5 精确判定帧边界） | **存在**（超时过大 → 不同帧的字节被粘合，依赖 CRC 作为最后防线） |
| 帧截断风险 | **无**（字节间隔 > T3.5 才判定结束） | **存在**（超时过小 → 正常帧被截断） |

**结论**：在 OS 上，正确地实现 Modbus RTU 的 T3.5 在物理上是不可能的——OS 的定时器粒度不支持。这不是 PyModbus/NModbus 的 bug，是 **POSIX termios 的设计缺陷**。Modbus 协议诞生于 1979 年，termios 的 VTIME 同样古老——两者从未被设计为协同工作。

#### 阻塞模型

| | 嵌入式库 (modbusX) | PyModbus (async) | PyModbus (sync) | NModbus |
|---|---|---|---|---|
| 原生模型 | 非阻塞 Tick 轮询 | asyncio 协程（逻辑上仍是请求-等待） | 同步阻塞 | 同步阻塞（方法名带 `Async` 但内部是 `Task.Run(blocking_read)`） |
| 等待回复期间 CPU 状态 | 继续执行其他任务 | 协程挂起，事件循环处理其他协程 | **线程阻塞，CPU 空转或睡眠** | **线程阻塞** |
| 多从机并发 | 链表顺序驱动，一个 Tick | `asyncio.Lock` 全局串行化 | 不能（需多线程） | 不能（需多线程） |
| 对应用代码的影响 | 周期性调用 Tick | 需要在 asyncio 事件循环中运行 | **卡住调用线程** | **卡住调用线程** |
| 资源消耗 | Tick 占用 CPU 在微秒级 | 事件循环开销 + 协程调度 | 线程栈（MB 级） | 线程栈（MB 级） |

**PyModbus 的 async 不是真正的非阻塞。** `asyncio.Lock` 确保同一时刻只有一个请求在飞行——从协议角度看这是正确的（RTU 是半双工），但这个锁是所有目标从机共享的。请求从机 A 等待超时期间，请求从机 B 也被锁挡住，即使从机 B 完全可用。

#### 错误处理

| | 嵌入式库 (modbusX) | OS 级库 |
|---|---|---|
| 帧级错误 | 精确区分（CRC 错误、超时、地址不匹配、功能码不支持） | 通常只能区分"超时"和"其他错误" |
| 错误可见性 | 完整的错误信息（发送的功能码、起始地址、寄存器数量、错误码） | 通常只抛出一个异常，丢失上下文 |
| 错误恢复 | 自动（帧丢弃，回到 IDLE 等待下一帧） | 依赖用户 try-catch 重试逻辑 |
| 总线健康诊断 | 可通过错误队列分析通信质量 | 难以实现（异常粒度太粗） |

#### 资源消耗

| | modbusX (ARM, 精简配置) | PyModbus (Linux) | NModbus (Windows) |
|---|---|---|---|
| ROM / 磁盘 | ~5-8 KB | ~500 KB（.py + 依赖） | ~200 KB（.dll） |
| RAM | ~1-2 KB | ~10-50 MB（Python 运行时） | ~10-30 MB（.NET 运行时） |
| 最小系统 | Cortex-M0, 16KB flash, 4KB RAM | Linux + Python 3.8+ | Windows/Linux + .NET 6+ |
| 启动时间 | 微秒级 | 秒级（Python 解释器启动） | 秒级（CLR 启动） |

**这不是公平对比**——它们运行在不同世界。只是想说明：嵌入式库和 OS 库之间的资源差距是 **3-4 个数量级**，而不是百分之几十。

---

### 适用场景

| 场景 | 推荐 |
|---|---|
| STM32/AVR 裸机 Modbus 设备 | FreeModbus / modbusX / libmodbus 嵌入式分支 |
| 树莓派工业网关（Linux） | libmodbus（主线）或 modbusX（如果需要精确 T3.5） |
| Python 脚本临时读写 Modbus 设备 | PyModbus |
| Windows 上位机 / SCADA | NModbus |
| 生产级嵌入式 Modbus 产品 | FreeModbus（资源优先）或 modbusX（可维护性优先） |
| 需同时做主站和从站的嵌入式设备 | modbusX 或 libmodbus |
| 需精确 Modbus RTU 诊断工具 | modbusX 或 FreeModbus（PyModbus/NModbus 无法查看帧级细节） |

---

## 总览

```
                        资源消耗 →
                  低                    高
          ┌─────────┬─────────┬─────────┬─────────┐
    裸机   │FreeModbus│ modbusX  │         │         │
  平台     │          │          │         │         │
  抽   ────┼──────────┼──────────┼─────────┼─────────┤
  象      │libmodbus │          │         │         │
  层      │(嵌入式)   │          │         │         │
  级   ────┼──────────┼──────────┼─────────┼─────────┤
          │          │          │libmodbus│ NModbus │
    OS    │          │          │ (主线)   │ PyModbus│
  平台    │          │          │         │         │
          └─────────┴─────────┴─────────┴─────────┘
                        抽象层级 →
                  低                    高
```

**最终结论**：

1. **在嵌入式裸机领域**，FreeModbus 是最保守（也最省资源）的选择，modbusX 牺牲少量资源换取更好的可移植性、更低的维护成本和更灵活的多实例能力。没有绝对的优劣——取决于你的 MCU 有多紧张、你的应用有多复杂。

2. **在 OS 平台**，PyModbus/NModbus 的便利性无可替代——一个 `pip install` + 三行代码就能读写 Modbus 设备。但它们的"阻塞"不是实现问题，是**架构问题**：POSIX 串口抽象在底层就无法精确实现 Modbus RTU 的帧定界。这是所有 OS 级 Modbus 库共享的原罪。

3. **不存在全能的 Modbus 库。** 不同平台、不同资源约束、不同响应时间要求，对应不同的最优选择。理解这些库的设计取舍，比记住它们的特性列表更重要。
