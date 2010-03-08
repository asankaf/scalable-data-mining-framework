#include "stdafx.h"
#include "DecodedTuple.h"

Tuple::Tuple(){

}

Tuple::~Tuple(){

}

void Tuple::setDecodedInts(std::vector<PureIntAttInfo*> decodedInts){
	this->_decodedInts = decodedInts;
}

void Tuple::setDecodedStrings(vector<PureStringAttInfo*> decodedStrings){
	this->_decodedStringAtts = decodedStrings;
}

vector<PureIntAttInfo*> Tuple::decodedInts(){
	return this->_decodedInts;
}

vector<PureStringAttInfo*> Tuple::decodedStringAtts(){
	return this->_decodedStringAtts;
}