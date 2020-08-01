################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Monitaure_Drivers/Src/imu/bmi2.c \
../Drivers/Monitaure_Drivers/Src/imu/bmi270.c \
../Drivers/Monitaure_Drivers/Src/imu/bmi2_ois.c 

OBJS += \
./Drivers/Monitaure_Drivers/Src/imu/bmi2.o \
./Drivers/Monitaure_Drivers/Src/imu/bmi270.o \
./Drivers/Monitaure_Drivers/Src/imu/bmi2_ois.o 

C_DEPS += \
./Drivers/Monitaure_Drivers/Src/imu/bmi2.d \
./Drivers/Monitaure_Drivers/Src/imu/bmi270.d \
./Drivers/Monitaure_Drivers/Src/imu/bmi2_ois.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Monitaure_Drivers/Src/imu/bmi2.o: ../Drivers/Monitaure_Drivers/Src/imu/bmi2.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/imu/bmi2.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/imu/bmi270.o: ../Drivers/Monitaure_Drivers/Src/imu/bmi270.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/imu/bmi270.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/imu/bmi2_ois.o: ../Drivers/Monitaure_Drivers/Src/imu/bmi2_ois.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/imu/bmi2_ois.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

