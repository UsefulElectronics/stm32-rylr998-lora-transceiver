################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modules/rylr998.c \
../modules/ulora.c 

OBJS += \
./modules/rylr998.o \
./modules/ulora.o 

C_DEPS += \
./modules/rylr998.d \
./modules/ulora.d 


# Each subdirectory must supply rules for building sources it contributes
modules/%.o modules/%.su: ../modules/%.c modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../modules -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-modules

clean-modules:
	-$(RM) ./modules/rylr998.d ./modules/rylr998.o ./modules/rylr998.su ./modules/ulora.d ./modules/ulora.o ./modules/ulora.su

.PHONY: clean-modules

