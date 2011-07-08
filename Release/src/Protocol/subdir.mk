################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/Protocol/stream.pb.cc 

OBJS += \
./src/Protocol/stream.pb.o 

CC_DEPS += \
./src/Protocol/stream.pb.d 


# Each subdirectory must supply rules for building sources it contributes
src/Protocol/%.o: ../src/Protocol/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Users/aboxer/src/boost_1_46_1 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


