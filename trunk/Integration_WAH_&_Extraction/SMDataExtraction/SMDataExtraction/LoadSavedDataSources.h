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
	__declspec(dllexport) LoadSavedDataSources::LoadSavedDataSources(string metaDataFile,string dataFile,long limit);
	__declspec(dllexport) DataSources* loadSavedEncodedData(bool limit = false);
	__declspec(dllexport) ~LoadSavedDataSources(void);
	WrapDataSource::DATASOURCE getDataSourceType(int sourceType);
	ATT_TYPE getAttType(int attType);
	vector<EncodedAttributeInfo*> loadCodedAttributes(string dsName,int rowCount,bool limit);

private:
	string _fileName;
	string _metaFile;
	long _rowLimit;
};
