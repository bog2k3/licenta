################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../entities/EStaticMesh.cpp \
../entities/Entity.cpp 

OBJS += \
./entities/EStaticMesh.o \
./entities/Entity.o 

CPP_DEPS += \
./entities/EStaticMesh.d \
./entities/Entity.d 


# Each subdirectory must supply rules for building sources it contributes
entities/%.o: ../entities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++14 -DGLM_FORCE_RADIANS -O0 -g3 -Wall -c -fmessage-length=0 -static-libstdc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


