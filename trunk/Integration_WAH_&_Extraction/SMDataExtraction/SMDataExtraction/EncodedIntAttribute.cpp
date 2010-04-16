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
		return (int)this->_signBitMap.at(tupleID - 1);
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
		
	if (this->_signBitMap.at(tupleID - 1))
	{
		val *= -1;
	}
	
	return val;
}


void EncodedIntAttribute::setTheSignBitMap(vector<long int> values,int valSet){
	try{
	this->_signBitMap.resize(valSet);
	for (int i = 0 ; i < valSet ; i++)
	{
			if(values[i] == abs(values.at(i))) this->_signBitMap.at(i) = false;
			else this->_signBitMap.at(i) = true;
		}
	}
	catch(std::exception &e){
		std::cerr<<"Error in setting the sign map : "<<e.what()<<endl;
		exit(7);
	}
}
