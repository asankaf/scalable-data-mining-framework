#include "StdAfx.h"
#include "EncodedMultiCatAttribute.h"
#include "MultiCatDataInfo.h"
#include "../Include/boost/dynamic_bitset.hpp"
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

void EncodedMultiCatAttribute::mapStringDataToCategories(string* _valueList,vector<string> _uniqueValList,int noOfRows){
	this->_uniqueValList = _uniqueValList;
	int maxUniqueIndex = _uniqueValList.size();
	int temp = (int)(ceil(log10((double)maxUniqueIndex)/log10(2.0)));
	this->setNoOfVBitStreams(temp,noOfRows);
	this->setVBitStreamSize(this->NoOfVBitStreams());
	this->_noOfUniqueVals = _uniqueValList.size();

	MultiCatDataInfo *df = new MultiCatDataInfo(_uniqueValList);
	this->_mappedIntVals = df->getAssignedEncodedNumberList();
	
	for (int i = 0 ; i < noOfRows ; i++)
	{
		int pos = std::find(_uniqueValList.begin(),_uniqueValList.end(),_valueList[i]) - _uniqueValList.begin();
		dynamic_bitset<> bitSet(this->NoOfVBitStreams(),(unsigned long)pos);
		this->_mappedValList.push_back(bitSet);
	}

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