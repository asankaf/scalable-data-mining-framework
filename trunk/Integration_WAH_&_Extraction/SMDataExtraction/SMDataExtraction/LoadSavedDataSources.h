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
	__declspec(dllexport) LoadSavedDataSources(long limit,string directory_name,string meta_file){this->_saved_folder = directory_name; this->_metaFile = meta_file;this->_rowLimit = limit;}
	__declspec(dllexport) DataSources* loadSavedEncodedData(bool limit = false);
	__declspec(dllexport) DataSources* loadSavedEncodedDataFromMultipleFiles(bool limit = false);
	__declspec(dllexport) ~LoadSavedDataSources(void);
	WrapDataSource::DATASOURCE getDataSourceType(int sourceType);
	ATT_TYPE getAttType(int attType);
	vector<EncodedAttributeInfo*> loadCodedAttributes(string dsName,int rowCount,bool limit);
	vector<EncodedAttributeInfo*> loadCodedAttributesFromMultipleFiles(string dsName,int rowCount,bool limit);

private:
	string _fileName;
	string _metaFile;
	string _saved_folder;
	long _rowLimit;
	vector<string> saved_file_names;
};