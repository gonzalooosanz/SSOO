################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mytar/mytar.c \
../Mytar/mytar_routines.c 

OBJS += \
./Mytar/mytar.o \
./Mytar/mytar_routines.o 

C_DEPS += \
./Mytar/mytar.d \
./Mytar/mytar_routines.d 


# Each subdirectory must supply rules for building sources it contributes
Mytar/%.o: ../Mytar/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


