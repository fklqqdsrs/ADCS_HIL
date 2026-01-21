################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f767zgtx.s 

S_DEPS += \
./Core/Startup/startup_stm32f767zgtx.d 

OBJS += \
./Core/Startup/startup_stm32f767zgtx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/CSP_1.6-main" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/include" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/RTOS_LIB" -I"C:/Users/Chinnapop/Documents/TSC1A_EM/ext_lib/REG_MAP" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f767zgtx.d ./Core/Startup/startup_stm32f767zgtx.o

.PHONY: clean-Core-2f-Startup

