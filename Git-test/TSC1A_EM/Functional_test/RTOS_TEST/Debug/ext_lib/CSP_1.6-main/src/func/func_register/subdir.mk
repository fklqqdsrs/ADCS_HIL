################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/bit_management.c \
C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/csp_register.c \
C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.c \
C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.c 

C_DEPS += \
./ext_lib/CSP_1.6-main/src/func/func_register/bit_management.d \
./ext_lib/CSP_1.6-main/src/func/func_register/csp_register.d \
./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.d \
./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.d 

OBJS += \
./ext_lib/CSP_1.6-main/src/func/func_register/bit_management.o \
./ext_lib/CSP_1.6-main/src/func/func_register/csp_register.o \
./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.o \
./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.o 


# Each subdirectory must supply rules for building sources it contributes
ext_lib/CSP_1.6-main/src/func/func_register/bit_management.o: C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/bit_management.c ext_lib/CSP_1.6-main/src/func/func_register/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
ext_lib/CSP_1.6-main/src/func/func_register/csp_register.o: C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/csp_register.c ext_lib/CSP_1.6-main/src/func/func_register/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.o: C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.c ext_lib/CSP_1.6-main/src/func/func_register/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.o: C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.c ext_lib/CSP_1.6-main/src/func/func_register/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ext_lib-2f-CSP_1-2e-6-2d-main-2f-src-2f-func-2f-func_register

clean-ext_lib-2f-CSP_1-2e-6-2d-main-2f-src-2f-func-2f-func_register:
	-$(RM) ./ext_lib/CSP_1.6-main/src/func/func_register/bit_management.cyclo ./ext_lib/CSP_1.6-main/src/func/func_register/bit_management.d ./ext_lib/CSP_1.6-main/src/func/func_register/bit_management.o ./ext_lib/CSP_1.6-main/src/func/func_register/bit_management.su ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register.cyclo ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register.d ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register.o ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register.su ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.cyclo ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.d ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.o ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service.su ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.cyclo ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.d ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.o ./ext_lib/CSP_1.6-main/src/func/func_register/csp_register_service_handler.su

.PHONY: clean-ext_lib-2f-CSP_1-2e-6-2d-main-2f-src-2f-func-2f-func_register

