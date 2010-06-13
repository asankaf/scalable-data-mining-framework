#pragma once
#include <hash_map>
#include <iostream>
//#include <string>

using namespace std;
using namespace stdext;
class ConfigurationReader
{
public:
	static enum configutation{	COMPRESSION,
		METAFILE_NAME,
		DATAFILE_NAME,
		FILE_NAME ,
		FOLDER_NAME ,
		LOAD_TYPE ,
		LOAD_TYPE_MULTI,
		LOAD_TYPE_SINGLE,
		LOAD_TYPE_CSV	};

	_declspec(dllexport) ConfigurationReader(void);

	_declspec(dllexport) ConfigurationReader(string & _path);

	_declspec(dllexport) ~ConfigurationReader(void);

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

};
