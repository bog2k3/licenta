################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../entities/Box.cpp \
../entities/Entity.cpp \
../entities/StaticMesh.cpp 

OBJS += \
./entities/Box.o \
./entities/Entity.o \
./entities/StaticMesh.o 

CPP_DEPS += \
./entities/Box.d \
./entities/Entity.d \
./entities/StaticMesh.d 


# Each subdirectory must supply rules for building sources it contributes
entities/%.o: ../entities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++14 -DGLM_FORCE_RADIANS -O0 -g3 -Wall -c -fmessage-length=0 -static-libstdc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


