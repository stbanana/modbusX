set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          x86_64)        # 设置处理器架构为x86_64

set(CMAKE_C_COMPILER_FORCED TRUE)       # 强制指定C编译器
set(CMAKE_CXX_COMPILER_FORCED TRUE)     # 强制指定C++编译器
set(CMAKE_C_COMPILER_ID GNU)            # 设置C编译器ID为GNU
set(CMAKE_CXX_COMPILER_ID GNU)          # 设置C++编译器ID为GNU

set(TOOLCHAIN_PREFIX                 )

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)     # 设置C编译器
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})        # 设置汇编编译器，使用C编译器
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)     # 设置C++编译器
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}g++)     # 设置链接器
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy) # 设置对象复制工具
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)    # 设置大小计算工具

set(CMAKE_EXECUTABLE_SUFFIX_C       ".exe")     # 设置C可执行文件后缀为.exe
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".exe")     # 设置C++可执行文件后缀为.exe

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)   # 设置尝试编译的目标类型为静态库

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g0")

set(CMAKE_C_LINK_FLAGS "")
