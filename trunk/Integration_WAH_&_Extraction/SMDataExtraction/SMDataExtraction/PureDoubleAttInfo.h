#pragma once

#include "PureAttInfo.h"
#include <vector>

using namespace std;

class PureDoubleAttInfo : public PureAttInfo
{
public:
	__declspec(dllexport) PureDoubleAttInfo();
	__declspec(dllexport) ~PureDoubleAttInfo();
	__declspec(dllexport) double Upper();
	__declspec(dllexport) double Lower();
	__declspec(dllexport) void setUpper(double upperLimit);
	__declspec(dllexport) void setLower(double lowerLimit);	
	__declspec(dllexport) double* ValueList();
	__declspec(dllexport) void setValueList(double *values);
	__declspec(dllexport) vector<double> valList(){return this->_valList;}
	__declspec(dllexport) void setValList(vector<double> vals){this->_valList = vals;}

private:
	double _upper;
	double _lower;
	double* _valueList;
	vector<double> _valList;

};
