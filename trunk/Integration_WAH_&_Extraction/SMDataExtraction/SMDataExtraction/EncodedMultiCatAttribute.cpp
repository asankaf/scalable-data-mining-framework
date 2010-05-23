#include "StdAfx.h"
#include "EncodedMultiCatAttribute.h"
#include "MultiCatDataInfo.h"
#include "boost/dynamic_bitset.hpp"
#include "VBitStream.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <string>
#include "commons.h"

using namespace std;

EncodedMultiCatAttribute::EncodedMultiCatAttribute(void)
{
	Init();
}

EncodedMultiCatAttribute::~EncodedMultiCatAttribute(void)
{
// 	
// 	if ( _mappedValList.size() != 0)
// 	{
// _mappedValList.clear();
// 	}
	//delete []this->_mappedVals;

}

void EncodedMultiCatAttribute::Init()
{
	_mappedIntVals = NULL;
	_mappedVals = NULL;	
}

int* EncodedMultiCatAttribute::getMappedIntVals()
{
	return this->_mappedIntVals;
}

int EncodedMultiCatAttribute::noOfUniqueValues(){
	return this->_noOfUniqueVals;
}

dynamic_bitset<>* EncodedMultiCatAttribute::mapStringDataToCategories(vector<string> _valueList,std::set<string> uniqueValList,int noOfRows){
	this->_mappedVals = new dynamic_bitset<>[noOfRows];
	time_t start,end;
	start = clock();
	this->_uniqueValList.assign(uniqueValList.begin(),uniqueValList.end());
	setUniqueMap();
	end = clock();
	cout<<"Time to assign the set to a vector : "<<(end - start)<<endl;

	int maxUniqueIndex = _uniqueValList.size();
	int temp=0;

	if (maxUniqueIndex == 1)
	{
		temp = 1;
	}
	else 
	{
		temp = (int)(ceil(log10((double)maxUniqueIndex)/log10(2.0)));		
	}
	this->setNoOfVBitStreams(temp,noOfRows);
	this->_noOfUniqueVals = _uniqueValList.size();

	int no_v_bitStreams = this->NoOfVBitStreams();
	for (int i = 0 ; i < noOfRows ; i++)
	{
		//int pos = std::find(_uniqueValList.begin(),_uniqueValList.end(),_valueList[i]) - _uniqueValList.begin();
		_it = this->_uniqueValueMap.find(_valueList[i]);
		int pos = _it->second;
		dynamic_bitset<> bitSet(no_v_bitStreams,(unsigned long)pos);
		this->_mappedVals[i] = bitSet;
	}
	return this->_mappedVals;
}

dynamic_bitset<>* EncodedMultiCatAttribute::mapStringDataToCategories(TempStringObjects* _tempStrs,int noRows,int NoUniqueVals){
	vector<TempStringObjects> tempObjs(_tempStrs,_tempStrs + noRows);
	int tempMax = (int)(ceil(log10((double)NoUniqueVals)/log10(2.0)));
	this->setNoOfVBitStreams(tempMax,noRows);
	std::sort(tempObjs.begin(),tempObjs.end());
	string temp;
	int tempCount = 0;
	if (noRows > 0)
	{
		temp = tempObjs[0].Val();
	}
	for (int i = 0 ; i < tempObjs.size() ; i++)
	{
		TempStringObjects obj = tempObjs[i];
		obj.Index(obj.Index() - i);
		if (strcmp(obj.Val().c_str(),temp.c_str()) != 0)
		{
			tempCount++;
			temp = obj.Val();
		}
		dynamic_bitset<> bitSet(this->NoOfVBitStreams(),(unsigned long)(obj.Index() + tempCount));
		this->_mappedVals[i] = bitSet;		
	}
	return this->_mappedVals;
}

void EncodedMultiCatAttribute::setMappedValList(vector<dynamic_bitset<>> & _mapped_vals)
{
	_mappedValList = _mapped_vals;
}

vector<dynamic_bitset<>> EncodedMultiCatAttribute::mappedValList(){
	return this->_mappedValList;
}

vector<string> EncodedMultiCatAttribute::uniqueValList(){
	return this->_uniqueValList;
}

string EncodedMultiCatAttribute::decodeTheTuple(int tupleID){

	dynamic_bitset<> temp(this->NoOfVBitStreams());
	int val=0;

	for (int i=0 ; i < this->NoOfVBitStreams() ;i++)
	{
		temp[i] = this->vBitStreams()[i]->getProcessedBitStream()[tupleID - 1];
	}

	val = (int)temp.to_ulong();
	return this->_uniqueValList[val];
}

int EncodedMultiCatAttribute::binarySearch(vector<string> arr, std::string value, int left, int right){
	while (left <= right) {
		int middle = (left + right) / 2;
		if (arr[middle] == value)
			return middle;
		else if (arr[middle] > value)
			right = middle - 1;
		else
			left = middle + 1;
	}
	return -1;
}

void EncodedMultiCatAttribute::setUniqueMap(){
	for (int i = 0 ; i < this->_uniqueValList.size() ; i++)
	{
		this->_uniqueValueMap[this->_uniqueValList.at(i)] = i;
	}
}