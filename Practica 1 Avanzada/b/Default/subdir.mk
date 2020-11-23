################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../archi.c \
../archi_n.c \
../archi_n2.c \
../badsort.c \
../greetings.c 

OBJS += \
./archi.o \
./archi_n.o \
./archi_n2.o \
./badsort.o \
./greetings.o 

C_DEPS += \
./archi.d \
./archi_n.d \
./archi_n2.d \
./badsort.d \
./greetings.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


