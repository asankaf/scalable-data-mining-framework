#include "StdAfx.h"
#include "PureStringAttInfo.h"
#include <algorithm>

using namespace std;

PureStringAttInfo::PureStringAttInfo(void)
{
}

PureStringAttInfo::~PureStringAttInfo(void)
{
}

void PureStringAttInfo::setValueList(string* values,int noRows){
	this->_valueList = values;
	this->setUniqueValueList(noRows);
}

string* PureStringAttInfo::ValueList(){
	return this->_valueList;
}

vector<string> PureStringAttInfo::uniqueValueList(){
	return this->_uniqueValList;
}

void PureStringAttInfo::setUniqueValueList(int noRows){
	
	int temp;
	for (temp = 0 ; temp < noRows ; temp++)
	{
		this->_uniqueValList.push_back(this->_valueList[temp]);
	}

	std::sort(this->_uniqueValList.begin(),this->_uniqueValList.end());
	vector<string>::iterator nonrepetitivePos;
	nonrepetitivePos = std::unique(this->_uniqueValList.begin(),this->_uniqueValList.end());
	this->_uniqueValList.erase(nonrepetitivePos,this->_uniqueValList.end());

}
