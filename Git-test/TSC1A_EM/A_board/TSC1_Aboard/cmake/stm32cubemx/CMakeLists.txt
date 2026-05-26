cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

set(LIB_PATH "../../../../ext_lib")

file(GLOB STM32_SOURCES "../../Core/Src/*.c" "../../Core/Src/*.cpp")
file(GLOB ASM330LHH "${LIB_PATH}/driver/ASM330LHH/*.c" "${LIB_PATH}/driver/ASM330LHH/*.cpp")
file(GLOB MMC5983MA "${LIB_PATH}/driver/MMC5983MA/*.c" "${LIB_PATH}/driver/MMC5983MA/*.cpp")

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32F765xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ${LIB_PATH}
    ../../Core/Inc
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Inc
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/include
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/CMSIS/Device/ST/STM32F7xx/Include
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ${STM32_SOURCES}
    ${ASM330LHH}
    ${MMC5983MA}
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi_ex.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
    ../../Core/Src/system_stm32f7xx.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/list.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    C:/Users/megus/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../Core/Src/main.cpp
    ../../startup_stm32f765xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


