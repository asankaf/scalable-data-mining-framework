#include "StdAfx.h"
#include "EncodedIntAttribute.h"
#include <bitset>
#include <math.h>
#include <iostream>
#include "ExceptionCodes.h"
#include "ExceptionReader.h"

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
	catch(...){
		error_vector_out_of_range ex;
		string err = ExceptionReader::GetError(SM1007);
		err += "-> @Retrieving bits from the signbitmap.";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
}

int EncodedIntAttribute::decodeTheTuple(int tupleID){

	dynamic_bitset<> temp(this->NoOfVBitStreams());
	int val=0;
	try
	{
		for (int i=0 ; i < this->NoOfVBitStreams() ;i++)
		{
			temp[i] = this->vBitStreams()[i]->Decompress()[tupleID - 1];
		}

		val = temp.to_ulong();

		if (this->_signBitSet[tupleID - 1] == 1)
		{
			val *= -1;
		}
	}
	catch(...)
	{
		error_vector_out_of_range ex;
		string err = ExceptionReader::GetError(SM1007);
		err += "-> @Decoding an Int tuple.";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
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
	catch(...){
		error_vector_out_of_range ex;
		string err = ExceptionReader::GetError(SM1007);
		err += "-> @Setting the signbitmap and signbitset.";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
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