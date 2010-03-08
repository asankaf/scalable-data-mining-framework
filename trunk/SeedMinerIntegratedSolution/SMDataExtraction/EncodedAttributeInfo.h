#pragma once

#include "VBitStream.h"
#include "AttributeType.h"
#include "boost/dynamic_bitset.hpp"

#include <vector>

using namespace std;
using namespace boost;

class EncodedAttributeInfo{
public:
	__declspec(dllexport) EncodedAttributeInfo();
	__declspec(dllexport) ~EncodedAttributeInfo();
	__declspec(dllexport) vector<BitStreamInfo*> vBitStreams();
	__declspec(dllexport) string attributeName();
	__declspec(dllexport) int attributeID();
	__declspec(dllexport) int NoOfVBitStreams();
	__declspec(dllexport) ATT_TYPE attributeType();
	__declspec(dllexport) void setVBitStreams(vector<BitStreamInfo*> vBitset);
	__declspec(dllexport) void setNoOfVBitStreams(int novBitsets,int rows);
	__declspec(dllexport) void setAttType(ATT_TYPE type);
	__declspec(dllexport) void setAttName(string name);
	__declspec(dllexport) void setAttID(int attID);
	__declspec(dllexport) void setVBitStreamSize(int newSize);
	__declspec(dllexport) BitStreamInfo* bitStreamAt(int bitStreamID);
	__declspec(dllexport) BitStreamInfo* operator()(const int bitStreamID);
	
	template<typename T>
	__declspec(dllexport) T decodeTheTuple(int tupleID) = 0;


private:
	vector<BitStreamInfo*> _vBitStreams;
	ATT_TYPE _attType;
	int _attID;
	string _attName;
	int _noOfVBitStreams;
};