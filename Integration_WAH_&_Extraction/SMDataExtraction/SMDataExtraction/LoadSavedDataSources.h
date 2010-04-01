#pragma once
#include "DataSources.h"
#include "WrapDataSource.h"
#include "AttributeType.h"
#include <string>

using namespace std;

class LoadSavedDataSources
{
public:
	__declspec(dllexport) LoadSavedDataSources(string metaDataFile,string dataFile);
	__declspec(dllexport) DataSources* loadSavedEncodedData();
	__declspec(dllexport) ~LoadSavedDataSources(void);
	WrapDataSource::DATASOURCE getDataSourceType(int sourceType);
	ATT_TYPE getAttType(int attType);
	vector<EncodedAttributeInfo*> loadCodedAttributes(string dsName,int rowCount);

private:
	string _fileName;
	string _metaFile;
};
