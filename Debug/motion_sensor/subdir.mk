################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../motion_sensor/hc_sr501.c 

OBJS += \
./motion_sensor/hc_sr501.o 

C_DEPS += \
./motion_sensor/hc_sr501.d 


# Each subdirectory must supply rules for building sources it contributes
motion_sensor/%.o motion_sensor/%.su: ../motion_sensor/%.c motion_sensor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../motion_sensor -I../lora_module -I../siren -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-motion_sensor

clean-motion_sensor:
	-$(RM) ./motion_sensor/hc_sr501.d ./motion_sensor/hc_sr501.o ./motion_sensor/hc_sr501.su

.PHONY: clean-motion_sensor

