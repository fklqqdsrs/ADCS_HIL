################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/drivers/i2c/f722ze_i2c.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/drivers/i2c/g431kb_i2c.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/drivers/i2c/l496zg-p_i2c.c 

C_DEPS += \
./CSP_1.6/src/drivers/i2c/f722ze_i2c.d \
./CSP_1.6/src/drivers/i2c/g431kb_i2c.d \
./CSP_1.6/src/drivers/i2c/l496zg-p_i2c.d 

OBJS += \
./CSP_1.6/src/drivers/i2c/f722ze_i2c.o \
./CSP_1.6/src/drivers/i2c/g431kb_i2c.o \
./CSP_1.6/src/drivers/i2c/l496zg-p_i2c.o 


# Each subdirectory must supply rules for building sources it contributes
CSP_1.6/src/drivers/i2c/f722ze_i2c.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/drivers/i2c/f722ze_i2c.c CSP_1.6/src/drivers/i2c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/drivers/i2c/g431kb_i2c.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/drivers/i2c/g431kb_i2c.c CSP_1.6/src/drivers/i2c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/drivers/i2c/l496zg-p_i2c.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/drivers/i2c/l496zg-p_i2c.c CSP_1.6/src/drivers/i2c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CSP_1-2e-6-2f-src-2f-drivers-2f-i2c

clean-CSP_1-2e-6-2f-src-2f-drivers-2f-i2c:
	-$(RM) ./CSP_1.6/src/drivers/i2c/f722ze_i2c.cyclo ./CSP_1.6/src/drivers/i2c/f722ze_i2c.d ./CSP_1.6/src/drivers/i2c/f722ze_i2c.o ./CSP_1.6/src/drivers/i2c/f722ze_i2c.su ./CSP_1.6/src/drivers/i2c/g431kb_i2c.cyclo ./CSP_1.6/src/drivers/i2c/g431kb_i2c.d ./CSP_1.6/src/drivers/i2c/g431kb_i2c.o ./CSP_1.6/src/drivers/i2c/g431kb_i2c.su ./CSP_1.6/src/drivers/i2c/l496zg-p_i2c.cyclo ./CSP_1.6/src/drivers/i2c/l496zg-p_i2c.d ./CSP_1.6/src/drivers/i2c/l496zg-p_i2c.o ./CSP_1.6/src/drivers/i2c/l496zg-p_i2c.su

.PHONY: clean-CSP_1-2e-6-2f-src-2f-drivers-2f-i2c

