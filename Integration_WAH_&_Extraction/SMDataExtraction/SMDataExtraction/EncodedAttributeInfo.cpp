#include "stdafx.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include <iostream>
#include "commons.h"

using namespace std;

EncodedAttributeInfo::EncodedAttributeInfo(){

}

EncodedAttributeInfo::~EncodedAttributeInfo(){
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
	size_t space = 0;
	//typedef vector<BitStreamInfo *>::const_iterator bit_iter;
	for (size_t index = 0; index < _vBitStreams.size(); index++)
	{
		space += _vBitStreams.at(index)->SpaceUtilisation();
	}
	return space;
}

BitStreamInfo* EncodedAttributeInfo::bitStreamAt(int bitStreamID){
	return this->_vBitStreams[bitStreamID];
}


BitStreamInfo* EncodedAttributeInfo::operator ()(const int bitStreamID){
	try{
	return this->bitStreamAt(bitStreamID);
}
	catch(std::exception &e){
		std::cerr<<"Error in retrieving bit stream : "<<e.what()<<endl;
		exit(0);
	}
}

void EncodedAttributeInfo::setVBitStreamSize(int newSize){
	//this->_vBitStreams.clear();
	//this->_vBitStreams.resize(newSize);
}

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
	try{
		this->_noOfVBitStreams = novBitsets;
// 		for (int i = 0 ; i < rows ; i++)
// 		{
// 			this->_vBStreams[i] = new VBitStream(rows);
// 		}
	}
	catch(std::exception &e){
		cerr<<"Error in creating vertical bit streams for a given # of bit stream"<<endl;
		exit(5);
	}
}

void EncodedAttributeInfo::setVBitStreams(BitStreamInfo **VBitStreams){
	this->_vBStreams = VBitStreams;
}

void EncodedAttributeInfo::setVBitStreams(vector<BitStreamInfo *> VBitStreams){
	this->_vBitStreams = VBitStreams;
}