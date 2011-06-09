################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/Parse/stream.pb.cc 

OBJS += \
./src/Parse/stream.pb.o 

CC_DEPS += \
./src/Parse/stream.pb.d 


# Each subdirectory must supply rules for building sources it contributes
src/Parse/%.o: ../src/Parse/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Users/aboxer/src/boost_1_46_1 -I"/Users/aboxer/src/imebra/project_files/library/imebra/include" -I"/Users/aboxer/src/imebra/project_files/library/base/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


