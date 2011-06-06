/*
 * FileParser.cpp
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#include "FileParser.h"

#include "imebra.h"
#include <string>

using namespace std;
using namespace puntoexe;
using namespace puntoexe::imebra;

FileParser::FileParser() {
	// TODO Auto-generated constructor stub

}

FileParser::~FileParser() {
	// TODO Auto-generated destructor stub
}

void FileParser::parse(string fileName)
{
	// Open the file containing the dicom dataset
	ptr<puntoexe::stream> inputStream(new puntoexe::stream);
	inputStream->openFile(fileName, std::ios_base::in);

	// Connect a stream reader to the dicom stream. Several stream reader
	//  can share the same stream
	ptr<puntoexe::streamReader> reader(new streamReader(inputStream));

	// Get a codec factory and let it use the right codec to create a dataset
	//  from the input stream
	ptr<codecs::codecFactory> codecsFactory(codecs::codecFactory::getCodecFactory());
	ptr<dataSet> loadedDataSet = codecsFactory->load(reader, 2048);

	// Get the patient name
	wstring patientName(loadedDataSet->getUnicodeString(0x10, 0, 0x10, 0));

}
