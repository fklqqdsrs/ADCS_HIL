################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_clock.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_malloc.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_queue.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_semaphore.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_system.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_thread.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_time.c 

C_DEPS += \
./CSP_1.6/src/arch/freertos/csp_clock.d \
./CSP_1.6/src/arch/freertos/csp_malloc.d \
./CSP_1.6/src/arch/freertos/csp_queue.d \
./CSP_1.6/src/arch/freertos/csp_semaphore.d \
./CSP_1.6/src/arch/freertos/csp_system.d \
./CSP_1.6/src/arch/freertos/csp_thread.d \
./CSP_1.6/src/arch/freertos/csp_time.d 

OBJS += \
./CSP_1.6/src/arch/freertos/csp_clock.o \
./CSP_1.6/src/arch/freertos/csp_malloc.o \
./CSP_1.6/src/arch/freertos/csp_queue.o \
./CSP_1.6/src/arch/freertos/csp_semaphore.o \
./CSP_1.6/src/arch/freertos/csp_system.o \
./CSP_1.6/src/arch/freertos/csp_thread.o \
./CSP_1.6/src/arch/freertos/csp_time.o 


# Each subdirectory must supply rules for building sources it contributes
CSP_1.6/src/arch/freertos/csp_clock.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_clock.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/arch/freertos/csp_malloc.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_malloc.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/arch/freertos/csp_queue.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_queue.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/arch/freertos/csp_semaphore.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_semaphore.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/arch/freertos/csp_system.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_system.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/arch/freertos/csp_thread.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_thread.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/arch/freertos/csp_time.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/arch/freertos/csp_time.c CSP_1.6/src/arch/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CSP_1-2e-6-2f-src-2f-arch-2f-freertos

clean-CSP_1-2e-6-2f-src-2f-arch-2f-freertos:
	-$(RM) ./CSP_1.6/src/arch/freertos/csp_clock.cyclo ./CSP_1.6/src/arch/freertos/csp_clock.d ./CSP_1.6/src/arch/freertos/csp_clock.o ./CSP_1.6/src/arch/freertos/csp_clock.su ./CSP_1.6/src/arch/freertos/csp_malloc.cyclo ./CSP_1.6/src/arch/freertos/csp_malloc.d ./CSP_1.6/src/arch/freertos/csp_malloc.o ./CSP_1.6/src/arch/freertos/csp_malloc.su ./CSP_1.6/src/arch/freertos/csp_queue.cyclo ./CSP_1.6/src/arch/freertos/csp_queue.d ./CSP_1.6/src/arch/freertos/csp_queue.o ./CSP_1.6/src/arch/freertos/csp_queue.su ./CSP_1.6/src/arch/freertos/csp_semaphore.cyclo ./CSP_1.6/src/arch/freertos/csp_semaphore.d ./CSP_1.6/src/arch/freertos/csp_semaphore.o ./CSP_1.6/src/arch/freertos/csp_semaphore.su ./CSP_1.6/src/arch/freertos/csp_system.cyclo ./CSP_1.6/src/arch/freertos/csp_system.d ./CSP_1.6/src/arch/freertos/csp_system.o ./CSP_1.6/src/arch/freertos/csp_system.su ./CSP_1.6/src/arch/freertos/csp_thread.cyclo ./CSP_1.6/src/arch/freertos/csp_thread.d ./CSP_1.6/src/arch/freertos/csp_thread.o ./CSP_1.6/src/arch/freertos/csp_thread.su ./CSP_1.6/src/arch/freertos/csp_time.cyclo ./CSP_1.6/src/arch/freertos/csp_time.d ./CSP_1.6/src/arch/freertos/csp_time.o ./CSP_1.6/src/arch/freertos/csp_time.su

.PHONY: clean-CSP_1-2e-6-2f-src-2f-arch-2f-freertos

