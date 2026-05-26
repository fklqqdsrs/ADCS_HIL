################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_can.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_can_pbuf.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_i2c.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_kiss.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_lo.c 

C_DEPS += \
./CSP_1.6/src/interfaces/csp_if_can.d \
./CSP_1.6/src/interfaces/csp_if_can_pbuf.d \
./CSP_1.6/src/interfaces/csp_if_i2c.d \
./CSP_1.6/src/interfaces/csp_if_kiss.d \
./CSP_1.6/src/interfaces/csp_if_lo.d 

OBJS += \
./CSP_1.6/src/interfaces/csp_if_can.o \
./CSP_1.6/src/interfaces/csp_if_can_pbuf.o \
./CSP_1.6/src/interfaces/csp_if_i2c.o \
./CSP_1.6/src/interfaces/csp_if_kiss.o \
./CSP_1.6/src/interfaces/csp_if_lo.o 


# Each subdirectory must supply rules for building sources it contributes
CSP_1.6/src/interfaces/csp_if_can.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_can.c CSP_1.6/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/interfaces/csp_if_can_pbuf.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_can_pbuf.c CSP_1.6/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/interfaces/csp_if_i2c.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_i2c.c CSP_1.6/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/interfaces/csp_if_kiss.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_kiss.c CSP_1.6/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/interfaces/csp_if_lo.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/interfaces/csp_if_lo.c CSP_1.6/src/interfaces/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CSP_1-2e-6-2f-src-2f-interfaces

clean-CSP_1-2e-6-2f-src-2f-interfaces:
	-$(RM) ./CSP_1.6/src/interfaces/csp_if_can.cyclo ./CSP_1.6/src/interfaces/csp_if_can.d ./CSP_1.6/src/interfaces/csp_if_can.o ./CSP_1.6/src/interfaces/csp_if_can.su ./CSP_1.6/src/interfaces/csp_if_can_pbuf.cyclo ./CSP_1.6/src/interfaces/csp_if_can_pbuf.d ./CSP_1.6/src/interfaces/csp_if_can_pbuf.o ./CSP_1.6/src/interfaces/csp_if_can_pbuf.su ./CSP_1.6/src/interfaces/csp_if_i2c.cyclo ./CSP_1.6/src/interfaces/csp_if_i2c.d ./CSP_1.6/src/interfaces/csp_if_i2c.o ./CSP_1.6/src/interfaces/csp_if_i2c.su ./CSP_1.6/src/interfaces/csp_if_kiss.cyclo ./CSP_1.6/src/interfaces/csp_if_kiss.d ./CSP_1.6/src/interfaces/csp_if_kiss.o ./CSP_1.6/src/interfaces/csp_if_kiss.su ./CSP_1.6/src/interfaces/csp_if_lo.cyclo ./CSP_1.6/src/interfaces/csp_if_lo.d ./CSP_1.6/src/interfaces/csp_if_lo.o ./CSP_1.6/src/interfaces/csp_if_lo.su

.PHONY: clean-CSP_1-2e-6-2f-src-2f-interfaces

