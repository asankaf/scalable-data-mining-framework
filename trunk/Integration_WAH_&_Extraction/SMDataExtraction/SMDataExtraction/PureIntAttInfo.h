#include "PureAttInfo.h"
#include <vector>
#include "Commons.h"

#pragma once

using namespace std;

class PureIntAttInfo : public PureAttInfo
{
public:
	__declspec(dllexport) PureIntAttInfo();
	__declspec(dllexport) ~PureIntAttInfo();
	__declspec(dllexport) long int Upper();
	__declspec(dllexport) long int Lower();
	void setUpper(long int upperLimit);
	void setLower(long int lowerLimit);	
	__declspec(dllexport) long int* ValueList();
	__declspec(dllexport) vector<long int> valList(){return this->_valList;}
	__declspec(dllexport) void setValueList(long int *values);
	__declspec(dllexport) void setValList(vector<long int> vals){this->_valList = vals;}

	void Init(){
		Commons::InitVector(_valList.begin(),_valList.end());
		_valueList = NULL;
	}
private:
	long int _upper;
	long int _lower;
	long int* _valueList;
	vector<long int> _valList;
};
