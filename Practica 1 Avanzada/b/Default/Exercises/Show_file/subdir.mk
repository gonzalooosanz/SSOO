################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Exercises/Show_file/show_file.c 

OBJS += \
./Exercises/Show_file/show_file.o 

C_DEPS += \
./Exercises/Show_file/show_file.d 


# Each subdirectory must supply rules for building sources it contributes
Exercises/Show_file/%.o: ../Exercises/Show_file/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


