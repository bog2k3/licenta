################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Infrastructure.cpp \
../SessionManager.cpp \
../World.cpp \
../main.cpp 

OBJS += \
./Infrastructure.o \
./SessionManager.o \
./World.o \
./main.o 

CPP_DEPS += \
./Infrastructure.d \
./SessionManager.d \
./World.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++14 -DGLM_FORCE_RADIANS -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


