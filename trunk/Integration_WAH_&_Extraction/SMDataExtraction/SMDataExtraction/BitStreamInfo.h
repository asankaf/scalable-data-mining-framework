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
	static enum vertical_bit_type{WAH_COMPRESSION,VERTICAL_STREAM_FORMAT};
	__declspec(dllexport) BitStreamInfo();
	__declspec(dllexport) BitStreamInfo(int bitCount);
	__declspec(dllexport) ~BitStreamInfo();
	//__declspec(dllexport) virtual void printVector() = 0;
	//__declspec(dllexport) virtual void setCompressedStream(vector<unsigned long int> &x)  = 0;
	//__declspec(dllexport) virtual void printArray() = 0;
	//__declspec(dllexport) virtual void printCompressedStream() = 0;
	//__declspec(dllexport) virtual vector<unsigned long int>& getCompressedVector() = 0;
	__declspec(dllexport) virtual dynamic_bitset<> Decompress() = 0;
	__declspec(dllexport) virtual void CompressWords(boost::dynamic_bitset<>& bitMap) = 0;
	__declspec(dllexport) virtual unsigned long long Count() = 0;
	//__declspec(dllexport) virtual void flip() = 0;
	//__declspec(dllexport) virtual int getMainArrayLength() = 0;
	//__declspec(dllexport) virtual dynamic_bitset<> getCompressedWord() = 0;
	//__declspec(dllexport) virtual int getValue(boost::dynamic_bitset<> & bitMap,int startIndex,int offset) = 0;
	

	__declspec(dllexport) virtual BitStreamInfo* operator~() = 0;

	__declspec(dllexport) virtual BitStreamInfo* operator &(BitStreamInfo &) = 0;

	__declspec(dllexport) virtual BitStreamInfo* operator |(BitStreamInfo &) = 0;

	//__declspec(dllexport) virtual const unsigned long int ActiveWordSize() = 0 ;
	//__declspec(dllexport) virtual void ActiveWordSize(unsigned long int val) = 0;
	//__declspec(dllexport) virtual unsigned long int ActiveWord() = 0;
	//__declspec(dllexport) virtual void ActiveWord(unsigned long int val) = 0;
	//__declspec(dllexport) virtual int MainArraySize() const = 0;
	//__declspec(dllexport) virtual void MainArraySize(int val) = 0;

	__declspec(dllexport) dynamic_bitset<> getProcessedBitStream();
	__declspec(dllexport) unsigned long long count(){return this->_decompressedVBitStream.count();}
	__declspec(dllexport) void convert(dynamic_bitset<> bitStream);
	__declspec(dllexport) void setBitValue(int pos,bool val) {this->_decompressedVBitStream[pos] = val;}
	__declspec(dllexport) vector<int> getActiveBitIDs();
	__declspec(dllexport) int BitCount() {return this->_bitCount;}
	__declspec(dllexport) void setBitCount(int bitCount){this->_bitCount = bitCount;this->_decompressedVBitStream.resize(bitCount);}
	__declspec(dllexport) int BitStreamAllocAttID() {return this->_bitStreamAllocAttID;}
	__declspec(dllexport) void setBitStreamAllocAttID(int attID){this->_bitStreamAllocAttID = attID;}
	__declspec(dllexport) string BitStreamAllocAttName() {return this->_bitStreamAllocAttName;}
	__declspec(dllexport) void setBitStreamAllocAttName(string attName){this->_bitStreamAllocAttName = attName;}
	__declspec(dllexport) BitStreamInfo::vertical_bit_type Type();
	__declspec(dllexport) void Type(BitStreamInfo::vertical_bit_type val);
	__declspec(dllexport) void Print(void);
	__declspec(dllexport) void Clone(BitStreamInfo * _new_stream);
	__declspec(dllexport) virtual size_t SpaceUtilisation() = 0;
	/*
	static const unsigned long int ZERO_LITERAL = 0;
	static const unsigned long int ONE_LITERAL = 2147483647;
	static const unsigned long int ONE_GAP_START_FLAG = 3221225472;
	static const unsigned long int ZERO_GAP_START_FLAG = 2147483648;
	static const int LITERAL_WORD = 0;
	static const int ZERO_GAP_WORD = 1;
	static const int ONE_GAP_WORD = 2;
	*/
private:
	vertical_bit_type m_type;	
	int _bitCount;
	dynamic_bitset<> _decompressedVBitStream;
	int _bitStreamAllocAttID;
	string _bitStreamAllocAttName;
};
#endif