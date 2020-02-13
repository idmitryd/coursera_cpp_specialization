################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/server_stats.cpp \
../src/stats.cpp 

OBJS += \
./src/server_stats.o \
./src/stats.o 

CPP_DEPS += \
./src/server_stats.d \
./src/stats.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1z -I"/home/dmitryd/coursera/eclipse-workspace/red_belt/include" -I"/home/dmitryd/coursera/eclipse-workspace/red_belt/server_stats/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


