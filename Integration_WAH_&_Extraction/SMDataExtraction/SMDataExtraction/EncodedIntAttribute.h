#pragma once
#include <vector>
#include "VBitStream.h"
#include "AttributeType.h"
#include "EncodedAttributeInfo.h"

using namespace std;

class EncodedIntAttribute : public EncodedAttributeInfo
{
public:
	__declspec(dllexport) EncodedIntAttribute(void);
	__declspec(dllexport) ~EncodedIntAttribute(void);
 	__declspec(dllexport) vector<bool> SignBitMap();
	__declspec(dllexport) long maxAttVal(){return this->_maxVal;}
	__declspec(dllexport) long minAttVal(){return this->_minVal;}
	__declspec(dllexport) void setMaxVal(long val){this->_maxVal = val;}
	__declspec(dllexport) void setMinVal(long val){this->_minVal = val;}
	__declspec(dllexport) int decodeTheTuple(int tupleID);
	__declspec(dllexport) int getTheSignOf(int tupleID);
	__declspec(dllexport) void setTheSignBitMap(vector<long int> values,int valSet);

private:
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.
	long _maxVal;
	long _minVal;
};
