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
	
	__declspec(dllexport) dynamic_bitset<> Decompress();
	__declspec(dllexport) void CompressWords(boost::dynamic_bitset<>& bitMap);
	__declspec(dllexport) unsigned long long Count();	
	__declspec(dllexport) size_t SpaceUtilisation();
	__declspec(dllexport) BitStreamInfo* operator~();
	__declspec(dllexport) BitStreamInfo* operator &(BitStreamInfo &);
	__declspec(dllexport) BitStreamInfo* operator |(BitStreamInfo &);

	/** Overridden method declared in the superclass.
	Provides a clone of the VBitStream object **/
	__declspec(dllexport) BitStreamInfo* Clone();


private:
	static const int JUNK_VAL = 2147483647;
};
