################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.c 

C_DEPS += \
./ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.d 

OBJS += \
./ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.o 


# Each subdirectory must supply rules for building sources it contributes
ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.o: C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.c ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ext_lib-2f-sw-2d-bundle-2d-master-2d-v4-2e-3-2e-0-2e-16-2f-libcubeobc-2f-src-2f-drivers-2f-uart

clean-ext_lib-2f-sw-2d-bundle-2d-master-2d-v4-2e-3-2e-0-2e-16-2f-libcubeobc-2f-src-2f-drivers-2f-uart:
	-$(RM) ./ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.cyclo ./ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.d ./ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.o ./ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/cubeObc_termios.su

.PHONY: clean-ext_lib-2f-sw-2d-bundle-2d-master-2d-v4-2e-3-2e-0-2e-16-2f-libcubeobc-2f-src-2f-drivers-2f-uart

