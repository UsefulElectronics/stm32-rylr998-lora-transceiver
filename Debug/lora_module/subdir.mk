################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lora_module/rylr998.c \
../lora_module/ulora.c 

OBJS += \
./lora_module/rylr998.o \
./lora_module/ulora.o 

C_DEPS += \
./lora_module/rylr998.d \
./lora_module/ulora.d 


# Each subdirectory must supply rules for building sources it contributes
lora_module/%.o lora_module/%.su: ../lora_module/%.c lora_module/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../motion_sensor -I../lora_module -I../siren -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lora_module

clean-lora_module:
	-$(RM) ./lora_module/rylr998.d ./lora_module/rylr998.o ./lora_module/rylr998.su ./lora_module/ulora.d ./lora_module/ulora.o ./lora_module/ulora.su

.PHONY: clean-lora_module

