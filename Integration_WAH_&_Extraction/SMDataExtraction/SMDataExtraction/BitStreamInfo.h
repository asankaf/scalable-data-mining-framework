#pragma once
#include "boost/dynamic_bitset.hpp"
#include "SEEDMinerExceptions.h"

#include <vector>
#include <string>
#include <iostream>

#ifndef BIT_STREAM_HOLDER_H
#define BIT_STREAM_HOLDER_H

using namespace boost;
using namespace std;

/************************************************************************/
/*    Class  :BitStreamInfo.h
/*	  Started:18.02.2010 21:45:12
/*    Updated:18.02.2010 21:45:12
/*    Author :SEEDMiner
/*    Subj   :This is the most fine grained class in SEEDMiner. This is 
/*			  the most basic abstract class to handle bit level operations.
/*    Version: 
/************************************************************************/

class BitStreamInfo{
public:
	/** This is the enum to store the type of bit representation in which the bitstreams are processed*/
	static enum vertical_bit_type{WAH_COMPRESSION,VERTICAL_STREAM_FORMAT,EWAH_COMPRESSION,NULL_COMP};

	#pragma region constructors & destructor

	__declspec(dllexport) BitStreamInfo();
	__declspec(dllexport) BitStreamInfo(int bitCount);
	__declspec(dllexport) virtual ~BitStreamInfo();	

	#pragma endregion constructors & destructor
	
	
	/** Abstract Method to decompress the bitstream if the particular bitstream is already compressed*/
	/** If the bitstream is not in compressed version, then also it will simply returns the uncompressed bitstream*/
	__declspec(dllexport) virtual dynamic_bitset<> Decompress() = 0;

	/** Abstract Method to compress the bitset when the bitset is provided as a boost dynamic_bitset*/
	__declspec(dllexport) virtual void CompressWords(boost::dynamic_bitset<>& bitMap) = 0;

	/** Abstract Method to return number of 1's available in a particular bitstream*/
	__declspec(dllexport) virtual unsigned long long Count() = 0;

	/** Abstract Operator to return the inversion of the bits in the current bitstream*/
	/** The method will simply return a same type of bitstream with bits inverted   */
	__declspec(dllexport) virtual BitStreamInfo* operator ~() = 0;

	/** Abstract Operator to perform AND operation between two bitstreams, Input to this*/
	/** operator is the other bitstream and this will simply return another bitstream   */
	__declspec(dllexport) virtual BitStreamInfo* operator &(BitStreamInfo &) = 0;

	/** Abstract Operator to perform OR operation between two bitstreams, Input to this */
	/** operator is the other bitstream and this will simply return another bitstream   */
	__declspec(dllexport) virtual BitStreamInfo* operator |(BitStreamInfo &) = 0;

	/** Abstract method to return the amount of space used by the particular bitstream*/
	__declspec(dllexport) virtual size_t SpaceUtilisation() = 0;

	/** Method to return the processing bit stream as a dynamic_bitset*/
	__declspec(dllexport) dynamic_bitset<> getProcessedBitStream();

	/** Method to return number of 1's available in a particular bitstream*/
	__declspec(dllexport) unsigned long long count(){return this->_decompressedVBitStream.count();}

	/** setter of the decompressedVBitStream instance variable*/
	__declspec(dllexport) void convert(dynamic_bitset<> bitStream);

	/** Method to set a bit given the position and value to set in the bitstream that is being processing*/
	__declspec(dllexport) void setBitValue(int pos,bool val) {this->_decompressedVBitStream[pos] = val;}
	
	/** Method to return the zero based indices of 1's in the processed bitstream*/
	__declspec(dllexport) vector<int> getActiveBitIDs() throw (error_vector_out_of_range);

	#pragma region Getters & Setters
	
	__declspec(dllexport) int BitCount() {return this->_bitCount;}
	__declspec(dllexport) void setBitCount(int bitCount){this->_bitCount = bitCount;}
	__declspec(dllexport) int BitStreamAllocAttID() {return this->_bitStreamAllocAttID;}
	__declspec(dllexport) void setBitStreamAllocAttID(int attID){this->_bitStreamAllocAttID = attID;}
	__declspec(dllexport) string BitStreamAllocAttName() {return this->_bitStreamAllocAttName;}
	__declspec(dllexport) void setBitStreamAllocAttName(string attName){this->_bitStreamAllocAttName = attName;}
	__declspec(dllexport) BitStreamInfo::vertical_bit_type Type();
	__declspec(dllexport) void Type(BitStreamInfo::vertical_bit_type val);

	#pragma endregion Getters & Setters


	/** Method to print the bitstream as a dynamic_bitset as a bitstream*/
	__declspec(dllexport) virtual void Print(void);

	/** Copies the current state of the object to object referred by _new_stream*/
	__declspec(dllexport) void Clone(BitStreamInfo * _new_stream);

	/** Provides a current copy of the object*/
	__declspec(dllexport) virtual BitStreamInfo * Clone()=0;	
	
private:
	vertical_bit_type m_type;	
	int _bitCount;
	dynamic_bitset<> _decompressedVBitStream;
	int _bitStreamAllocAttID;
	string _bitStreamAllocAttName;
};
#endif