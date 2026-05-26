################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/Func_SSDV.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/Func_Time_sync.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_bridge.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_buffer.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_conn.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_crc32.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_debug.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_dedup.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_endian.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_hex_dump.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_iflist.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_init.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_io.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_port.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_promisc.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_qfifo.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_route.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_service_handler.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_services.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_sfp.c 

C_DEPS += \
./CSP_1.6/src/Func_SSDV.d \
./CSP_1.6/src/Func_Time_sync.d \
./CSP_1.6/src/csp_bridge.d \
./CSP_1.6/src/csp_buffer.d \
./CSP_1.6/src/csp_conn.d \
./CSP_1.6/src/csp_crc32.d \
./CSP_1.6/src/csp_debug.d \
./CSP_1.6/src/csp_dedup.d \
./CSP_1.6/src/csp_endian.d \
./CSP_1.6/src/csp_hex_dump.d \
./CSP_1.6/src/csp_iflist.d \
./CSP_1.6/src/csp_init.d \
./CSP_1.6/src/csp_io.d \
./CSP_1.6/src/csp_port.d \
./CSP_1.6/src/csp_promisc.d \
./CSP_1.6/src/csp_qfifo.d \
./CSP_1.6/src/csp_route.d \
./CSP_1.6/src/csp_service_handler.d \
./CSP_1.6/src/csp_services.d \
./CSP_1.6/src/csp_sfp.d 

OBJS += \
./CSP_1.6/src/Func_SSDV.o \
./CSP_1.6/src/Func_Time_sync.o \
./CSP_1.6/src/csp_bridge.o \
./CSP_1.6/src/csp_buffer.o \
./CSP_1.6/src/csp_conn.o \
./CSP_1.6/src/csp_crc32.o \
./CSP_1.6/src/csp_debug.o \
./CSP_1.6/src/csp_dedup.o \
./CSP_1.6/src/csp_endian.o \
./CSP_1.6/src/csp_hex_dump.o \
./CSP_1.6/src/csp_iflist.o \
./CSP_1.6/src/csp_init.o \
./CSP_1.6/src/csp_io.o \
./CSP_1.6/src/csp_port.o \
./CSP_1.6/src/csp_promisc.o \
./CSP_1.6/src/csp_qfifo.o \
./CSP_1.6/src/csp_route.o \
./CSP_1.6/src/csp_service_handler.o \
./CSP_1.6/src/csp_services.o \
./CSP_1.6/src/csp_sfp.o 


# Each subdirectory must supply rules for building sources it contributes
CSP_1.6/src/Func_SSDV.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/Func_SSDV.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/Func_Time_sync.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/Func_Time_sync.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_bridge.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_bridge.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_buffer.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_buffer.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_conn.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_conn.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_crc32.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_crc32.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_debug.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_debug.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_dedup.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_dedup.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_endian.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_endian.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_hex_dump.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_hex_dump.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_iflist.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_iflist.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_init.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_init.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_io.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_io.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_port.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_port.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_promisc.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_promisc.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_qfifo.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_qfifo.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_route.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_route.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_service_handler.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_service_handler.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_services.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_services.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/csp_sfp.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/csp_sfp.c CSP_1.6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CSP_1-2e-6-2f-src

clean-CSP_1-2e-6-2f-src:
	-$(RM) ./CSP_1.6/src/Func_SSDV.cyclo ./CSP_1.6/src/Func_SSDV.d ./CSP_1.6/src/Func_SSDV.o ./CSP_1.6/src/Func_SSDV.su ./CSP_1.6/src/Func_Time_sync.cyclo ./CSP_1.6/src/Func_Time_sync.d ./CSP_1.6/src/Func_Time_sync.o ./CSP_1.6/src/Func_Time_sync.su ./CSP_1.6/src/csp_bridge.cyclo ./CSP_1.6/src/csp_bridge.d ./CSP_1.6/src/csp_bridge.o ./CSP_1.6/src/csp_bridge.su ./CSP_1.6/src/csp_buffer.cyclo ./CSP_1.6/src/csp_buffer.d ./CSP_1.6/src/csp_buffer.o ./CSP_1.6/src/csp_buffer.su ./CSP_1.6/src/csp_conn.cyclo ./CSP_1.6/src/csp_conn.d ./CSP_1.6/src/csp_conn.o ./CSP_1.6/src/csp_conn.su ./CSP_1.6/src/csp_crc32.cyclo ./CSP_1.6/src/csp_crc32.d ./CSP_1.6/src/csp_crc32.o ./CSP_1.6/src/csp_crc32.su ./CSP_1.6/src/csp_debug.cyclo ./CSP_1.6/src/csp_debug.d ./CSP_1.6/src/csp_debug.o ./CSP_1.6/src/csp_debug.su ./CSP_1.6/src/csp_dedup.cyclo ./CSP_1.6/src/csp_dedup.d ./CSP_1.6/src/csp_dedup.o ./CSP_1.6/src/csp_dedup.su ./CSP_1.6/src/csp_endian.cyclo ./CSP_1.6/src/csp_endian.d ./CSP_1.6/src/csp_endian.o ./CSP_1.6/src/csp_endian.su ./CSP_1.6/src/csp_hex_dump.cyclo ./CSP_1.6/src/csp_hex_dump.d ./CSP_1.6/src/csp_hex_dump.o ./CSP_1.6/src/csp_hex_dump.su ./CSP_1.6/src/csp_iflist.cyclo ./CSP_1.6/src/csp_iflist.d ./CSP_1.6/src/csp_iflist.o ./CSP_1.6/src/csp_iflist.su ./CSP_1.6/src/csp_init.cyclo ./CSP_1.6/src/csp_init.d ./CSP_1.6/src/csp_init.o ./CSP_1.6/src/csp_init.su ./CSP_1.6/src/csp_io.cyclo ./CSP_1.6/src/csp_io.d ./CSP_1.6/src/csp_io.o ./CSP_1.6/src/csp_io.su ./CSP_1.6/src/csp_port.cyclo ./CSP_1.6/src/csp_port.d ./CSP_1.6/src/csp_port.o ./CSP_1.6/src/csp_port.su ./CSP_1.6/src/csp_promisc.cyclo ./CSP_1.6/src/csp_promisc.d ./CSP_1.6/src/csp_promisc.o ./CSP_1.6/src/csp_promisc.su ./CSP_1.6/src/csp_qfifo.cyclo ./CSP_1.6/src/csp_qfifo.d ./CSP_1.6/src/csp_qfifo.o ./CSP_1.6/src/csp_qfifo.su ./CSP_1.6/src/csp_route.cyclo ./CSP_1.6/src/csp_route.d ./CSP_1.6/src/csp_route.o ./CSP_1.6/src/csp_route.su ./CSP_1.6/src/csp_service_handler.cyclo ./CSP_1.6/src/csp_service_handler.d ./CSP_1.6/src/csp_service_handler.o ./CSP_1.6/src/csp_service_handler.su ./CSP_1.6/src/csp_services.cyclo ./CSP_1.6/src/csp_services.d ./CSP_1.6/src/csp_services.o ./CSP_1.6/src/csp_services.su ./CSP_1.6/src/csp_sfp.cyclo ./CSP_1.6/src/csp_sfp.d ./CSP_1.6/src/csp_sfp.o ./CSP_1.6/src/csp_sfp.su

.PHONY: clean-CSP_1-2e-6-2f-src

