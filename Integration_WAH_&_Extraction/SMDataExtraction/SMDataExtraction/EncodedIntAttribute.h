#pragma once

#include "VBitStream.h"
#include "AttributeType.h"
#include "EncodedAttributeInfo.h"
#include "boost/dynamic_bitset.hpp"
#include "SEEDMinerExceptions.h"

#include <vector>

using namespace std;


/************************************************************************/
/*    Class  :EncodedIntAttribute.h
/*	  Started:25.02.2010 21:45:12
/*    Updated:22.04.2010 06:13:19
/*    Author :SEEDMiner
/*    Subj   :An Attribute class that is inheriting from the Attribute.h
/*			  class, this particular class stores whatever the specific
/*			  behaviours and attributes in int data type.
/*    Version: 
/************************************************************************/

class EncodedIntAttribute : public EncodedAttributeInfo
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) EncodedIntAttribute(void);
	__declspec(dllexport) ~EncodedIntAttribute(void);

	#pragma endregion Constructors & Destructor

	#pragma region Getters & Setters

 	__declspec(dllexport) vector<bool> SignBitMap();
	__declspec(dllexport) dynamic_bitset<> SignBitSet(){return this->_signBitSet;}
	__declspec(dllexport) long maxAttVal(){return this->_maxVal;}
	__declspec(dllexport) long minAttVal(){return this->_minVal;}
	__declspec(dllexport) void setMaxVal(long val){this->_maxVal = val;}
	__declspec(dllexport) void setMinVal(long val){this->_minVal = val;}
	__declspec(dllexport) void setTheSignBitMap(vector<long int> values,int valSet) throw(error_vector_out_of_range);
	__declspec(dllexport) void setSignBitMap(vector<bool> bitMap){this->_signBitMap = bitMap;}
	__declspec(dllexport) void setSignBitSet(dynamic_bitset<> bitSet){this->_signBitSet = bitSet;}

	#pragma endregion Getters & Setters

	/** Concrete implementation of the Decoding the tuple according to the integer attribute type*/
	__declspec(dllexport) int decodeTheTuple(int tupleID) throw(error_vector_out_of_range);

	/** Method to return the sign of a particular value in the integer value set*/
	__declspec(dllexport) int getTheSignOf(int tupleID) throw(error_vector_out_of_range);

	/** Method to return the signbitmap as an integer value*/
	__declspec(dllexport) dynamic_bitset<> signMapAsBitSet();

private:
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.
	dynamic_bitset<> _signBitSet;
	long _maxVal;
	long _minVal;
};
