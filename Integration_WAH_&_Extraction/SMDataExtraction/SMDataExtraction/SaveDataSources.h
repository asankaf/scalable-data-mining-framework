#pragma once
#include "boost/dynamic_bitset.hpp"
#include "tinyxml.h"
#include "WrapDataSource.h"
#include "DataSources.h"
#include "SEEDMinerExceptions.h"
#include "ConfigurationReader.h"

#include <string>
#include <direct.h>


/************************************************************************/
/*    Class  :LoadSavedDataSources.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.05.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :Class to Serialize the already filled data source as xml
/*    Version: 
/************************************************************************/

class DataSourceSerialization{
public:

	#pragma region Constructors & Destructor

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
	__declspec(dllexport) ~DataSourceSerialization(){}

	#pragma endregion Constructors & Destructor

	/**	Method to serialize the data source in the provided xml file, a parameter can be provided to*/
	/**	save the encoded data in multiple attribute data files*/
	__declspec(dllexport) bool serializeDataSource(bool saveAttsInDifferentFiles = false) throw(error_saving_encoded_data);
	
	/**	Method to save coded attribute data in the provided single data file*/
	void saveCodedData();

	/** Method to save coded attribute data in the provided multiple data file*/
	void saveCodedDataInDifferentFiles(vector<string> attFiles);

	/**	Method to convert a boolean vector to a dynamic_bitset*/
	dynamic_bitset<> signMapAsLong(vector<bool> signMap);

	/**	Method to generate the attribute file names by looking at the data source*/
	vector<string> getAttributeFileNames(string dsName);

private:
	DataSources *_dataSources;
	std::string _xmlFileName;
	std::string _attDataFileName;
	std::string _dataFileName;
};