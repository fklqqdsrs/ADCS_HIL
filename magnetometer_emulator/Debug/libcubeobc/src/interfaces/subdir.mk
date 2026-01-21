################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/cubeObc_canIfc.c \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/cubeObc_cspIfc.c \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/cubeObc_uartIfc.c 

OBJS += \
./libcubeobc/src/interfaces/cubeObc_canIfc.o \
./libcubeobc/src/interfaces/cubeObc_cspIfc.o \
./libcubeobc/src/interfaces/cubeObc_uartIfc.o 

C_DEPS += \
./libcubeobc/src/interfaces/cubeObc_canIfc.d \
./libcubeobc/src/interfaces/cubeObc_cspIfc.d \
./libcubeobc/src/interfaces/cubeObc_uartIfc.d 


# Each subdirectory must supply rules for building sources it contributes
libcubeobc/src/interfaces/cubeObc_canIfc.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/cubeObc_canIfc.c libcubeobc/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
libcubeobc/src/interfaces/cubeObc_cspIfc.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/cubeObc_cspIfc.c libcubeobc/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
libcubeobc/src/interfaces/cubeObc_uartIfc.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/cubeObc_uartIfc.c libcubeobc/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libcubeobc-2f-src-2f-interfaces

clean-libcubeobc-2f-src-2f-interfaces:
	-$(RM) ./libcubeobc/src/interfaces/cubeObc_canIfc.cyclo ./libcubeobc/src/interfaces/cubeObc_canIfc.d ./libcubeobc/src/interfaces/cubeObc_canIfc.o ./libcubeobc/src/interfaces/cubeObc_canIfc.su ./libcubeobc/src/interfaces/cubeObc_cspIfc.cyclo ./libcubeobc/src/interfaces/cubeObc_cspIfc.d ./libcubeobc/src/interfaces/cubeObc_cspIfc.o ./libcubeobc/src/interfaces/cubeObc_cspIfc.su ./libcubeobc/src/interfaces/cubeObc_uartIfc.cyclo ./libcubeobc/src/interfaces/cubeObc_uartIfc.d ./libcubeobc/src/interfaces/cubeObc_uartIfc.o ./libcubeobc/src/interfaces/cubeObc_uartIfc.su

.PHONY: clean-libcubeobc-2f-src-2f-interfaces

