/* 入口点 */
ENTRY(Reset_Handler)

/* 用户模式栈的最高地址, 栈初始化的末端 */
/* _estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM); /*堆空间地址(堆栈向下生长),此值后续会赋值给sp寄存器*/
_estack = __initial_sp; /* 堆空间地址(堆栈向下生长)，与预设堆空间关联*/
/* 如果堆和栈的大小超出RAM容量, 链接时将报错 */
_Min_Heap_Size = 0x200;      /* 预设最小堆栈空间 */
_Min_Stack_Size = 0x400; /* 预设最小堆空间 */

/* 指定存储器区域 */
MEMORY
{
    FLASH (rx)     : ORIGIN = 0x08020000, LENGTH = 0x00200000   /* 最大 2048K  内部FLASH 首地址 0x8000 0000 大小 2048K*/

    RAM_BLFlag(xrw): ORIGIN = 0x20000000, LENGTH = 0x04         /* 与bootload程序共通的内存 */
    DTCMRAM (xrw)  : ORIGIN = 0x20000004, LENGTH = 0x0001FFFC   /* 最大 128K DTCM 首地址 0x2000 0000 大小 128K*/
    RAM_D1 (xrw)   : ORIGIN = 0x24000000, LENGTH = 0x00080000   /* 最大 512K AXI SRAM 首地址 0x24000000 大小 512KB*/
    RAM_D2 (xrw)   : ORIGIN = 0x30000000, LENGTH = 288K         /* 最大288K 128KB SRAM1(0x30000000) + 128KB SRAM2(0x3002 0000) + 32KB SRAM3(0x30040000)*/
    RAM_D3 (xrw)   : ORIGIN = 0x38000000, LENGTH = 64K
}

/* 定义输出段 */
SECTIONS
{

    /* 中断向量表放在APP_FLASH的最前面 */
    .isr_vector (READONLY):
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* 保留中断向量表 */
        . = ALIGN(4);
    } >FLASH

    /* 程序代码和其他数据放在FLASH */
    .text (READONLY):
    {
        . = ALIGN(4);
        *(.text)           /* .text段(代码) */
        *(.text*)          /* .text*段(代码) */
        *(.glue_7)         /* ARM到Thumb的粘合代码 */
        *(.glue_7t)        /* Thumb到ARM的粘合代码 */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;        /* 在代码末尾定义一个全局符号 */
    } >FLASH

    /* 常量数据放在FLASH */
    .rodata (READONLY):
    {
        . = ALIGN(4);
        *(.rodata)         /* .rodata段(常量, 字符串等) */
        *(.rodata*)        /* .rodata*段(常量, 字符串等) */
        . = ALIGN(4);
    } >FLASH

    /* 异常处理信息放在FLASH */
    .ARM.extab (READONLY): 
    { 
        *(.ARM.extab* .gnu.linkonce.armextab.*) 
    } >FLASH

    .ARM (READONLY): 
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    /* 初始化函数指针放在FLASH */
    .preinit_array (READONLY):
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH

    .init_array (READONLY):
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH

    .fini_array (READONLY):
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    /* 一些不进行初始化的内存, 用于bootload或多程序互通 */
    .bss.BLFlag (NOLOAD) : 
    {
        . = ALIGN(4);
        _sblflag = .; /* 定义一个符号指向BLFlag的起始 */
        *(.bss.BLFlag)
        . = ALIGN(4);
        _eblflag = .; /* 定义一个符号指向BLFlag的结束 */
    } > RAM_BLFlag

    /* 由启动代码使用, 用于初始化数据 */
    _sidata = LOADADDR(.data);

    /* 初始化的数据段放在RAM中, 并在代码之后加载 */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* 在数据开始处创建一个全局符号 */
        *(.data)           /* .data段 */
        *(.data*)          /* .data*段 */
        . = ALIGN(4);
        _edata = .;        /* 在数据末尾定义一个全局符号 */
    } >DTCMRAM AT> FLASH

    /* 未初始化的数据段 */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* 在bss开始处定义一个全局符号 */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;         /* 在bss末尾定义一个全局符号 */
        __bss_end__ = _ebss;
    } >RAM_D1

    /* 用户堆栈段, 用于检查RAM剩余空间是否足够 */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
        __initial_sp = .;
        __RAM_segment_used_end__ = .;
    } >DTCMRAM AT>FLASH

    /* RAM_D2区域。 */
    .RAM_D2  :
    {
        *(.RAM_D2)
    } > RAM_D2 AT>FLASH

    /* RAM_D3区域。 */
    .RAM_D3 (NOLOAD) :
    {
        *(.RAM_D3)
    } > RAM_D3 AT>FLASH

    /* 移除标准库中的信息 */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    /* ARM属性段 */
    .ARM.attributes 0 : { *(.ARM.attributes) }
}
