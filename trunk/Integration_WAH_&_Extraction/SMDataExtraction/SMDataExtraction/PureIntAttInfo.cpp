#include "StdAfx.h"
#include "PureIntAttInfo.h"

PureIntAttInfo::PureIntAttInfo(void)
{
	Init();
}

PureIntAttInfo::~PureIntAttInfo(void)
{
	_valList.clear();
	vector<long int> temp;
	_valList.swap(temp);
	delete _valueList;
}

long int PureIntAttInfo::Upper(){
	return this->_upper;
}

long int PureIntAttInfo::Lower(){
	return this->_lower;
}

long int* PureIntAttInfo::ValueList(){
	return this->_valueList;
}

void PureIntAttInfo::setValueList(long int *values){
	this->_valueList = values;
}

void PureIntAttInfo::setLower(long int lowerLimit){
	this->_lower = lowerLimit;
}

void PureIntAttInfo::setUpper(long int upperLimit){
	this->_upper = upperLimit;
}
