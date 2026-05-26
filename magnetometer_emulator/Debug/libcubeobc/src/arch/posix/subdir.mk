################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/arch/posix/cubeObc_time.c 

OBJS += \
./libcubeobc/src/arch/posix/cubeObc_time.o 

C_DEPS += \
./libcubeobc/src/arch/posix/cubeObc_time.d 


# Each subdirectory must supply rules for building sources it contributes
libcubeobc/src/arch/posix/cubeObc_time.o: C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/arch/posix/cubeObc_time.c libcubeobc/src/arch/posix/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Downloads/Git-test-DEMO/Git-test-DEMO/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libcubeobc-2f-src-2f-arch-2f-posix

clean-libcubeobc-2f-src-2f-arch-2f-posix:
	-$(RM) ./libcubeobc/src/arch/posix/cubeObc_time.cyclo ./libcubeobc/src/arch/posix/cubeObc_time.d ./libcubeobc/src/arch/posix/cubeObc_time.o ./libcubeobc/src/arch/posix/cubeObc_time.su

.PHONY: clean-libcubeobc-2f-src-2f-arch-2f-posix

