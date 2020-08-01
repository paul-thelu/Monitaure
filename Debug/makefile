################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Drivers/Monitaure_Drivers/Src/imu/subdir.mk
-include Drivers/Monitaure_Drivers/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Monitaure.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Monitaure.list \

OBJCOPY_BIN += \
Monitaure.bin \


# All Target
all: Monitaure.elf secondary-outputs

# Tool invocations
Monitaure.elf: $(OBJS) $(USER_OBJS) C:\Users\PaulT\OneDrive\Electronique\Monitaure\Firmware\Monitaure\STM32L451CEUX_FLASH.ld
	arm-none-eabi-gcc -o "Monitaure.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\PaulT\OneDrive\Electronique\Monitaure\Firmware\Monitaure\STM32L451CEUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Monitaure.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Monitaure.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Monitaure.list"
	@echo 'Finished building: $@'
	@echo ' '

Monitaure.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Monitaure.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
