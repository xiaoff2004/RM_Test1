################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/Chassis.c \
../bsp/Gimbal.c \
../bsp/MahonyAHRS.c \
../bsp/Shooter.c \
../bsp/imu.c \
../bsp/motor.c \
../bsp/pid.c \
../bsp/remote.c 

OBJS += \
./bsp/Chassis.o \
./bsp/Gimbal.o \
./bsp/MahonyAHRS.o \
./bsp/Shooter.o \
./bsp/imu.o \
./bsp/motor.o \
./bsp/pid.o \
./bsp/remote.o 

C_DEPS += \
./bsp/Chassis.d \
./bsp/Gimbal.d \
./bsp/MahonyAHRS.d \
./bsp/Shooter.d \
./bsp/imu.d \
./bsp/motor.d \
./bsp/pid.d \
./bsp/remote.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o bsp/%.su bsp/%.cyclo: ../bsp/%.c bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/keil MDK/clion_project/RM_Test1/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bsp

clean-bsp:
	-$(RM) ./bsp/Chassis.cyclo ./bsp/Chassis.d ./bsp/Chassis.o ./bsp/Chassis.su ./bsp/Gimbal.cyclo ./bsp/Gimbal.d ./bsp/Gimbal.o ./bsp/Gimbal.su ./bsp/MahonyAHRS.cyclo ./bsp/MahonyAHRS.d ./bsp/MahonyAHRS.o ./bsp/MahonyAHRS.su ./bsp/Shooter.cyclo ./bsp/Shooter.d ./bsp/Shooter.o ./bsp/Shooter.su ./bsp/imu.cyclo ./bsp/imu.d ./bsp/imu.o ./bsp/imu.su ./bsp/motor.cyclo ./bsp/motor.d ./bsp/motor.o ./bsp/motor.su ./bsp/pid.cyclo ./bsp/pid.d ./bsp/pid.o ./bsp/pid.su ./bsp/remote.cyclo ./bsp/remote.d ./bsp/remote.o ./bsp/remote.su

.PHONY: clean-bsp

