################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Exercises/Badsort_ptr/badsort-ptr.c 

OBJS += \
./Exercises/Badsort_ptr/badsort-ptr.o 

C_DEPS += \
./Exercises/Badsort_ptr/badsort-ptr.d 


# Each subdirectory must supply rules for building sources it contributes
Exercises/Badsort_ptr/%.o: ../Exercises/Badsort_ptr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


