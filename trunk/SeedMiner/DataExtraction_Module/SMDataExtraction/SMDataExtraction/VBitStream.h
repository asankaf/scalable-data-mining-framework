#pragma once

#include <bitset>
#include <vector>
#include <string>

using namespace std;

class VBitStream
{
public:
	__declspec(dllexport) VBitStream(int bitCount);
	__declspec(dllexport) ~VBitStream(void);
	__declspec(dllexport) vector<bool> BitStream();
	__declspec(dllexport) void setBitValue(int pos,bool val=false);
	__declspec(dllexport) void setBitStreamAggregation(string bitAgg);  
	__declspec(dllexport) int oneCount();
	__declspec(dllexport) string bitStreamAggregation();
	__declspec(dllexport) int bitStreamAllocAttID();
	__declspec(dllexport) void setBitStreamAllocAttID(int attID);

	
private:
	vector<bool> _dataBitStream;
	int _bitCount;
	string _bitAggregation;
	int _bitStreamAllocAttID;
};