################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/interfaces/subdir.mk
-include ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/uart/subdir.mk
-include ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/drivers/can/subdir.mk
-include ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/arch/cmsis-rtos-v2/subdir.mk
-include ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/src/subdir.mk
-include ext_lib/sw-bundle-master-v4.3.0.16/libcubeobc/api/src/subdir.mk
-include ext_lib/CSP_1.6-main/src/transport/subdir.mk
-include ext_lib/CSP_1.6-main/src/rtable/subdir.mk
-include ext_lib/CSP_1.6-main/src/interfaces/subdir.mk
-include ext_lib/CSP_1.6-main/src/func/func_timesync/subdir.mk
-include ext_lib/CSP_1.6-main/src/func/func_ssdv/subdir.mk
-include ext_lib/CSP_1.6-main/src/func/func_register/subdir.mk
-include ext_lib/CSP_1.6-main/src/func/func_chicken/subdir.mk
-include ext_lib/CSP_1.6-main/src/func/func_adcs/subdir.mk
-include ext_lib/CSP_1.6-main/src/drivers/usart/subdir.mk
-include ext_lib/CSP_1.6-main/src/drivers/radio/subdir.mk
-include ext_lib/CSP_1.6-main/src/drivers/i2c/subdir.mk
-include ext_lib/CSP_1.6-main/src/drivers/can_stm/subdir.mk
-include ext_lib/CSP_1.6-main/src/crypto/subdir.mk
-include ext_lib/CSP_1.6-main/src/arch/freertos/subdir.mk
-include ext_lib/CSP_1.6-main/src/arch/subdir.mk
-include ext_lib/CSP_1.6-main/src/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := RTOS_TEST
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
RTOS_TEST.elf \

MAP_FILES += \
RTOS_TEST.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
RTOS_TEST.list \


# All Target
all: main-build

# Main-build Target
main-build: RTOS_TEST.elf secondary-outputs

# Tool invocations
RTOS_TEST.elf RTOS_TEST.map: $(OBJS) $(USER_OBJS) C:\Users\Chinnapop\Documents\TSC1A_EM\Functional_test\RTOS_TEST\STM32F767ZGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "RTOS_TEST.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\Chinnapop\Documents\TSC1A_EM\Functional_test\RTOS_TEST\STM32F767ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="RTOS_TEST.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

RTOS_TEST.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "RTOS_TEST.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) RTOS_TEST.elf RTOS_TEST.list RTOS_TEST.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
