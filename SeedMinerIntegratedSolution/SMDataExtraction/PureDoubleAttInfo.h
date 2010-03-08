#pragma once

#include "PureAttInfo.h"

class PureDoubleAttInfo : public PureAttInfo
{
public:
	__declspec(dllexport) PureDoubleAttInfo();
	__declspec(dllexport) ~PureDoubleAttInfo();
	__declspec(dllexport) double Upper();
	__declspec(dllexport) double Lower();
	void setUpper(double upperLimit);
	void setLower(double lowerLimit);	
	__declspec(dllexport) double* ValueList();
	__declspec(dllexport) void setValueList(double *values);

private:
	double _upper;
	double _lower;
	double* _valueList;

};
