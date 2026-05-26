################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.c \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/examples/cubeObc_example_eventDownload.c \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/examples/cubeObc_example_passthrough.c 

OBJS += \
./libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.o \
./libcubeobc/examples/cubeObc_example_eventDownload.o \
./libcubeobc/examples/cubeObc_example_passthrough.o 

C_DEPS += \
./libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.d \
./libcubeobc/examples/cubeObc_example_eventDownload.d \
./libcubeobc/examples/cubeObc_example_passthrough.d 


# Each subdirectory must supply rules for building sources it contributes
libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.c libcubeobc/examples/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
libcubeobc/examples/cubeObc_example_eventDownload.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/examples/cubeObc_example_eventDownload.c libcubeobc/examples/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
libcubeobc/examples/cubeObc_example_passthrough.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/examples/cubeObc_example_passthrough.c libcubeobc/examples/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libcubeobc-2f-examples

clean-libcubeobc-2f-examples:
	-$(RM) ./libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.cyclo ./libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.d ./libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.o ./libcubeobc/examples/cubeObc_example_cubeSpaceFileUpload.su ./libcubeobc/examples/cubeObc_example_eventDownload.cyclo ./libcubeobc/examples/cubeObc_example_eventDownload.d ./libcubeobc/examples/cubeObc_example_eventDownload.o ./libcubeobc/examples/cubeObc_example_eventDownload.su ./libcubeobc/examples/cubeObc_example_passthrough.cyclo ./libcubeobc/examples/cubeObc_example_passthrough.d ./libcubeobc/examples/cubeObc_example_passthrough.o ./libcubeobc/examples/cubeObc_example_passthrough.su

.PHONY: clean-libcubeobc-2f-examples

