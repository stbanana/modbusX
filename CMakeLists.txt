# 指定CMake的最低版本要求为3.22
cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)            # 设置C标准为C11
set(CMAKE_C_STANDARD_REQUIRED ON)   # 要求使用指定的C标准
set(CMAKE_C_EXTENSIONS ON)          # 启用编译器扩展

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include("${CMAKE_CURRENT_LIST_DIR}/Example/win-gcc.cmake") # 包含编译环境的配置文件
include(FetchContent)

project(WinProjects)
enable_language(C)

# 使用 FetchContent 拉取 modbusX 源码
FetchContent_Declare(
    modbusx
    GIT_REPOSITORY https://github.com/stbanana/modbusX.git
    GIT_TAG main
    SOURCE_SUBDIR common  # 使用子目录，避免触发测试主构建
)
# 使能 FetchContent_MakeAvailable，这将下载 modbusX
FetchContent_MakeAvailable(modbusx)


set(BUILD_PROJECT_NAME RTU_Mmain)   # 控制编译RTU主机测试
add_executable(${BUILD_PROJECT_NAME})

target_compile_definitions(${BUILD_PROJECT_NAME} PRIVATE
    MBX_INCLUDE_USER_DEFINE_FILE # 控制使用用户自定义的配置文件
)
target_include_directories(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test
)
target_sources(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/RTU_Mmain.c
)

target_link_libraries(${BUILD_PROJECT_NAME} # 没有修饰词默认为 PRIVATE 
    modbusx 
)


set(BUILD_PROJECT_NAME RTU_Smain) # 控制编译RTU从机测试
add_executable(${BUILD_PROJECT_NAME})

target_compile_definitions(${BUILD_PROJECT_NAME} PRIVATE 
    MBX_INCLUDE_USER_DEFINE_FILE # 控制使用用户自定义的配置文件
)
target_include_directories(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test
)
target_sources(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/RTU_Smain.c
)
target_link_libraries(${BUILD_PROJECT_NAME} # 没有修饰词默认为 PRIVATE 
    modbusx 
)


# 启用测试
enable_testing()

# 添加子目录
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/unitTest/test_case/test_utility)
