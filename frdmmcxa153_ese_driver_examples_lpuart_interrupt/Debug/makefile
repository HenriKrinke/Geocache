################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/utils/temperatureSensor/subdir.mk
-include source/utils/solenoid/subdir.mk
-include source/utils/sdCard/FatFs/subdir.mk
-include source/utils/sdCard/subdir.mk
-include source/utils/lcdScreen/subdir.mk
-include source/utils/joystick/subdir.mk
-include source/utils/gps/subdir.mk
-include source/utils/comProtocols/Lpuart/subdir.mk
-include source/utils/comProtocols/Lpi2c/subdir.mk
-include source/utils/comProtocols/GPIO/subdir.mk
-include source/utils/comProtocols/subdir.mk
-include source/utils/colourSensor/subdir.mk
-include source/utils/buttonMatrix/subdir.mk
-include source/utils/subdir.mk
-include source/games/levelTwo/subdir.mk
-include source/games/levelThree/subdir.mk
-include source/games/levelOne/subdir.mk
-include source/games/levelFour/subdir.mk
-include source/games/gpsGame/subdir.mk
-include source/subdir.mk
-include device/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := frdmmcxa153_ese_driver_examples_lpuart_interrupt
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: frdmmcxa153_ese_driver_examples_lpuart_interrupt.axf

# Tool invocations
frdmmcxa153_ese_driver_examples_lpuart_interrupt.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker -Map="frdmmcxa153_ese_driver_examples_lpuart_interrupt.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m33+nodsp -mthumb -T frdmmcxa153_ese_driver_examples_lpuart_interrupt_Debug.ld -o "frdmmcxa153_ese_driver_examples_lpuart_interrupt.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) frdmmcxa153_ese_driver_examples_lpuart_interrupt.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "frdmmcxa153_ese_driver_examples_lpuart_interrupt.axf"; # arm-none-eabi-objcopy -v -O binary "frdmmcxa153_ese_driver_examples_lpuart_interrupt.axf" "frdmmcxa153_ese_driver_examples_lpuart_interrupt.bin" ; # checksum -p MCXA153 -d "frdmmcxa153_ese_driver_examples_lpuart_interrupt.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
