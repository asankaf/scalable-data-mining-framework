#pragma once
#include <hash_map>
#include <iostream>
//#include <string>

using namespace std;
using namespace stdext;

/************************************************************************/
/*    Class  :ConfigurationReader.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.05.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :Common utility class to read configuration values
/*    Version: 
/************************************************************************/

class ConfigurationReader
{
public:

	/** enum to hold the available configuration keys available*/
	static enum configutation{	COMPRESSION,
		METAFILE_NAME,
		DATAFILE_NAME,
		FILE_NAME ,
		FOLDER_NAME ,
		LOAD_TYPE ,
		LOAD_TYPE_MULTI,
		LOAD_TYPE_SINGLE,
		LOAD_TYPE_CSV,
		SAVE_DATA_FOLDER,
		DOUBLE_PRECISION,
		CSV_NULL	};

	#pragma region Constructors & Destructor

	_declspec(dllexport) ConfigurationReader(void);

	_declspec(dllexport) ConfigurationReader(string & _path);

	_declspec(dllexport) ~ConfigurationReader(void);

	#pragma endregion Constructors & Destructor

	/** Gives the Configuration indicated by _property*/
	_declspec(dllexport)static  string ReadConfiguration(string & _property);

	/** Gives the Configuration indicated by _property*/
	_declspec(dllexport)static  string ReadConfiguration(const string & _property);

	/** Gives the Configuration indicated by _property*/
	_declspec(dllexport)static string ReadConfiguration(configutation _property);

	/** Creating the Exception file, given by the location _path*/
	_declspec(dllexport) static void BuildFile(string & _path);	

private :
	static hash_map<string,string> m_map;
	static string getMapping(configutation & _configuration);
	static string COMPRESSION_VAL;
	static string METAFILE_NAME_VAL;
	static string DATAFILE_NAME_VAL;
	static string FILE_NAME_VAL;
	static string FOLDER_NAME_VAL ;
	static string LOAD_TYPE_VAL ;
	static string LOAD_TYPE_MULTI_VAL;
	static string LOAD_TYPE_SINGLE_VAL;
	static string LOAD_TYPE_CSV_VAL;
	static string SAVE_XML_FILE_FOLDER;
	static string DOUBLE_PRECISION_VAL;
	static string CSV_NULL_VAL;
};
