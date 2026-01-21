################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.c 

C_DEPS += \
./ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.d 

OBJS += \
./ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.o 


# Each subdirectory must supply rules for building sources it contributes
ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.o: C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.c ext_lib/CSP_1.6-main/src/func/func_timesync/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ext_lib-2f-CSP_1-2e-6-2d-main-2f-src-2f-func-2f-func_timesync

clean-ext_lib-2f-CSP_1-2e-6-2d-main-2f-src-2f-func-2f-func_timesync:
	-$(RM) ./ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.cyclo ./ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.d ./ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.o ./ext_lib/CSP_1.6-main/src/func/func_timesync/Func_Time_sync.su

.PHONY: clean-ext_lib-2f-CSP_1-2e-6-2d-main-2f-src-2f-func-2f-func_timesync

