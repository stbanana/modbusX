# 单独管理构建后的任务

# 定义工具链工具
set(OBJCOPY arm-none-eabi-objcopy)
set(OBJDUMP arm-none-eabi-objdump)

# 添加自定义命令生成 bin 和 hex 文件
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMAND ${OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND  ${CMAKE_CURRENT_LIST_DIR}/gccMapView.exe ${CMAKE_BINARY_DIR}
    COMMENT "Generating bin and hex files from elf"
)