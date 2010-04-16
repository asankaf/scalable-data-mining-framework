#include "StdAfx.h"
#include "EncodedDoubleAttribute.h"

EncodedDoubleAttribute::EncodedDoubleAttribute(void)
{
}

EncodedDoubleAttribute::~EncodedDoubleAttribute(void)
{
}

double EncodedDoubleAttribute::decodeTheTuple(int tupleID,bool asAnInteger){
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

	if (asAnInteger)
	{
		return val;
	}
	else return (double)(val/this->precision);
}

int EncodedDoubleAttribute::getTheSignOf(int tupleID){
	try{
		return (int)this->_signBitMap.at(tupleID - 1);
	}
	catch(std::exception &e){
		cerr<<"Error in retrieving sign data of int attributes : "<<e.what()<<endl;
		exit(6);
	}
}

void EncodedDoubleAttribute::setTheSignBitMap(vector<double> values,int valSet){
	try{
		this->_signBitMap.resize(valSet);
		for (int i = 0 ; i < valSet ; i++)
		{
			if(values[i] == abs(values[i])) this->_signBitMap[i] = false;
			else this->_signBitMap[i] = true;
		}
	}
	catch(std::exception &e){
		std::cerr<<"Error in setting the sign map : "<<e.what()<<endl;
		exit(7);
	}
}