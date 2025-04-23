cmake_minimum_required(VERSION 3.22)

project(MODBUSX)
add_library(modbusx INTERFACE) # INTERFACE意味着这个库本身并不会被编译，而是作为依赖被其他目标使用，以便于MBx_user.h可以自己定义并且对库行为进行变更

# 递归查找所有源码文件
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/source/*.c)
# 非递归的案例
# file(GLOB SRC ${CMAKE_CURRENT_LIST_DIR}/source/*.c)

target_include_directories(modbusx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if(WIN32)
    target_include_directories(modbusx INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../port/win32/inc
    )
else()
    target_include_directories(modbusx INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../port/generic/inc
    )
endif()

target_sources(modbusx INTERFACE
     ${SRC}
)

if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()
