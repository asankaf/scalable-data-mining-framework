#pragma once

#include <bitset>
#include <vector>
#include <string>
#include <math.h>
#include <boost/dynamic_bitset.hpp>
#include "BitStreamInfo.h"

using namespace std;
using namespace boost;

class VBitStream : public BitStreamInfo
{
public:
	__declspec(dllexport) VBitStream(int bitCount);
	__declspec(dllexport) VBitStream();
	__declspec(dllexport) ~VBitStream(void);

// 	__declspec(dllexport) dynamic_bitset<> BitStream();
// 	__declspec(dllexport) void setBitValue(int pos,bool val = false);
// 	__declspec(dllexport) void setBitStreamAggregation(string bitAgg);  
// 	__declspec(dllexport) int oneCount();
// 	__declspec(dllexport) string bitStreamAggregation();
// 	__declspec(dllexport) int bitStreamAllocAttID();
// 	__declspec(dllexport) void setBitStreamAllocAttID(int attID);
// 	__declspec(dllexport) void setBitCount(int bitCount);
// 	__declspec(dllexport) int getBitCount();
//	__declspec(dllexport) void printVector();
//	__declspec(dllexport) void setCompressedStream(vector<unsigned long int> &x);
//	__declspec(dllexport) void printArray();
//	__declspec(dllexport) void printCompressedStream();
//	__declspec(dllexport) vector<unsigned long int>& getCompressedVector();
	__declspec(dllexport) dynamic_bitset<> Decompress();
	__declspec(dllexport) void CompressWords(boost::dynamic_bitset<>& bitMap);
	__declspec(dllexport) unsigned long long Count();
//	__declspec(dllexport) void flip();
//	__declspec(dllexport) int getMainArrayLength();
//	__declspec(dllexport) dynamic_bitset<> getCompressedWord();
//	__declspec(dllexport) int getValue(boost::dynamic_bitset<> & bitMap,int startIndex,int offset);
	__declspec(dllexport) int GetSpaceUtilisation();

	__declspec(dllexport) BitStreamInfo* operator~();

	__declspec(dllexport) BitStreamInfo* operator &(BitStreamInfo &);

	__declspec(dllexport) BitStreamInfo* operator |(BitStreamInfo &);

// 	__declspec(dllexport) virtual const unsigned long int ActiveWordSize();
// 	__declspec(dllexport) virtual void ActiveWordSize(unsigned long int val);
// 	__declspec(dllexport) virtual unsigned long int ActiveWord();
// 	__declspec(dllexport) virtual void ActiveWord(unsigned long int val);
// 	__declspec(dllexport) virtual int MainArraySize() const;
// 	__declspec(dllexport) virtual void MainArraySize(int val);
	
private:
	static const int JUNK_VAL = 2147483647;
// 	dynamic_bitset<> _dataBitStream;
// 	int _bitCount;
// 	string _bitAggregation;
// 	int _bitStreamAllocAttID;
};
