################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AZTEC_relays.c \
../src/ball_count_unit.c \
../src/bonus_unit.c \
../src/coin_unit.c \
../src/credit_unit.c \
../src/main.c \
../src/player_unit.c \
../src/queue.c \
../src/relay.c \
../src/score_motor.c 

OBJS += \
./src/AZTEC_relays.o \
./src/ball_count_unit.o \
./src/bonus_unit.o \
./src/coin_unit.o \
./src/credit_unit.o \
./src/main.o \
./src/player_unit.o \
./src/queue.o \
./src/relay.o \
./src/score_motor.o 

C_DEPS += \
./src/AZTEC_relays.d \
./src/ball_count_unit.d \
./src/bonus_unit.d \
./src/coin_unit.d \
./src/credit_unit.d \
./src/main.d \
./src/player_unit.d \
./src/queue.d \
./src/relay.d \
./src/score_motor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/alex/workspace/aztec/include" -Wall -g2 -gstabs -O0 -O0 -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


