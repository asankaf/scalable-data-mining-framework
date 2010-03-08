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
	__declspec(dllexport) int decodeTheTuple(int tupleID);
	__declspec(dllexport) int getTheSignOf(int tupleID);
	__declspec(dllexport) void setTheSignBitMap(long int *values,int valSet);

private:
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.

};
