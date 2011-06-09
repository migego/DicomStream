################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dicom/FileParser.cpp 

OBJS += \
./src/Dicom/FileParser.o 

CPP_DEPS += \
./src/Dicom/FileParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/Dicom/%.o: ../src/Dicom/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Users/aboxer/src/boost_1_46_1 -I"/Users/aboxer/src/imebra/project_files/library/imebra/include" -I"/Users/aboxer/src/imebra/project_files/library/base/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


