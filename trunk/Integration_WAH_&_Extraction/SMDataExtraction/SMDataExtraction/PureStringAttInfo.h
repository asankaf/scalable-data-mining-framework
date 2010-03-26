#pragma once

#include "PureAttInfo.h"
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class PureStringAttInfo : public PureAttInfo
{
public:
	PureStringAttInfo(void);
	~PureStringAttInfo(void);

	__declspec(dllexport) string* ValueList();
	__declspec(dllexport) void setValueList(string* values,int noRows);
	__declspec(dllexport) vector<string> uniqueValueList();
	void setUniqueValueList(int noRows);

private:
	string* _valueList;
	vector<string> _uniqueValList;
};
