# 指定CMake的最低版本要求为3.22
cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)            # 设置C标准为C11
set(CMAKE_C_STANDARD_REQUIRED ON)   # 要求使用指定的C标准
set(CMAKE_C_EXTENSIONS ON)          # 启用编译器扩展


include("${CMAKE_CURRENT_LIST_DIR}/Example/win-gcc.cmake")
enable_language(C)
project(WinProjects)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)


set(BUILD_PROJECT_NAME RTU_Mmain)   # 控制编译RTU主机测试
add_executable(${BUILD_PROJECT_NAME})

target_compile_definitions(${BUILD_PROJECT_NAME} PRIVATE
    MBX_INCLUDE_USER_DEFINE_FILE # 控制使用用户自定义的配置文件
)
target_sources(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/RTU_Mmain.c
)
target_include_directories(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test
)
target_link_libraries(${BUILD_PROJECT_NAME}
    modbusx 
)


set(BUILD_PROJECT_NAME RTU_Smain) # 控制编译RTU从机测试
add_executable(${BUILD_PROJECT_NAME})

target_compile_definitions(${BUILD_PROJECT_NAME} PRIVATE 
    MBX_INCLUDE_USER_DEFINE_FILE # 控制使用用户自定义的配置文件
)
target_sources(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/RTU_Smain.c
)
target_include_directories(${BUILD_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test
)
target_link_libraries(${BUILD_PROJECT_NAME}
    modbusx 
)
