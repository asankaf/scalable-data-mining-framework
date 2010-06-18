#pragma once
#include "boost/dynamic_bitset.hpp"
#include "tinyxml.h"
#include "WrapDataSource.h"
#include "DataSources.h"

#include <string>
#include "ConfigurationReader.h"
#include <direct.h>
#include "SEEDMinerExceptions.h"

class DataSourceSerialization{
public:
	__declspec(dllexport) DataSourceSerialization(DataSources *ds,std::string xmlFile,std::string dataFile){
		this->_dataSources = ds; 
		this->_attDataFileName = dataFile; 
		this->_xmlFileName = xmlFile;
		string report_folder = ConfigurationReader::ReadConfiguration(ConfigurationReader::configutation::SAVE_DATA_FOLDER);
		if (chdir(report_folder.c_str()) == -1)
		{
			mkdir(report_folder.c_str());
		}
		this->_dataFileName = report_folder + dataFile;
	}
	__declspec(dllexport) bool serializeDataSource(bool saveAttsInDifferentFiles = false) throw(error_saving_encoded_data);
	__declspec(dllexport) ~DataSourceSerialization(){}

	void saveCodedData();
	void saveCodedDataInDifferentFiles(vector<string> attFiles);
	dynamic_bitset<> signMapAsLong(vector<bool> signMap);
	vector<string> getAttributeFileNames(string dsName);
private:
	DataSources *_dataSources;
	std::string _xmlFileName;
	std::string _attDataFileName;
	std::string _dataFileName;
};