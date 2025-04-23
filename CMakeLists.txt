cmake_minimum_required(VERSION 3.22)
project(MODBUSX_WRAPPER)

# 包含原库配置
add_subdirectory(common)  

# 创建新的共享库目标
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/common/source/*.c)
add_library(modbusx_shared SHARED ${SRC})

# 继承原库的INTERFACE属性
target_link_libraries(modbusx_shared PUBLIC modbusx)

# 设置符号可见性（关键！）
if(WIN32)
    target_compile_definitions(modbusx_shared PRIVATE "MBX_API=__declspec(dllexport)")
else()
    target_compile_definitions(modbusx_shared PRIVATE "MBX_API=__attribute__((visibility("default")))")
endif()

# 强制C11标准
set_target_properties(modbusx_shared PROPERTIES
    C_STANDARD 11
    C_VISIBILITY_PRESET hidden
)