#include "StdAfx.h"
#include "EncodedDoubleAttribute.h"
#include "ExceptionReader.h"
#include "ExceptionCodes.h"

EncodedDoubleAttribute::EncodedDoubleAttribute(void)
{
}

EncodedDoubleAttribute::~EncodedDoubleAttribute(void)
{
}

double EncodedDoubleAttribute::decodeTheTuple(int tupleID,bool asAnInteger){
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
		err += "-> @Decoding a double tuple.";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}

	if (asAnInteger)
	{
		return val;
	}
	double d_val = ((double)val) / this->precision;
	return d_val;
}

int EncodedDoubleAttribute::getTheSignOf(int tupleID){
	try{
		return (int)this->_signBitMap.at(tupleID - 1);
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

void EncodedDoubleAttribute::setTheSignBitMap(vector<double> values,int valSet){
	try{
		this->_signBitMap.resize(valSet);
		this->_signBitSet.resize(valSet);
		for (int i = 0 ; i < valSet ; i++)
		{
			if(values[i] == abs(values[i])) this->_signBitMap[i] = false;
			else {this->_signBitMap[i] = true; this->_signBitSet[i] = true;}
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