#pragma once
#include "boost/dynamic_bitset.hpp"
#include "tinyxml.h"
#include "WrapDataSource.h"
#include "DataSources.h"

#include <string>

class DataSourceSerialization{
public:
	__declspec(dllexport) DataSourceSerialization(DataSources *ds,std::string xmlFile,std::string dataFile){this->_dataSources = ds;this->_xmlFileName = xmlFile;this->_dataFileName = dataFile;}
	__declspec(dllexport) bool serializeDataSource();
	void saveCodedData();
	dynamic_bitset<> signMapAsLong(vector<bool> signMap);
	__declspec(dllexport) ~DataSourceSerialization(){}
private:
	DataSources *_dataSources;
	std::string _xmlFileName;
	std::string _dataFileName;
};