################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../OSD/EntityLabeler.cpp \
../OSD/Label.cpp \
../OSD/SignalViewer.cpp 

OBJS += \
./OSD/EntityLabeler.o \
./OSD/Label.o \
./OSD/SignalViewer.o 

CPP_DEPS += \
./OSD/EntityLabeler.d \
./OSD/Label.d \
./OSD/SignalViewer.d 


# Each subdirectory must supply rules for building sources it contributes
OSD/%.o: ../OSD/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++14 -DGLM_FORCE_RADIANS -O0 -g3 -Wall -c -fmessage-length=0 -static-libstdc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


