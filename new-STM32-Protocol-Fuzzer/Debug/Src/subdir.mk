################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/delay.c \
../Src/fuzzer.c \
../Src/main.c \
../Src/protocol.c \
../Src/uart.c 

OBJS += \
./Src/delay.o \
./Src/fuzzer.o \
./Src/main.o \
./Src/protocol.o \
./Src/uart.o 

C_DEPS += \
./Src/delay.d \
./Src/fuzzer.d \
./Src/main.d \
./Src/protocol.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/delay.cyclo ./Src/delay.d ./Src/delay.o ./Src/delay.su ./Src/fuzzer.cyclo ./Src/fuzzer.d ./Src/fuzzer.o ./Src/fuzzer.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/protocol.cyclo ./Src/protocol.d ./Src/protocol.o ./Src/protocol.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

