################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/utils/comProtocols/fifo.c \
../source/utils/comProtocols/fifo16.c 

C_DEPS += \
./source/utils/comProtocols/fifo.d \
./source/utils/comProtocols/fifo16.d 

OBJS += \
./source/utils/comProtocols/fifo.o \
./source/utils/comProtocols/fifo16.o 


# Each subdirectory must supply rules for building sources it contributes
source/utils/comProtocols/%.o: ../source/utils/comProtocols/%.c source/utils/comProtocols/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXA153VLH -DCPU_MCXA153VLH_cm33_nodsp -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DMCUX_META_BUILD -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\source" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\CMSIS" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\CMSIS\m-profile" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\device" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\device\periph" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33+nodsp -c -ffunction-sections -fdata-sections -fno-builtin -u _print_float -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-utils-2f-comProtocols

clean-source-2f-utils-2f-comProtocols:
	-$(RM) ./source/utils/comProtocols/fifo.d ./source/utils/comProtocols/fifo.o ./source/utils/comProtocols/fifo16.d ./source/utils/comProtocols/fifo16.o

.PHONY: clean-source-2f-utils-2f-comProtocols

