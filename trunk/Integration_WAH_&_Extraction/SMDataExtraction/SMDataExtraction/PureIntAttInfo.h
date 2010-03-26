#include "PureAttInfo.h"

#pragma once

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
	__declspec(dllexport) void setValueList(long int *values);

private:
	long int _upper;
	long int _lower;
	long int* _valueList;
};
