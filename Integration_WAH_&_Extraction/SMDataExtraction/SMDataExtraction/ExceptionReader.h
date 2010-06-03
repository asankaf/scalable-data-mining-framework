#pragma once
#include <iostream>
using namespace std;
class ExceptionReader
{
public:
	ExceptionReader(void);
	~ExceptionReader(void);
	string GetMessage(string & _error_code){return "";}
};
