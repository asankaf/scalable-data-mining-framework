#pragma once

#include "PureAttInfo.h"
#include "StringSpecAssignVals.h"
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include "Commons.h"

using namespace std;

class PureStringAttInfo : public PureAttInfo
{
public:
	__declspec(dllexport) PureStringAttInfo(void);
	__declspec(dllexport) ~PureStringAttInfo(void);

	__declspec(dllexport) vector<string> ValueList();
	__declspec(dllexport) TempStringObjects* ValObjects(){return this->_valObjects;}
	__declspec(dllexport) void setValObjects(TempStringObjects* _vals){this->_valObjects = _vals;}
	__declspec(dllexport) void setValueList(string* values,int noRows);
	__declspec(dllexport) void setValList(vector<string> valList);
	__declspec(dllexport) vector<string> uniqueValueList();
	__declspec(dllexport) std::set<string> uniqueValueSet(){return this->_uniqueSet;}
	__declspec(dllexport) void setUniqueValSet(std::set<string> _uniqueSet){this->_uniqueSet = _uniqueSet;}
	void setUniqueValueList(int noRows);

	void Init(){
		_valueList = NULL;
		//Commons::InitVector(_valList.begin(),_valList.end());
		//Commons::InitVector(_uniqueValList.begin(),_uniqueValList.end());
		//Commons::InitVector(_uniqueSet.begin(),_uniqueSet.end());
	}

private:
	string* _valueList;
	TempStringObjects* _valObjects;
	vector<string> _uniqueValList;
	vector<string> _valList;
	std::set<string> _uniqueSet;
};
