#include "StdAfx.h"
#include "EncodedMultiCatAttribute.h"
#include "MultiCatDataInfo.h"
#include "boost/dynamic_bitset.hpp"
#include "VBitStream.h"
#include <time.h>
#include <iostream>

using namespace std;

EncodedMultiCatAttribute::EncodedMultiCatAttribute(void)
{
}

EncodedMultiCatAttribute::~EncodedMultiCatAttribute(void)
{
	delete []this->_mappedVals;
}

int* EncodedMultiCatAttribute::getMappedIntVals(){
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
	int temp = (int)(ceil(log10((double)maxUniqueIndex)/log10(2.0)));
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