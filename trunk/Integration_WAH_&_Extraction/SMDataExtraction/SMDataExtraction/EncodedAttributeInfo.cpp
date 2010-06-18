#include "stdafx.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include <iostream>
#include "commons.h"
#include "seedminerexceptions.h"
#include "exceptionreader.h"
#include "ExceptionCodes.h"

using namespace std;

EncodedAttributeInfo::EncodedAttributeInfo(){

}

EncodedAttributeInfo::~EncodedAttributeInfo(){
	Commons::DeleteVector(_vBitStreams.begin(),_vBitStreams.end());
}

void EncodedAttributeInfo::DeleteVBitStreams()
{
Commons::DeleteVector(_vBitStreams.begin(),_vBitStreams.end());
}

vector<BitStreamInfo*> EncodedAttributeInfo::vBitStreams(){
	return this->_vBitStreams;
}

int EncodedAttributeInfo::attributeID(){
	return this->_attID;
}

string EncodedAttributeInfo::attributeName(){
	return this->_attName;
}

ATT_TYPE EncodedAttributeInfo::attributeType(){
	return this->_attType;
}

int EncodedAttributeInfo::NoOfVBitStreams(){
	return this->_noOfVBitStreams;
}

size_t EncodedAttributeInfo::SpaceUtilisation()
{
	size_t space = sizeof(this);
	try
	{
		for (size_t index = 0; index < _vBitStreams.size(); index++)
		{
			space += _vBitStreams.at(index)->SpaceUtilisation();
		}
	}
	catch(...)
	{
		error_vector_out_of_range ex;
		string err = ExceptionReader::GetError(SM1007);
		err += "-> Retrieving bits from bitstream @ space utilization.";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
	return space;
}

BitStreamInfo* EncodedAttributeInfo::bitStreamAt(int bitStreamID){
	return this->_vBitStreams.at(bitStreamID);
}


BitStreamInfo* EncodedAttributeInfo::operator ()(const int bitStreamID){
	try
	{
		return this->bitStreamAt(bitStreamID);
	}
	catch(...){
		error_vector_out_of_range ex;
		string err = ExceptionReader::GetError(SM1007);
		err += "-> Retrieving BitStream from EncodedAttribute";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
}

void EncodedAttributeInfo::setVBitStreamSize(int newSize){}

void EncodedAttributeInfo::setAttID(int attID){
	this->_attID = attID;
}

void EncodedAttributeInfo::setAttName(std::string name){
	this->_attName = name;
}

void EncodedAttributeInfo::setAttType(ATT_TYPE type){
	this->_attType = type;
}

void EncodedAttributeInfo::setNoOfVBitStreams(int novBitsets, int rows){
	this->_noOfVBitStreams = novBitsets;
}

void EncodedAttributeInfo::setVBitStreams(BitStreamInfo **VBitStreams){
	this->_vBStreams = VBitStreams;
}

void EncodedAttributeInfo::setVBitStreams(vector<BitStreamInfo *> VBitStreams){
	this->_vBitStreams = VBitStreams;
}