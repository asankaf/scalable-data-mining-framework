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

void VBitStream::printVector(){
	
}

void VBitStream::setCompressedStream(std::vector<unsigned long int> &x){

}

void VBitStream::printCompressedStream(){
	
}

void VBitStream::printArray(){

}

vector<unsigned long int>& VBitStream::getCompressedVector(){
	vector<unsigned long int> temp;
	return temp;
}

dynamic_bitset<> VBitStream::decompress(){
	return this->getProcessedBitStream();
}

void VBitStream::compressWords(boost::dynamic_bitset<>& bitMap){

}

void VBitStream::flip(){

}

int VBitStream::getMainArrayLength(){
	return this->BitCount();
}

dynamic_bitset<> VBitStream::getCompressedWord(){
	return this->getProcessedBitStream();
}

int VBitStream::getValue(boost::dynamic_bitset<> &bitMap, int startIndex, int offset){
	return this->JUNK_VAL;
}

int VBitStream::getSpaceUtilisation(){
	return this->JUNK_VAL;
}

BitStreamInfo* VBitStream::operator !(){
	return new VBitStream();
}

BitStreamInfo* VBitStream::operator &(BitStreamInfo &){
	return new VBitStream();
}

BitStreamInfo* VBitStream::operator |(BitStreamInfo &){
	return new VBitStream();
}

const short int VBitStream::ActiveWordSize(){
	return this->getProcessedBitStream().size();
}

void VBitStream::ActiveWordSize(short int val){
	
}

unsigned long int VBitStream::ActiveWord(){
	return this->getProcessedBitStream().size();
}

void VBitStream::ActiveWord(unsigned long val){

}

int VBitStream::MainArraySize() const{
	return this->JUNK_VAL;
}

void VBitStream::MainArraySize(int val){

}