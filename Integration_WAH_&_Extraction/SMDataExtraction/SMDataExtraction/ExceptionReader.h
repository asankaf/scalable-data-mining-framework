#pragma once
#include <hash_map>
#include <iostream>
//#include <string>

using namespace std;
using namespace stdext;
class ExceptionReader
{
public:
	_declspec(dllexport) ExceptionReader(void);

	_declspec(dllexport) ExceptionReader(string & _path);
	
	_declspec(dllexport) ~ExceptionReader(void);
	
	/** Gives the error message indicated by _error_code*/
	_declspec(dllexport)static  string GetError(string & _error_code);

	/** Gives the error message indicated by _error_code*/
	_declspec(dllexport)static  string GetError(const string & _error_code);

	/** Creating the Exception file, given by the location _path*/
	_declspec(dllexport) static void BuildFile(string & _path);


private :
	static hash_map<string,string> m_map;
	
	
};
/*

static struct static_block {
static_block() {		
cout <<"Hello" << endl;
ExceptionReader::BuildFile(string("exception.exp"));		
cout <<"Finished Hello" << endl;
}
} _STATIC_BLOCK_INSTANCE;*/
