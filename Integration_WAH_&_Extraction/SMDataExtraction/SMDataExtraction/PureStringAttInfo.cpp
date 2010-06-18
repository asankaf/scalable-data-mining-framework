#pragma once
#include "StdAfx.h"
#include "PureStringAttInfo.h"
#include <algorithm>
#include <time.h>
#include <iostream>
#include <set>

using namespace std;

PureStringAttInfo::PureStringAttInfo(void)
{
	Init();
}

PureStringAttInfo::~PureStringAttInfo(void)
{
	delete _valueList;
	_uniqueSet.clear();
	_uniqueValList.clear();
	_valList.clear();
	vector<string> temp;
	_valList.swap(temp);
	_uniqueValList.swap(temp);
	std::set<string> tempSet;
	tempSet.swap(_uniqueSet);
}

void PureStringAttInfo::setValueList(string* values,int noRows){
	this->_valueList = values;
	this->setUniqueValueList(noRows);
}

vector<string> PureStringAttInfo::ValueList(){
	return this->_valList;
}

vector<string> PureStringAttInfo::uniqueValueList(){
	return this->_uniqueValList;
}

void PureStringAttInfo::setUniqueValueList(int noRows){
	
	int temp;
	this->_uniqueValList.resize(noRows);
	for (temp = 0 ; temp < noRows ; temp++)
	{
		this->_uniqueValList[temp] = (this->_valueList[temp]);
	}

	std::sort(this->_uniqueValList.begin(),this->_uniqueValList.end());
	vector<string>::iterator nonrepetitivePos;
	nonrepetitivePos = std::unique(this->_uniqueValList.begin(),this->_uniqueValList.end());
	this->_uniqueValList.erase(nonrepetitivePos,this->_uniqueValList.end());

}

void PureStringAttInfo::setValList(vector<string> valList){
	this->_valList = valList;
		
	clock_t start,end;
	//start = clock();
	std::set<string> uniqueSet(valList.begin(),valList.end());
	this->_uniqueSet = uniqueSet;

	//end = clock();
	//cout<<"Time to set unique values : "<<(end - start)<<endl;
}
