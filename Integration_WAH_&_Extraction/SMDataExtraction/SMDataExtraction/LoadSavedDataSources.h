#pragma once
#include "DataSources.h"
#include "WrapDataSource.h"
#include "AttributeType.h"
#include "SEEDMinerExceptions.h"

#include <string>

using namespace std;

/************************************************************************/
/*    Class  :LoadSavedDataSources.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.05.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :Class to Deserialize the already saved data source as xml
/*    Version: 
/************************************************************************/

class LoadSavedDataSources
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) LoadSavedDataSources(string metaDataFile,string dataFile);
	__declspec(dllexport) LoadSavedDataSources::LoadSavedDataSources(string metaDataFile,string dataFile,long limit);
	//constructor to load from multiple files
	__declspec(dllexport) LoadSavedDataSources(long limit,string directory_name,string meta_file)
	{
		this->_saved_folder = directory_name; 
		this->_metaFile = meta_file;
		this->_rowLimit = limit;
	}
	__declspec(dllexport) ~LoadSavedDataSources(void);

	#pragma endregion Constructors & Destructor
	
	/**	Method to load saved data from single xml files, parameter can be passed to limit the amount of data loaded*/
	/**	Returns the de-serialized data source*/
	__declspec(dllexport) DataSources* loadSavedEncodedData(bool limit = false) throw(error_folder_not_exist,error_loading_encoded_data);
	
	/**	Method to load saved data from multiple xml files, parameter can be passed to limit the amount of data loaded*/
	/**	Returns the de-serialized data source*/
	__declspec(dllexport) DataSources* loadSavedEncodedDataFromMultipleFiles(bool limit = false) throw(error_folder_not_exist,error_loading_encoded_data);
	
	/**	Method to return the Data source type upon the data source type ID is provided*/
	WrapDataSource::DATASOURCE getDataSourceType(int sourceType);
	
	/**	Method to return the Attribute type upon the data source attribute ID is provided*/
	ATT_TYPE getAttType(int attType);

	/**	Method to loaded coded attribute info from a single xml data file*/
	/**	dsName = data source name*/
	/**	rowCount = no of rows available in data source*/
	/**	limit = whether to limit the amount of data loaded*/
	vector<EncodedAttributeInfo*> loadCodedAttributes(string dsName,int rowCount,bool limit);

	/**	Method to loaded coded attribute info from multiple xml data file*/
	/**	dsName = data source name*/
	/**	rowCount = no of rows available in data source*/
	/**	limit = whether to limit the amount of data loaded*/
	vector<EncodedAttributeInfo*> loadCodedAttributesFromMultipleFiles(string dsName,int rowCount,bool limit);

private:
	string _fileName;
	string _metaFile;
	string _saved_folder;
	long _rowLimit;
	vector<string> saved_file_names;
};
