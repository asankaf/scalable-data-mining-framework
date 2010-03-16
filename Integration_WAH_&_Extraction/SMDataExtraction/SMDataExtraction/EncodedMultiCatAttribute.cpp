#include "StdAfx.h"
#include "EncodedMultiCatAttribute.h"
#include "MultiCatDataInfo.h"
#include "boost/dynamic_bitset.hpp"
#include "VBitStream.h"

EncodedMultiCatAttribute::EncodedMultiCatAttribute(void)
{
}

EncodedMultiCatAttribute::~EncodedMultiCatAttribute(void)
{
}

int* EncodedMultiCatAttribute::getMappedIntVals(){
	return this->_mappedIntVals;
}

int EncodedMultiCatAttribute::noOfUniqueValues(){
	return this->_noOfUniqueVals;
}

void EncodedMultiCatAttribute::mapStringDataToCategories(vector<string> _valueList,vector<string> _uniqueValList,int noOfRows){
	this->_uniqueValList = _uniqueValList;
	int maxUniqueIndex = _uniqueValList.size();
	int temp = (int)(ceil(log10((double)maxUniqueIndex)/log10(2.0)));
	this->setNoOfVBitStreams(temp,noOfRows);
	this->setVBitStreamSize(this->NoOfVBitStreams());
	this->_noOfUniqueVals = _uniqueValList.size();

	MultiCatDataInfo *df = new MultiCatDataInfo(_uniqueValList);
	this->_mappedIntVals = df->getAssignedEncodedNumberList();

	int no_v_bitStreams = this->NoOfVBitStreams();
	for (int i = 0 ; i < noOfRows ; i++)
	{
		//int pos = std::find(_uniqueValList.begin(),_uniqueValList.end(),_valueList[i]) - _uniqueValList.begin();
		int pos = binarySearch(_uniqueValList,_valueList.at(i),0,(_uniqueValList.size() - 1));
		dynamic_bitset<> bitSet(no_v_bitStreams,(unsigned long)pos);
		this->_mappedValList.push_back(bitSet);
	}

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
		temp[i] = this->vBitStreams().at(i)->getProcessedBitStream()[tupleID - 1];
	}

	val = (int)temp.to_ulong();
	
	return this->_uniqueValList.at(val);
}

int EncodedMultiCatAttribute::binarySearch(std::vector<string> arr, std::string value, int left, int right){
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
