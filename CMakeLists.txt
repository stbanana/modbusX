# 指定CMake的最低版本要求为3.22
cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)            # 设置C标准为C11
set(CMAKE_C_STANDARD_REQUIRED ON)   # 要求使用指定的C标准
set(CMAKE_C_EXTENSIONS ON)          # 启用编译器扩展

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# 配置目标和链接属性
function(configure_target target_name)
    # 将剩余参数视为源文件列表
    add_executable(${target_name} ${ARGN})

    target_sources(${target_name} PUBLIC
    ${ARGN}
    )

    target_compile_definitions(${target_name} PUBLIC  ${MY_DEFINITIONS})
    target_include_directories(${target_name} PUBLIC ${MY_INCLUDE_DIR})
    target_link_libraries(${target_name} PUBLIC modbusx)

    # 添加链接器选项
    target_link_options(${target_name} PRIVATE
        -Wl,-Map=${target_name}.map
        -Wl,--gc-sections
    )
endfunction()



include("${CMAKE_CURRENT_LIST_DIR}/Example/win-gcc.cmake") # 包含编译环境的配置文件
include(FetchContent)

project(WinProjects)
enable_language(C)

# # 使用 FetchContent 拉取 modbusX 源码
# FetchContent_Declare(
#     modbusx
#     GIT_REPOSITORY https://github.com/stbanana/modbusX.git
#     GIT_TAG main
#     SOURCE_SUBDIR common  # 使用子目录，避免触发测试主构建
# )
# # 使能 FetchContent_MakeAvailable，这将下载 modbusX
# FetchContent_MakeAvailable(modbusx)

# 使用本地文件不进行拉取
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)     # 添加子目录

set(MY_INCLUDE_DIR 
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test
)
set(MY_DEFINITIONS 
    MBX_INCLUDE_USER_DEFINE_FILE
)



# 配置每个可执行文件
configure_target(RTU_Mmain ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/RTU_Mmain.c)
configure_target(RTU_Smain ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/RTU_Smain.c)

configure_target(TCP_Smain ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/TCP_Smain.c)
target_link_libraries(TCP_Smain PUBLIC wsock32 ws2_32)

configure_target(TCP_Mmain ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/TCP_Mmain.c)
target_link_libraries(TCP_Mmain PUBLIC wsock32 ws2_32)

# 启用测试
enable_testing()

# 添加子目录
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/unitTest/test_case/test_utility)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/unitTest/test_case/test_library)


# arm-gcc 环境下的资源占用测试
# set(CMAKE_PROJECT_NAME "arm-size-test")
# include("${CMAKE_CURRENT_LIST_DIR}/Example/gcc-arm-none-eabi.cmake") # 包含编译环境的配置文件
# enable_language(C ASM)
# project(${CMAKE_PROJECT_NAME})                  # 定义项目，使用之前设置的项目名称
# message("Build type: " ${CMAKE_BUILD_TYPE})     # 消息输出构建类型
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)     # 添加子目录
# set(MY_INCLUDE_DIR 
#     ${CMAKE_CURRENT_LIST_DIR}/Example/arm_size
# )
# set(MY_DEFINITIONS 
#     MBX_INCLUDE_USER_DEFINE_FILE
# )
# set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# configure_target(${CMAKE_PROJECT_NAME} 
#     ${CMAKE_CURRENT_LIST_DIR}/Example/arm_size/startup_stm32h743xx.s
#     ${CMAKE_CURRENT_LIST_DIR}/Example/arm_size/arm_main.c 
#     ${CMAKE_CURRENT_LIST_DIR}/Example/arm_size/syscalls.c
# )
# include("${CMAKE_CURRENT_LIST_DIR}/Example/arm_size/post-build-tasks.cmake")