#pragma once
#include "boost/dynamic_bitset.hpp"
#include <vector>
#include <string>
#include <iostream>

#ifndef BIT_STREAM_HOLDER_H
#define BIT_STREAM_HOLDER_H

using namespace boost;
using namespace std;

class BitStreamInfo{
public:
	static enum vertical_bit_type{WAH_COMPRESSION,VERTICAL_STREAM_FORMAT,EWAH_COMPRESSION};
	__declspec(dllexport) BitStreamInfo();
	__declspec(dllexport) BitStreamInfo(int bitCount);
	__declspec(dllexport) virtual ~BitStreamInfo();	
	
	__declspec(dllexport) virtual dynamic_bitset<> Decompress() = 0;
	__declspec(dllexport) virtual void CompressWords(boost::dynamic_bitset<>& bitMap) = 0;
	__declspec(dllexport) virtual unsigned long long Count() = 0;
	__declspec(dllexport) virtual BitStreamInfo* operator~() = 0;
	__declspec(dllexport) virtual BitStreamInfo* operator &(BitStreamInfo &) = 0;
	__declspec(dllexport) virtual BitStreamInfo* operator |(BitStreamInfo &) = 0;
	__declspec(dllexport) virtual size_t SpaceUtilisation() = 0;

	__declspec(dllexport) dynamic_bitset<> getProcessedBitStream();
	__declspec(dllexport) unsigned long long count(){return this->_decompressedVBitStream.count();}
	__declspec(dllexport) void convert(dynamic_bitset<> bitStream);
	__declspec(dllexport) void setBitValue(int pos,bool val) {this->_decompressedVBitStream[pos] = val;}
	__declspec(dllexport) vector<int> getActiveBitIDs();
	__declspec(dllexport) int BitCount() {return this->_bitCount;}
	__declspec(dllexport) void setBitCount(int bitCount){this->_bitCount = bitCount;
							//this->_decompressedVBitStream.resize(bitCount);
	}
	__declspec(dllexport) int BitStreamAllocAttID() {return this->_bitStreamAllocAttID;}
	__declspec(dllexport) void setBitStreamAllocAttID(int attID){this->_bitStreamAllocAttID = attID;}
	__declspec(dllexport) string BitStreamAllocAttName() {return this->_bitStreamAllocAttName;}
	__declspec(dllexport) void setBitStreamAllocAttName(string attName){this->_bitStreamAllocAttName = attName;}
	__declspec(dllexport) BitStreamInfo::vertical_bit_type Type();
	__declspec(dllexport) void Type(BitStreamInfo::vertical_bit_type val);
	__declspec(dllexport) virtual void Print(void);

	//Copies the current state of the object to object reffered by _new_stream
	__declspec(dllexport) void Clone(BitStreamInfo * _new_stream);

	//Provides a current copy of the object
	__declspec(dllexport) virtual BitStreamInfo * Clone()=0;	
	
private:
	vertical_bit_type m_type;	
	int _bitCount;
	dynamic_bitset<> _decompressedVBitStream;
	int _bitStreamAllocAttID;
	string _bitStreamAllocAttName;
};
#endif