################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
/Users/aboxer/src/gdcm/Source/Common/gdcmASN1.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmBase64.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmByteSwap.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmCommand.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmCryptographicMessageSyntax.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmDataEvent.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmDeflateStream.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmDirectory.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmDummyValueGenerator.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmEvent.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmException.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmFilename.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmFilenameGenerator.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmMD5.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmObject.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmProgressEvent.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmSHA1.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmString.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmSubject.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmSwapCode.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmSystem.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmTerminal.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmTesting.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmTrace.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmUnpacker12Bits.cxx \
/Users/aboxer/src/gdcm/Source/Common/gdcmVersion.cxx 

OBJS += \
./gdcm/Common/gdcmASN1.o \
./gdcm/Common/gdcmBase64.o \
./gdcm/Common/gdcmByteSwap.o \
./gdcm/Common/gdcmCommand.o \
./gdcm/Common/gdcmCryptographicMessageSyntax.o \
./gdcm/Common/gdcmDataEvent.o \
./gdcm/Common/gdcmDeflateStream.o \
./gdcm/Common/gdcmDirectory.o \
./gdcm/Common/gdcmDummyValueGenerator.o \
./gdcm/Common/gdcmEvent.o \
./gdcm/Common/gdcmException.o \
./gdcm/Common/gdcmFilename.o \
./gdcm/Common/gdcmFilenameGenerator.o \
./gdcm/Common/gdcmMD5.o \
./gdcm/Common/gdcmObject.o \
./gdcm/Common/gdcmProgressEvent.o \
./gdcm/Common/gdcmSHA1.o \
./gdcm/Common/gdcmString.o \
./gdcm/Common/gdcmSubject.o \
./gdcm/Common/gdcmSwapCode.o \
./gdcm/Common/gdcmSystem.o \
./gdcm/Common/gdcmTerminal.o \
./gdcm/Common/gdcmTesting.o \
./gdcm/Common/gdcmTrace.o \
./gdcm/Common/gdcmUnpacker12Bits.o \
./gdcm/Common/gdcmVersion.o 

CXX_DEPS += \
./gdcm/Common/gdcmASN1.d \
./gdcm/Common/gdcmBase64.d \
./gdcm/Common/gdcmByteSwap.d \
./gdcm/Common/gdcmCommand.d \
./gdcm/Common/gdcmCryptographicMessageSyntax.d \
./gdcm/Common/gdcmDataEvent.d \
./gdcm/Common/gdcmDeflateStream.d \
./gdcm/Common/gdcmDirectory.d \
./gdcm/Common/gdcmDummyValueGenerator.d \
./gdcm/Common/gdcmEvent.d \
./gdcm/Common/gdcmException.d \
./gdcm/Common/gdcmFilename.d \
./gdcm/Common/gdcmFilenameGenerator.d \
./gdcm/Common/gdcmMD5.d \
./gdcm/Common/gdcmObject.d \
./gdcm/Common/gdcmProgressEvent.d \
./gdcm/Common/gdcmSHA1.d \
./gdcm/Common/gdcmString.d \
./gdcm/Common/gdcmSubject.d \
./gdcm/Common/gdcmSwapCode.d \
./gdcm/Common/gdcmSystem.d \
./gdcm/Common/gdcmTerminal.d \
./gdcm/Common/gdcmTesting.d \
./gdcm/Common/gdcmTrace.d \
./gdcm/Common/gdcmUnpacker12Bits.d \
./gdcm/Common/gdcmVersion.d 


# Each subdirectory must supply rules for building sources it contributes
gdcm/Common/gdcmASN1.o: /Users/aboxer/src/gdcm/Source/Common/gdcmASN1.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmBase64.o: /Users/aboxer/src/gdcm/Source/Common/gdcmBase64.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmByteSwap.o: /Users/aboxer/src/gdcm/Source/Common/gdcmByteSwap.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmCommand.o: /Users/aboxer/src/gdcm/Source/Common/gdcmCommand.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmCryptographicMessageSyntax.o: /Users/aboxer/src/gdcm/Source/Common/gdcmCryptographicMessageSyntax.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmDataEvent.o: /Users/aboxer/src/gdcm/Source/Common/gdcmDataEvent.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmDeflateStream.o: /Users/aboxer/src/gdcm/Source/Common/gdcmDeflateStream.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmDirectory.o: /Users/aboxer/src/gdcm/Source/Common/gdcmDirectory.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmDummyValueGenerator.o: /Users/aboxer/src/gdcm/Source/Common/gdcmDummyValueGenerator.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmEvent.o: /Users/aboxer/src/gdcm/Source/Common/gdcmEvent.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmException.o: /Users/aboxer/src/gdcm/Source/Common/gdcmException.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmFilename.o: /Users/aboxer/src/gdcm/Source/Common/gdcmFilename.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmFilenameGenerator.o: /Users/aboxer/src/gdcm/Source/Common/gdcmFilenameGenerator.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmMD5.o: /Users/aboxer/src/gdcm/Source/Common/gdcmMD5.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmObject.o: /Users/aboxer/src/gdcm/Source/Common/gdcmObject.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmProgressEvent.o: /Users/aboxer/src/gdcm/Source/Common/gdcmProgressEvent.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmSHA1.o: /Users/aboxer/src/gdcm/Source/Common/gdcmSHA1.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmString.o: /Users/aboxer/src/gdcm/Source/Common/gdcmString.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmSubject.o: /Users/aboxer/src/gdcm/Source/Common/gdcmSubject.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmSwapCode.o: /Users/aboxer/src/gdcm/Source/Common/gdcmSwapCode.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmSystem.o: /Users/aboxer/src/gdcm/Source/Common/gdcmSystem.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmTerminal.o: /Users/aboxer/src/gdcm/Source/Common/gdcmTerminal.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmTesting.o: /Users/aboxer/src/gdcm/Source/Common/gdcmTesting.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmTrace.o: /Users/aboxer/src/gdcm/Source/Common/gdcmTrace.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmUnpacker12Bits.o: /Users/aboxer/src/gdcm/Source/Common/gdcmUnpacker12Bits.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/Common/gdcmVersion.o: /Users/aboxer/src/gdcm/Source/Common/gdcmVersion.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


