#include "StdAfx.h"
#include "EncodedIntAttribute.h"

#include <bitset>
#include <math.h>
#include <iostream>

EncodedIntAttribute::EncodedIntAttribute(void)
{
}

EncodedIntAttribute::~EncodedIntAttribute(void)
{
}


vector<bool> EncodedIntAttribute::SignBitMap(){
	return this->_signBitMap;
}


int EncodedIntAttribute::getTheSignOf(int tupleID){
	try{
		return (int)this->_signBitMap[tupleID - 1];
	}
	catch(std::exception &e){
		cerr<<"Error in retrieving sign data of int attributes : "<<e.what()<<endl;
		exit(6);
	}
}

int EncodedIntAttribute::decodeTheTuple(int tupleID){

	dynamic_bitset<> temp(this->NoOfVBitStreams());
	int val=0;

	for (int i=0 ; i < this->NoOfVBitStreams() ;i++)
	{
		temp[i] = this->vBitStreams()[i]->getProcessedBitStream()[tupleID - 1];
	}

	val = temp.to_ulong();
		
	if (this->_signBitSet[tupleID - 1] == 1)
	{
		val *= -1;
	}
	
	return val;
}


void EncodedIntAttribute::setTheSignBitMap(vector<long int> values,int valSet){
	try{
	this->_signBitMap.resize(valSet);
	this->_signBitSet.resize(valSet);
	for (int i = 0 ; i < valSet ; i++)
	{
			if(values[i] == abs(values[i])) this->_signBitMap[i] = false;
			else {this->_signBitMap.at(i) = true;this->_signBitSet[i] = true;}
		}
	}
	catch(std::exception &e){
		std::cerr<<"Error in setting the sign map : "<<e.what()<<endl;
		exit(7);
	}
}

dynamic_bitset<> EncodedIntAttribute::signMapAsBitSet()
{
	dynamic_bitset<> bitSet(this->_signBitMap.size());
	for (int i=0 ; i < this->_signBitMap.size() ; i++)
	{
		bitSet[i] = _signBitMap[i];
	}
	return bitSet;
}