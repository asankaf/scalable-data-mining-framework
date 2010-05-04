#include "StdAfx.h"
#include "PureDoubleAttInfo.h"

PureDoubleAttInfo::PureDoubleAttInfo(void)
{
	Init();
}

PureDoubleAttInfo::~PureDoubleAttInfo(void)
{
	this->_valList.clear();
	vector<double> temp;
	_valList.swap(temp);
	if (_valueList != NULL)
	{
		delete _valueList;
	}
}

double PureDoubleAttInfo::Lower(){
	return this->_lower;
}

double PureDoubleAttInfo::Upper(){
	return this->_upper;
}

double* PureDoubleAttInfo::ValueList(){
	return this->_valueList;
}

void PureDoubleAttInfo::setLower(double lowerLimit){
	this->_lower = lowerLimit;
}

void PureDoubleAttInfo::setUpper(double upperLimit){
	this->_upper = upperLimit;
}

void PureDoubleAttInfo::setValueList(double *values){
	this->_valueList = values;
}