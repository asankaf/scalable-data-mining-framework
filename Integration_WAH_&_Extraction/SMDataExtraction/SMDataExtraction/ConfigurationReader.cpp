#include "ConfigurationReader.h"
#include "commons.h"

ConfigurationReader::ConfigurationReader(void)
{
}

ConfigurationReader::ConfigurationReader(string & _path)
{
	BuildFile(_path);
	COMPRESSION_VAL ="compression";
	METAFILE_NAME_VAL ="metafile";
	DATAFILE_NAME_VAL ="datafile",
	FILE_NAME_VAL = "filename";
	FOLDER_NAME_VAL = "datafolder";
	LOAD_TYPE_VAL = "loadtype";
	LOAD_TYPE_MULTI_VAL = "multiple";
	LOAD_TYPE_SINGLE_VAL = "single";
	LOAD_TYPE_CSV_VAL = "csv";
	SAVE_XML_FILE_FOLDER = "savedatafolder";
}

hash_map<string,string> ConfigurationReader::m_map;

ConfigurationReader::~ConfigurationReader(void)
{
}

string ConfigurationReader::ReadConfiguration(string & _property)
{
	return m_map[_property];
}

string ConfigurationReader::ReadConfiguration(const string & _property)
{
	return m_map[_property];
}

string ConfigurationReader::ReadConfiguration(configutation _property)
{
	return m_map[getMapping(_property)];
}

string ConfigurationReader::getMapping(ConfigurationReader::configutation &_configuration)
{
	switch (_configuration)
	{
	case COMPRESSION:
		{
			return COMPRESSION_VAL;
		}
		break;
	case METAFILE_NAME:
		{
			return METAFILE_NAME_VAL;
		}
		break;
	case DATAFILE_NAME:
		{
			return DATAFILE_NAME_VAL;
		}
		break;
	case FILE_NAME:
		{
			return FILE_NAME_VAL;
		}
		break;
	case FOLDER_NAME:
		{
			return FOLDER_NAME_VAL;
		}
		break;
	case LOAD_TYPE:
		{
			return LOAD_TYPE_VAL;
		}
		break;
	case LOAD_TYPE_MULTI:
		{
			return LOAD_TYPE_MULTI_VAL;
		}
		break;
	case LOAD_TYPE_SINGLE:
		{
			return LOAD_TYPE_SINGLE_VAL;
		}
		break;
	case LOAD_TYPE_CSV:
		{
			return LOAD_TYPE_CSV_VAL;
		}
	case SAVE_DATA_FOLDER:
		{
			return SAVE_XML_FILE_FOLDER;
		}
		break;
	}	
}

string ConfigurationReader::COMPRESSION_VAL = "compression";

string ConfigurationReader::METAFILE_NAME_VAL = "metafile";

string ConfigurationReader::DATAFILE_NAME_VAL = "datafile";

string ConfigurationReader::FILE_NAME_VAL = "filename";

string ConfigurationReader::FOLDER_NAME_VAL = "datafolder";

string ConfigurationReader::LOAD_TYPE_VAL = "loadtype";

string ConfigurationReader::LOAD_TYPE_MULTI_VAL = "multiple";

string ConfigurationReader::LOAD_TYPE_SINGLE_VAL = "single";

string ConfigurationReader::LOAD_TYPE_CSV_VAL = "csv";

string ConfigurationReader::SAVE_XML_FILE_FOLDER = "savedatafolder";

void ConfigurationReader::BuildFile( string & _path )
{
	if (m_map.empty())
	{
		vector<string> temp = Commons::ReadFile(_path);
		for (size_t i = 0 ; i < temp.size() ; i++)
		{
			Commons::Tokenise(temp[i],":",m_map);
		}
	}

}
