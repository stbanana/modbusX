cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)            # 设置C标准为C11
set(CMAKE_C_STANDARD_REQUIRED ON)   # 要求使用指定的C标准
set(CMAKE_C_EXTENSIONS ON)          # 启用编译器扩展

set(BUILD_PROJECT_NAME Win_Mmain) # 控制编译主机测试

include("${CMAKE_CURRENT_LIST_DIR}/Example/win-gcc.cmake")

enable_language(C)

project(${BUILD_PROJECT_NAME})
message("Build type: " ${BUILD_PROJECT_NAME})

add_executable(${BUILD_PROJECT_NAME})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)

target_sources(modbusx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/Win_Mmain.c
)

target_link_libraries(${BUILD_PROJECT_NAME}
    modbusx 
)


# set(BUILD_PROJECT_NAME Win_Smain) # 控制编译从机测试

# project(${BUILD_PROJECT_NAME})
# message("Build type: " ${BUILD_PROJECT_NAME})

# add_executable(${BUILD_PROJECT_NAME})

# target_sources(modbusx INTERFACE
# ${CMAKE_CURRENT_LIST_DIR}/Example/win_test/Win_Smain.c
# )

# target_link_libraries(${BUILD_PROJECT_NAME}
#     modbusx 
# )
