#pragma once
#include "WrappedBitVector.h"
#include "boost/dynamic_bitset.hpp"
#include "BitStreamInfo.h"

class EWAH : public BitStreamInfo
{
public: 
	__declspec(dllexport) EWAH(void);
	__declspec(dllexport) EWAH(WrappedBitVector & _vect);
	__declspec(dllexport) ~EWAH(void);
	__declspec(dllexport) dynamic_bitset<> Decompress();
	__declspec(dllexport) void CompressWords(boost::dynamic_bitset<>& bitMap);
	__declspec(dllexport) unsigned long long Count();
	__declspec(dllexport) BitStreamInfo* operator~();
	__declspec(dllexport) BitStreamInfo* operator &(BitStreamInfo &);
	__declspec(dllexport) BitStreamInfo* operator |(BitStreamInfo &);
	__declspec(dllexport) size_t SpaceUtilisation();
	__declspec(dllexport) void EWAH::Print();
	__declspec(dllexport) BitStreamInfo * Clone();


private:
	WrappedBitVector m_bitvector;
};
