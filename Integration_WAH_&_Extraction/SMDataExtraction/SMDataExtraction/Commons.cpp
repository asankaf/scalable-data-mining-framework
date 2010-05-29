#include "StdAfx.h"
#include "Commons.h"
#include "EncodedDoubleAttribute.h"

Commons::Commons(void)
{
}

Commons::~Commons(void)
{
}

hash_map<string,string> Commons::m_map;

void Commons::PrintArray(double * _double_arr,int _length)
{
	for (size_t i =0 ; i < _length ;i++)
	{
		cout << "Index " << i << " : " << _double_arr[i]<< endl;
	}
}

void Commons::PrintArray(double **_double_arr, int _length, int _width)
{
	for (size_t i = 0; i < _length ; i++)
	{
		cout << "Printing Row " << i << endl;
		for (size_t j = 0 ; j < _width ; j++)
		{
			cout << _double_arr[i][j] << "\t";
		}
		cout << endl;
	}
}

void Commons::InitArray( double * _double_arr,int _length,double _init_val )
{
	for (size_t i = 0 ; i < _length ; i++)
	{
		_double_arr[i] = _init_val;
	}
}

void Commons::InitArray(double ** _double_arr,int _length,int _width,double _init_val)
{
	for (size_t i = 0 ; i < _length ; i++)
	{
		for (size_t j = 0 ; j < _width ; j++)
		{
			_double_arr[i][j] = _init_val;
		}
	}
}

string Commons::ReadConfiguration(string & _property)
{
	return m_map[_property];
}

vector<string> Commons::ReadFile(string & _path)
{
	vector<string> row_vect;
	ifstream infile(_path.data());
	while(!infile.eof()) // To get you all the lines.
	{
		string temp_string;
		getline(infile,temp_string);
		row_vect.push_back(temp_string);
	}
	return row_vect;
}

void Commons::BuildFile( string & _path )
{
	vector<string> temp = ReadFile(_path);
	for (size_t i = 0 ; i < temp.size() ; i++)
	{
		Tokenise(temp[i],m_map);
	}
}

void Commons::Tokenise(string & _str,hash_map<string,string> & _map)
{
	vector<string> result(2);
	char_separator<char> sep(":");
	tokenizer<char_separator<char>> tokens(_str, sep);
	tokenizer<char_separator<char>>::iterator iter = tokens.begin();
	size_t i = 0 ;
	_map[*iter++] = *iter;	
}