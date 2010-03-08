#include "StdAfx.h"
#include "VBitStream.h"

VBitStream::VBitStream(int bitCount)
{
	this->setBitCount(bitCount);
}

VBitStream::VBitStream(){

}

VBitStream::~VBitStream(void)
{
}

// void VBitStream::printVector(){
// 	
// }

// void VBitStream::setCompressedStream(std::vector<unsigned long int> &x){
// 
// }

// void VBitStream::printCompressedStream(){
// 	
// }

// void VBitStream::printArray(){
// 
// }

// vector<unsigned long int>& VBitStream::getCompressedVector(){
// 	vector<unsigned long int> temp;
// 	return temp;
// }

dynamic_bitset<> VBitStream::Decompress(){
	return this->getProcessedBitStream();
}

void VBitStream::CompressWords(boost::dynamic_bitset<>& bitMap){
this->convert(bitMap);
}

// void VBitStream::flip(){
// 
// }

// int VBitStream::getMainArrayLength(){
// 	return this->BitCount();
// }

// dynamic_bitset<> VBitStream::getCompressedWord(){
// 	return this->getProcessedBitStream();
// }

// int VBitStream::getValue(boost::dynamic_bitset<> &bitMap, int startIndex, int offset){
// 	return this->JUNK_VAL;
// }

int VBitStream::GetSpaceUtilisation(){
	return this->JUNK_VAL;
}

BitStreamInfo* VBitStream::operator ~(){
	return new VBitStream();
}

BitStreamInfo* VBitStream::operator &(BitStreamInfo &){
	return new VBitStream();
}

BitStreamInfo* VBitStream::operator |(BitStreamInfo &){
	return new VBitStream();
}

unsigned long long VBitStream::Count(){
	return (unsigned long long)this->count();
}

// const unsigned long  int VBitStream::ActiveWordSize(){
// 	return this->getProcessedBitStream().size();
// }
// 
// void VBitStream::ActiveWordSize(unsigned long int val){
// 	
// }
// 
// unsigned long int VBitStream::ActiveWord(){
// 	return this->getProcessedBitStream().size();
// }
// 
// void VBitStream::ActiveWord(unsigned long val){
// 
// }
// 
// int VBitStream::MainArraySize() const{
// 	return this->JUNK_VAL;
// }
// 
// void VBitStream::MainArraySize(int val){
// 
// }

// int VBitStream::oneCount(){
// 	return this->_dataBitStream.count();
// }
// 
// dynamic_bitset<> VBitStream::BitStream(){
// 	return this->_dataBitStream;
// }
// 
// string VBitStream::bitStreamAggregation(){
// 	return this->_bitAggregation;
// }
// 
// int VBitStream::bitStreamAllocAttID(){
// 	return this->_bitStreamAllocAttID;
// }
// 
// void VBitStream::setBitValue(int pos, bool val){
// 	this->_dataBitStream[pos] = val;
// }
// 
// void VBitStream::setBitStreamAggregation(string bitAgg){
// 	this->_bitAggregation = bitAgg;
// }
// 
// void VBitStream::setBitStreamAllocAttID(int attID){
// 	this->_bitStreamAllocAttID = attID;
// }
// 
// int VBitStream::getBitCount(){
// 	return this->_bitCount;
// }
// 
// void VBitStream::setBitCount(int bitCount){
// 	this->_bitCount = bitCount;
// 	this->_dataBitStream.resize(bitCount);
// }



