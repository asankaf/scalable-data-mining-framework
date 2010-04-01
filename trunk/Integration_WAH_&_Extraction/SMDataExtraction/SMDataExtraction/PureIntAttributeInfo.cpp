#include "PureIntAttributeInfo.h"
#include "stdafx.h"

namespace AttributeInfo{
	AttributeInfo::PureIntAttributeInfo()
	{

	}

	int AttributeInfo::PureIntAttributeInfo::Lower(){
		return this->_lower;
	}

	int AttributeInfo::PureIntAttributeInfo::Upper(){
		return this->_upper;
	}

	int* AttributeInfo::PureIntAttributeInfo::ValueList(){
		return this->_valueList;
	}

	void AttributeInfo::PureIntAttributeInfo::setValueList(int *values){
		this->_valueList=values;
	}

	AttributeInfo::PureIntAttributeInfo::~PureIntAttributeInfo(){

	}

}