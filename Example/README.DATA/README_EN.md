<p align="center">
 <h1 align="center">modbusX (Translated from GPT)</h1>
 <p align="center">A decoupled Modbus protocol parsing stack at the physical drive layer</p>
</p>

<p align="center">
    <a href="https://github.com/anuraghazra/github-readme-stats/graphs/contributors"><img alt="C" src="https://img.shields.io/badge/C-00599C.svg?logo=c&logoColor=white" /></a>
    <a href="https://mit-license.org"><img alt="Static Badge" src="https://img.shields.io/badge/license%20-%20MIT%20-%20blue" /></a>
    <a href="https://github.com/stbanana/modbusX/actions/workflows/c-cpp.yml"><img alt="C/C++ CI" src="https://github.com/stbanana/modbusX/actions/workflows/c-cpp.yml/badge.svg" /></a>
</p>

<p align="center">
    <a href="https://wiki.yono233.cn/modbusX/zh_hans/">Documentation</a>
</p>

<p align="center">
    <a href="/README.md">Simplified Chinese</a>
    ·
    <a href="/Example/README.DATA/README_EN.md">English</a>
</p>

## ✨Features

My understanding of the protocol library is that you just need to implement the input and output ports for data, and possibly call a timing function at intervals, to drive the entire library (polling).

If you would like to learn more about the Modbus protocol, visit the [Introduction to Modbus Protocol](https://www.yono233.cn/posts/shoot/24_7_26_modbus%20protocol%20introduction%20(not%20completed)).)

Additionally, there is a lightweight report on the library [Resource Usage Test Report](https://wiki.yono233.cn/modbusX/zh_hans/apidoc/tools/资源占用测试报告.html).

I hope to have this kind of Modbus implementation; it is a hard requirement and also the final feature of this library.

> 1. ⚡ **Non-blocking**: Use the library's tick, and disable blocking.
> 2. 🪶 **Non-threaded**: A single tick drives the entire master-slave chain, allowing tick to operate periodically in any manner.
> 3. 🌠 **No interrupt usage**: Free port binding to manage data flow in any way.
> 4. 💪 **Multiple masters and slaves**: Any master or slave is an independent object, allowing for any number of masters and slaves to be initiated in the program.
> 5. 🗽 **Arbitrary mapping between register address tables and memory addresses**: This is a crucial feature of the library that addresses the mapping relationship between Modbus virtual addresses and actual variables, making development simpler and more flexible. The entire library's usage revolves around this mapping table, and future maintenance will only require updating this table.
> 6. ⚖️ **The lookup time for different address mappings should not exceed 500 clock cycles**: It is hard to declare this as fully implemented. Since Modbus has at most 0xFFFF virtual addresses, a binary search method is used, but this is still not flexible enough. In the future, better table lookup structures may be found (perhaps). This is a challenge for library upgrades; if anyone has solutions, please contact me, thank you very much! 😘
> 7. 🏃‍♀️ **Restrained event callbacks**: A small number of callback mechanisms. Maximize the use of stack polling to minimize impact on system timing.

# 📚 Function Plan (sorted by priority)

- [x] Reading and writing input and holding registers for RTU master-slave.
- [x] Synchronous mechanism for successful host writes.
- [x] Callback handling mechanism for failed host writes.
- [x] Reading and writing input and holding registers for TCP master-slave.
- [ ] Reading and writing coils and discrete inputs for RTU master-slave.
- [ ] Reading and writing coils and discrete inputs for TCP master-slave.
- [ ] Support for dynamic address mapping tables.

# 🤔 How to Use

> [!NOTE]
>
> It is important to note that since some utility functions are defined as `__weak`, it is necessary to use the C11 standard or higher. 
> If the existing project is in C99, modifications are needed for compatibility; search for all `__weak` functions and simply remove this keyword. The other code is compliant with C99.

## RTU Slave

First, implement the port function for the communication port as follows:

```C
/**
 * @brief Set the MBX_SEND_MODE_BYTES macro to 1, allowing for multi-byte sending via port.
 * @parHere's the translation of your blog post:

---

**Sending Buffer Pointer via Serial Port**
* @param Data Pointer to the data to be sent
* @param Len The expected length of data to send
* @return Standard return for port
*/
uint32_t SerialSendPort(const void *Data, size_t Len)
{
    WINBOOL b     = FALSE; // Sending operation flag
    DWORD   wWLen = 0;     // Actual length of data sent
    /* Try to send */
    b = WriteFile(comHandle, Data, Len, &wWLen, NULL);
    if(b && wWLen == Len)
        return MBX_PORT_RETURN_DEFAULT;
    else
        return MBX_PORT_RETURNT_ERR_INDEFINITE;
}

/**
 * @brief Data receiving port, functionality to read a single byte, return value indicates success of reception
 * @param Data Pointer to a byte to receive
 * @return Standard return for port
 */
uint32_t SerialGetcPort(uint8_t *Data)
{
    WINBOOL b     = FALSE; // Receiving operation flag
    DWORD   wRLen = 0;     // Actual length of data received
    /* Try to receive */
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

/* 
Next, create a mapping table similar to the one below, taking note of the following two points:

1. The **register addresses of the data model must be in increasing order**; the library uses binary search to improve lookup efficiency.
2. **If mapped internal memory variables are the same, the callback handling should also be the same**, as the library will ultimately pass the expected modified value of the entire variable in a void* format to the write callback. The write callback should then translate it back to the corresponding data, as detailed in the example on w... 

--- 

Feel free to modify any specific technical terms to fit your context.**RTU Slave Example on Windows Platform**

```c
/* Memory areas for mapping */
uint8_t  u8MapMem[64];
uint16_t u16MapMem[64];
uint32_t u32MapMem[64];
uint64_t u64MapMem[64];
float    fMapMem[64]; // 32-bit data model
double   dMapMem[64]; // 64-bit data model

/* To achieve faster lookups, the library uses a binary search method for the address table.
   The address table must be manually sorted in ascending order. Since the C11 standard does not support dynamic macros,
   automatic checks at compile time are temporarily not available. */
static const _MBX_MAP_LIST_ENTRY MapList[] = {
    /*  Register Address     Mapped Internal Memory         Internal Memory Data Type     Write Callback (NULL means read-only register) */
    {.Addr = 0x0000, .Memory = &u8MapMem[0],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest1 },
    {.Addr = 0x0001, .Memory = &u8MapMem[1],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest2 },
    {.Addr = 0x0002, .Memory = &u8MapMem[2],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0003, .Memory = &u8MapMem[3],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    
    {.Addr = 0x0100, .Memory = &u16MapMem[0], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest1},
    {.Addr = 0x0101, .Memory = &u16MapMem[1], .Type = MBX_REG_TYPE_U16,   .Handle = NULL         },
    // Additional entries can be added as needed
};
```This code snippet appears to be a part of a configuration for memory mapping and handling write operations for different types of registers. Below is the translated content retaining its original meaning:

```c
REG_TYPE_U16,   .Handle = u16WriteTest2},
    
{.Addr = 0x0200, .Memory = &u36MapMem[0], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest1},
{.Addr = 0x0201, .Memory = &u36MapMem[0], .Type = MBX_REG_TYPE_U32_L, .Handle = u32WriteTest1},
    
{.Addr = 0x0300, .Memory = &u64MapMem[0], .Type = MBX_REG_TYPE_U64_3, .Handle = u64WriteTest1},
{.Addr = 0x0301, .Memory = &u64MapMem[0], .Type = MBX_REG_TYPE_U64_2, .Handle = u64WriteTest1},
{.Addr = 0x0302, .Memory = &u64MapMem[0], .Type = MBX_REG_TYPE_U64_1, .Handle = u64WriteTest1},
{.Addr = 0x0303, .Memory = &u64MapMem[0], .Type = MBX_REG_TYPE_U64_0, .Handle = u64WriteTest1},
    
{.Addr = 0x0400, .Memory = &fMapMem[0],   .Type = MBX_REG_TYPE_U32_H, .Handle = fWriteTest1  }, 
{.Addr = 0x0401, .Memory = &fMapMem[0],   .Type = MBX_REG_TYPE_U32_L, .Handle = fWriteTest1  },
    
{.Addr = 0x0500, .Memory = &fMapMem[0],   .Type = MBX_REG_TYPE_U64_3, .Handle = dWriteTest1  }, 
{.Addr = 0x0501, .Memory = &fMapMem[0],
```

Please note that the snippet is still incomplete, and the last line should be finalized in the complete code context.Initialization in the main program

```c
/* Allocate buffers for sending and receiving slave objects */
uint8_t *SRxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));
uint8_t *STxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));

/* Initialize RTU Slave 1 */
MBx_Slave_RTU_Init(&MBxSlave,      // Slave object
                   1,              // Slave ID
                   MapList,        // Address mapping table
                   SerialSendPort, // Send function
                   SerialGetcPort, // Receive function
                   9600,           // Baud rate
                   SRxBuffer,      // Allocated internal receive buffer
                   84,             // Maximum length of receive buffer
                   STxBuffer,      // Allocated internal send buffer
                   84);            // Maximum length of send buffer

/* Pretend to initialize Slave 2 (if it really exists, fill in the parameters correctly) */
MBx_Slave_RTU_Init(&MBxSlave
```2,     // Slave object
                    2,              // Slave ID
                    MapList,        // Address mapping table
                    MBX_PARA_NULL,  // Send function
                    MBX_PARA_NULL,  // Receive function
                    MBX_PARA_NULL,  // Baud rate
                    MBX_PARA_NULL,  // Internal allocation for receive buffer
                    MBX_PARA_NULL,  // Maximum length of receive buffer
                    MBX_PARA_NULL,  // Internal allocation for send buffer
                    MBX_PARA_NULL); // Maximum length of send buffer

```c
// Periodically call the driver function
while(1)
{
    MBx_Ticks(1000);  // Convert to microseconds and pass to MBx driver for automatic linked list driving
    Sleep(1);          // 1ms period
}
```

## RTU Master

First, implement a port function similar to the slave; no further explanation needed.

Then, create an address mapping table similar to the one below. Note the following two points:

> 1. The **register addresses of the data model must be incremental**. Use binary search within the library to improve lookup efficiency.
> 2. **If the internal memory variables mapped are the same, the callback handling should also be the same** because the library will ultimately pass the expected modified value of the entire variable as void* in the write callback. The write callback should then translate this back to the corresponding data type. For details, see the example [RTU Master Example on Windows Platform](Example/win_test/RTU_Mmain.c).

```c
/* Memory region for mapping */
uint8_t  u8MapMem[64];
uint16_t u16MapMem[64];
uint32_t u32MapMem[64];
uint64_t u64MapMem[64];
float    fMapMem[64]; // 32-bit data model
double   dMapMem[64]; // 64-bit data model
```]; // 64-bit data model

/* To achieve faster lookups, a binary search method is used for the address table within the library.
    The address table must be manually sorted in ascending order. Due to the C11 standard not supporting dynamic macros, 
    it's currently impossible to automatically check at compile time. */
static const _MBX_MAP_LIST_ENTRY MapList[] = {
    /*  Register Address     Mapped Internal Memory           Internal Memory Data Attribute        Immediate Callback on Write Exception (NULL to ignore write exceptions)  */
    {.Addr = 0x0000, .Memory = &u8MapMem[10],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest1 },
    {.Addr = 0x0001, .Memory = &u8MapMem[11],  .Type = MBX_REG_TYPE_U8,    .Handle = u8WriteTest2 },
    {.Addr = 0x0002, .Memory = &u8MapMem[12],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0003, .Memory = &u8MapMem[13],  .Type = MBX_REG_TYPE_U8,    .Handle = NULL         },
    {.Addr = 0x0100, .Memory = &u16MapMem[10], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest1},
    {.Addr = 0x0101, .Memory = &u16MapMem[11], .Type = MBX_REG_TYPE_U16,   .Handle = u16WriteTest2},
    {.Addr = 0x0200, .Memory = &u36MapMem[10], .Type = MBX_REG_TYPE_U32_H, .Handle = u32WriteTest1}, /* Multiple register combinations map to the same memory variable, write exception callbacks should be the same (hard requirement). Simulates big-endian memory (ABCD arrangement based on the transfer protocol, which is most suitable). */In the main program, initialization is performed, and the slave managed by the host also adopts a linked structure, so it needs to be registered separately.

```c
    /* Allocate buffers for the host object to send and receive */
    uint8_t *SRxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));
    uint8_t *STxBuffer = (uint8_t *)malloc(84 * sizeof(uint8_t));

    /* Initialize Modbus master 1 */
    MBx_Master_RTU_Init(&MBxMaster,     // Host object
                        SerialSendPort, // Send function
                        SerialGetcPort, // Receive function
                        9600,           // Baud rate
                        SRxBuffer,      // Library internal receive buffer allocation
                        84,             // Maximum length of receive buffer
                        STxBuffer,      // Library internal send buffer allocation
                        84);            // // Maximum length of the send buffer

/* Add Slave 1 managed by Host 1 */
if (MBx_Master_Member_Add(&MBxMaster,        // Host object
                          &MBxMasterMember1, // Slave member object
                          1,                 // Slave ID
                          MapList)           // Mapping table for this slave object
    != MBX_API_RETURN_DEFAULT)
{
    /* Indicates that there is an abnormality in the parameters such as the mapping table or ID */
}

/* Pretend to add Slave 2 managed by Host 1 (if it exists, fill in the parameters correctly) */
if (MBx_Master_Member_Add(&MBxMaster,        // Host object
                          &MBxMasterMember2, // Slave member object
                          MBX_PARA_NULL,     // Slave ID
                          MBX_PARA_NULL)     // Mapping table for this slave object
    != MBX_API_RETURN_DEFAULT)
{
    /* Indicates that there is an abnormality in the parameters such as the mapping table or ID */
}

/* Pretend to initialize Host 2 (if it exists, fill in the parameters correctly) */
MBx_Master_RTU_Init(&MBxMaster2,    // Host object
                    MBX_PARA_NULL,  // Send function
                    MBX_PARA_NULL,  // Receive function
                    MBX_PARA_NULL,  // Baud rate
                    MBX_PARA_NULL,  // Internal buffer allocation for reception
                    MBX_### PARA_NULL,  // Maximum length of receiving buffer
### MBX_PARA_NULL,  // Library allocation for sending buffer
### MBX_PARA_NULL); // Maximum length of sending buffer

You can simply call the driver function at regular intervals. Note that, as the host, a single Tick can consume at most one request, so the frequency at which requests are generated must be lower than the frequency of calling Ticks.

```c
    while(1)
    {
        MBx_Ticks(1000);  // Convert to microseconds and pass to MBx driver; the linked list is driven automatically
        Sleep(1);          // Cycle of 1ms
    }
```

When you want to read from or write to a slave, simply call the API. The request will be pushed into a queue and will be automatically sent and processed within the driver. When a read or write operation succeeds, the intended data to be written or actually read will be synchronized into the memory area of the mapping table. In case of failure, the request will be pushed into an error queue, which will automatically trigger if there is a failure handling mechanism in the mapping table.

```c
MBx_Master_Read_Reg_Request(&MBxMaster, 1, 0, 4);           // Request to read 4 registers at address 0 from slave 1
MBx_Master_Read_Input_Reg_Request(&MBxMaster, 1, 0x100, 2); // Request to read 2 registers at address 0x100 from slave 1 (as input registers are read-only)
MBx_Master_Write_Reg_Mul_Request(&MBxMaster, 1, 0, 4, (uint8_t *)&u16buffer[0], 8); // Request to write to 4 registers at address 0 from slave 1; on successful write, it will automatically synchronize into the mapped memory area
```

Additionally, you can implement an error handling section to retrieve items from the error queue and perform unified custom handling on actual errors. For more details, see the example [RTU Host Example on Windows Platform](Example/win_test/RTU_Mmain.c)

## TCP Slave

The TCP slave is almost identical to the RTU slave; simply replace the API `MBx_Slave_RTU_Init()` at initialization with `MBx_Slave_TCP_Init()`.

For more details, refer to the example [TCP Slave Example on Windows Platform](Example/win_test/TCP_S).## TCP Master

It is almost identical to the RTU master; you can simply replace the initialization API from `MBx_Master_RTU_Init()` to `MBx_Master_TCP_Init()`.

For details, see the example [TCP Master example on Windows platform](Example/win_test/TCP_Mmain.c).

# ✏️ Program State Machine

![Master-Slave State Machine Flow](Example/README.DATA/MBX状态机.svg)

# Program Model

![Master-Slave Model](Example/README.DATA/MBX模型.svg)

# Optimizations

> Multi-threading is discouraged; the tick function should have a single entry point, driving each object in the chain sequentially. In practice, multi-threading is likely to be a negative optimization. If multi-threading is required, consider driving each object separately, at which point you will need to wrap intermediate variables in the mapping tool MBx_utility_map_list.c into objects.
> Performance for mappings of single bits, such as coils and discrete inputs, is relatively poor.