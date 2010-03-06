#include "stdafx.h"
#include "BitStreamInfo.h"

BitStreamInfo::BitStreamInfo(){

}

BitStreamInfo::BitStreamInfo(int bitCount){
	this->_bitCount = bitCount;
	this->_decompressedVBitStream.resize(bitCount);
}

BitStreamInfo::~BitStreamInfo(){

}
void BitStreamInfo::Print()
{
	cout <<this->Decompress()<< endl;
}

void BitStreamInfo::convert(dynamic_bitset<> bitStream){
	this->_decompressedVBitStream = bitStream;
}

dynamic_bitset<> BitStreamInfo::getProcessedBitStream(){
	return this->_decompressedVBitStream;
}

void BitStreamInfo::Type(BitStreamInfo::vertical_bit_type val)
{
	m_type = val;
}

BitStreamInfo::vertical_bit_type BitStreamInfo::Type()
{
	return m_type;
}

vector<int> BitStreamInfo::getActiveBitIDs(){
	vector<int> activeIDs;
	for (int i = 0 ; i < this->_decompressedVBitStream.size() ; i++)
	{
		if ((int)this->_decompressedVBitStream[i] == 1) activeIDs.push_back(i);
	}
	return activeIDs;
}