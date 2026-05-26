################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/crypto/csp_hmac.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/crypto/csp_sha1.c \
C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/crypto/csp_xtea.c 

C_DEPS += \
./CSP_1.6/src/crypto/csp_hmac.d \
./CSP_1.6/src/crypto/csp_sha1.d \
./CSP_1.6/src/crypto/csp_xtea.d 

OBJS += \
./CSP_1.6/src/crypto/csp_hmac.o \
./CSP_1.6/src/crypto/csp_sha1.o \
./CSP_1.6/src/crypto/csp_xtea.o 


# Each subdirectory must supply rules for building sources it contributes
CSP_1.6/src/crypto/csp_hmac.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/crypto/csp_hmac.c CSP_1.6/src/crypto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/crypto/csp_sha1.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/crypto/csp_sha1.c CSP_1.6/src/crypto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
CSP_1.6/src/crypto/csp_xtea.o: C:/Users/thaib/OneDrive/Documents/CSP_1.6/src/crypto/csp_xtea.c CSP_1.6/src/crypto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/thaib/OneDrive/Documents/CSP_1.6" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CSP_1-2e-6-2f-src-2f-crypto

clean-CSP_1-2e-6-2f-src-2f-crypto:
	-$(RM) ./CSP_1.6/src/crypto/csp_hmac.cyclo ./CSP_1.6/src/crypto/csp_hmac.d ./CSP_1.6/src/crypto/csp_hmac.o ./CSP_1.6/src/crypto/csp_hmac.su ./CSP_1.6/src/crypto/csp_sha1.cyclo ./CSP_1.6/src/crypto/csp_sha1.d ./CSP_1.6/src/crypto/csp_sha1.o ./CSP_1.6/src/crypto/csp_sha1.su ./CSP_1.6/src/crypto/csp_xtea.cyclo ./CSP_1.6/src/crypto/csp_xtea.d ./CSP_1.6/src/crypto/csp_xtea.o ./CSP_1.6/src/crypto/csp_xtea.su

.PHONY: clean-CSP_1-2e-6-2f-src-2f-crypto

