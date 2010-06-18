#pragma once
#include <vector>
#include "VBitStream.h"
#include "AttributeType.h"
#include "EncodedAttributeInfo.h"
#include "boost/dynamic_bitset.hpp"
#include "SEEDMinerExceptions.h"

using namespace std;

class EncodedIntAttribute : public EncodedAttributeInfo
{
public:
	__declspec(dllexport) EncodedIntAttribute(void);
	__declspec(dllexport) ~EncodedIntAttribute(void);
 	__declspec(dllexport) vector<bool> SignBitMap();
	__declspec(dllexport) dynamic_bitset<> SignBitSet(){return this->_signBitSet;}
	__declspec(dllexport) long maxAttVal(){return this->_maxVal;}
	__declspec(dllexport) long minAttVal(){return this->_minVal;}
	__declspec(dllexport) void setMaxVal(long val){this->_maxVal = val;}
	__declspec(dllexport) void setMinVal(long val){this->_minVal = val;}
	__declspec(dllexport) int decodeTheTuple(int tupleID) throw(error_vector_out_of_range);
	__declspec(dllexport) int getTheSignOf(int tupleID) throw(error_vector_out_of_range);
	__declspec(dllexport) void setTheSignBitMap(vector<long int> values,int valSet) throw(error_vector_out_of_range);
	__declspec(dllexport) void setSignBitMap(vector<bool> bitMap){this->_signBitMap = bitMap;}
	__declspec(dllexport) void setSignBitSet(dynamic_bitset<> bitSet){this->_signBitSet = bitSet;}
	__declspec(dllexport) dynamic_bitset<> signMapAsBitSet();

private:
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.
	dynamic_bitset<> _signBitSet;
	long _maxVal;
	long _minVal;
};
