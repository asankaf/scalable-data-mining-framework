#include "ExceptionReader.h"
#include "Commons.h"

ExceptionReader::ExceptionReader(void)
{
}

ExceptionReader::ExceptionReader(string & _path)
{
	BuildFile(_path);
}

hash_map<string,string> ExceptionReader::m_map;

ExceptionReader::~ExceptionReader(void)
{
}

string ExceptionReader::GetError(string & _property)
{
	return m_map[_property];
}

string ExceptionReader::GetError(const string & _property)
{
	return m_map[_property];
}

void ExceptionReader::BuildFile( string & _path )
{
	if (m_map.empty())
	{
		vector<string> temp = Commons::ReadFile(_path);
		for (size_t i = 0 ; i < temp.size() ; i++)
		{
			Commons::Tokenise(temp[i],m_map);
		}
	}
	
}