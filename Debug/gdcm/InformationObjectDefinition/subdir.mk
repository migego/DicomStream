################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmDefinedTerms.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmDefs.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmEnumeratedValues.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmIOD.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmIODEntry.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmMacro.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmMacros.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmModule.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmModules.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmNestedModuleEntries.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmPatient.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmSeries.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmStudy.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmTable.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmTableReader.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmType.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmUsage.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmXMLDictReader.cxx \
/Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmXMLPrivateDictReader.cxx 

OBJS += \
./gdcm/InformationObjectDefinition/gdcmDefinedTerms.o \
./gdcm/InformationObjectDefinition/gdcmDefs.o \
./gdcm/InformationObjectDefinition/gdcmEnumeratedValues.o \
./gdcm/InformationObjectDefinition/gdcmIOD.o \
./gdcm/InformationObjectDefinition/gdcmIODEntry.o \
./gdcm/InformationObjectDefinition/gdcmMacro.o \
./gdcm/InformationObjectDefinition/gdcmMacros.o \
./gdcm/InformationObjectDefinition/gdcmModule.o \
./gdcm/InformationObjectDefinition/gdcmModules.o \
./gdcm/InformationObjectDefinition/gdcmNestedModuleEntries.o \
./gdcm/InformationObjectDefinition/gdcmPatient.o \
./gdcm/InformationObjectDefinition/gdcmSeries.o \
./gdcm/InformationObjectDefinition/gdcmStudy.o \
./gdcm/InformationObjectDefinition/gdcmTable.o \
./gdcm/InformationObjectDefinition/gdcmTableReader.o \
./gdcm/InformationObjectDefinition/gdcmType.o \
./gdcm/InformationObjectDefinition/gdcmUsage.o \
./gdcm/InformationObjectDefinition/gdcmXMLDictReader.o \
./gdcm/InformationObjectDefinition/gdcmXMLPrivateDictReader.o 

CXX_DEPS += \
./gdcm/InformationObjectDefinition/gdcmDefinedTerms.d \
./gdcm/InformationObjectDefinition/gdcmDefs.d \
./gdcm/InformationObjectDefinition/gdcmEnumeratedValues.d \
./gdcm/InformationObjectDefinition/gdcmIOD.d \
./gdcm/InformationObjectDefinition/gdcmIODEntry.d \
./gdcm/InformationObjectDefinition/gdcmMacro.d \
./gdcm/InformationObjectDefinition/gdcmMacros.d \
./gdcm/InformationObjectDefinition/gdcmModule.d \
./gdcm/InformationObjectDefinition/gdcmModules.d \
./gdcm/InformationObjectDefinition/gdcmNestedModuleEntries.d \
./gdcm/InformationObjectDefinition/gdcmPatient.d \
./gdcm/InformationObjectDefinition/gdcmSeries.d \
./gdcm/InformationObjectDefinition/gdcmStudy.d \
./gdcm/InformationObjectDefinition/gdcmTable.d \
./gdcm/InformationObjectDefinition/gdcmTableReader.d \
./gdcm/InformationObjectDefinition/gdcmType.d \
./gdcm/InformationObjectDefinition/gdcmUsage.d \
./gdcm/InformationObjectDefinition/gdcmXMLDictReader.d \
./gdcm/InformationObjectDefinition/gdcmXMLPrivateDictReader.d 


# Each subdirectory must supply rules for building sources it contributes
gdcm/InformationObjectDefinition/gdcmDefinedTerms.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmDefinedTerms.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmDefs.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmDefs.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmEnumeratedValues.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmEnumeratedValues.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmIOD.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmIOD.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmIODEntry.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmIODEntry.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmMacro.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmMacro.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmMacros.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmMacros.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmModule.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmModule.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmModules.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmModules.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmNestedModuleEntries.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmNestedModuleEntries.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmPatient.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmPatient.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmSeries.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmSeries.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmStudy.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmStudy.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmTable.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmTable.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmTableReader.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmTableReader.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmType.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmType.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmUsage.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmUsage.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmXMLDictReader.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmXMLDictReader.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gdcm/InformationObjectDefinition/gdcmXMLPrivateDictReader.o: /Users/aboxer/src/gdcm/Source/InformationObjectDefinition/gdcmXMLPrivateDictReader.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/aboxer/src/gdcm/Source/Attribute" -I"/Users/aboxer/src/gdcm/Source/Common" -I"/Users/aboxer/src/gdcm/Source/DataDictionary" -I"/Users/aboxer/src/gdcm/Source/DataStructureAndEncodingDefinition" -I"/Users/aboxer/src/gdcm/Source/InformationObjectDefinition" -I"/Users/aboxer/src/gdcm/Source/MediaStorageAndFileFormat" -I/Users/aboxer/src/gdcmbin/Source/Common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


