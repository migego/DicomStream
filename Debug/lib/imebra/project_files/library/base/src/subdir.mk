################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/imebra/project_files/library/base/src/baseObject.cpp \
../lib/imebra/project_files/library/base/src/baseStream.cpp \
../lib/imebra/project_files/library/base/src/charsetConversion.cpp \
../lib/imebra/project_files/library/base/src/criticalSection.cpp \
../lib/imebra/project_files/library/base/src/exception.cpp \
../lib/imebra/project_files/library/base/src/huffmanTable.cpp \
../lib/imebra/project_files/library/base/src/memory.cpp \
../lib/imebra/project_files/library/base/src/memoryStream.cpp \
../lib/imebra/project_files/library/base/src/stream.cpp \
../lib/imebra/project_files/library/base/src/streamController.cpp \
../lib/imebra/project_files/library/base/src/streamReader.cpp \
../lib/imebra/project_files/library/base/src/streamWriter.cpp \
../lib/imebra/project_files/library/base/src/thread.cpp 

OBJS += \
./lib/imebra/project_files/library/base/src/baseObject.o \
./lib/imebra/project_files/library/base/src/baseStream.o \
./lib/imebra/project_files/library/base/src/charsetConversion.o \
./lib/imebra/project_files/library/base/src/criticalSection.o \
./lib/imebra/project_files/library/base/src/exception.o \
./lib/imebra/project_files/library/base/src/huffmanTable.o \
./lib/imebra/project_files/library/base/src/memory.o \
./lib/imebra/project_files/library/base/src/memoryStream.o \
./lib/imebra/project_files/library/base/src/stream.o \
./lib/imebra/project_files/library/base/src/streamController.o \
./lib/imebra/project_files/library/base/src/streamReader.o \
./lib/imebra/project_files/library/base/src/streamWriter.o \
./lib/imebra/project_files/library/base/src/thread.o 

CPP_DEPS += \
./lib/imebra/project_files/library/base/src/baseObject.d \
./lib/imebra/project_files/library/base/src/baseStream.d \
./lib/imebra/project_files/library/base/src/charsetConversion.d \
./lib/imebra/project_files/library/base/src/criticalSection.d \
./lib/imebra/project_files/library/base/src/exception.d \
./lib/imebra/project_files/library/base/src/huffmanTable.d \
./lib/imebra/project_files/library/base/src/memory.d \
./lib/imebra/project_files/library/base/src/memoryStream.d \
./lib/imebra/project_files/library/base/src/stream.d \
./lib/imebra/project_files/library/base/src/streamController.d \
./lib/imebra/project_files/library/base/src/streamReader.d \
./lib/imebra/project_files/library/base/src/streamWriter.d \
./lib/imebra/project_files/library/base/src/thread.d 


# Each subdirectory must supply rules for building sources it contributes
lib/imebra/project_files/library/base/src/%.o: ../lib/imebra/project_files/library/base/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


