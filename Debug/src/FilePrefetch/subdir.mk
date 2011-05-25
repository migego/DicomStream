################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FilePrefetch/FilePrefetchQueue.cpp 

OBJS += \
./src/FilePrefetch/FilePrefetchQueue.o 

CPP_DEPS += \
./src/FilePrefetch/FilePrefetchQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/FilePrefetch/%.o: ../src/FilePrefetch/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Users/aboxer/src/boost_1_46_1 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


