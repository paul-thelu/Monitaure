################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Monitaure_Drivers/Src/bluetooth.c \
../Drivers/Monitaure_Drivers/Src/stm32l4xx_bme280.c \
../Drivers/Monitaure_Drivers/Src/stm32l4xx_bq27421.c \
../Drivers/Monitaure_Drivers/Src/stm32l4xx_is1678.c \
../Drivers/Monitaure_Drivers/Src/stm32l4xx_max86150.c \
../Drivers/Monitaure_Drivers/Src/stm32l4xx_ssd1351.c \
../Drivers/Monitaure_Drivers/Src/stm32l4xx_tmp007.c 

OBJS += \
./Drivers/Monitaure_Drivers/Src/bluetooth.o \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_bme280.o \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_bq27421.o \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_is1678.o \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_max86150.o \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_ssd1351.o \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_tmp007.o 

C_DEPS += \
./Drivers/Monitaure_Drivers/Src/bluetooth.d \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_bme280.d \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_bq27421.d \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_is1678.d \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_max86150.d \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_ssd1351.d \
./Drivers/Monitaure_Drivers/Src/stm32l4xx_tmp007.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Monitaure_Drivers/Src/bluetooth.o: ../Drivers/Monitaure_Drivers/Src/bluetooth.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/bluetooth.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/stm32l4xx_bme280.o: ../Drivers/Monitaure_Drivers/Src/stm32l4xx_bme280.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/stm32l4xx_bme280.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/stm32l4xx_bq27421.o: ../Drivers/Monitaure_Drivers/Src/stm32l4xx_bq27421.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/stm32l4xx_bq27421.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/stm32l4xx_is1678.o: ../Drivers/Monitaure_Drivers/Src/stm32l4xx_is1678.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/stm32l4xx_is1678.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/stm32l4xx_max86150.o: ../Drivers/Monitaure_Drivers/Src/stm32l4xx_max86150.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/stm32l4xx_max86150.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/stm32l4xx_ssd1351.o: ../Drivers/Monitaure_Drivers/Src/stm32l4xx_ssd1351.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/stm32l4xx_ssd1351.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Monitaure_Drivers/Src/stm32l4xx_tmp007.o: ../Drivers/Monitaure_Drivers/Src/stm32l4xx_tmp007.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L451xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Monitaure_Drivers/Src/stm32l4xx_tmp007.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

