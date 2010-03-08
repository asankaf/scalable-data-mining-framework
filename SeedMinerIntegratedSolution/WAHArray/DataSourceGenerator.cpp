#include "StdAfx.h"
#include "DataSourceGenerator.h"


DataSourceGenerator::DataSourceGenerator(void)
{
}

DataSourceGenerator::~DataSourceGenerator(void)
{
}

EncodedAttributeInfo * DataSourceGenerator::CreateAttribute(std::vector<string> &_string_vector,BitStreamInfo::vertical_bit_type _type)
{
	EncodedAttributeInfo * pattribute = new EncodedAttributeInfo();
	vector<string>::iterator iter = _string_vector.begin();
	vector<BitStreamInfo *> bit_streams;
	while(iter != _string_vector.end())
	{
		bit_streams.push_back(CreateBitStreamInfo(*(iter),_type));
		iter++;
	}
	pattribute->setVBitStreams(bit_streams);
	return pattribute;
}

BitStreamInfo * DataSourceGenerator::CreateBitStreamInfo(std::string &_bit_stream, BitStreamInfo::vertical_bit_type _type)
{
	BitsetGenerator bg;
	dynamic_bitset<> temp = bg.buildFromString(_bit_stream);
	BitStreamInfo * result = NULL;
	if (_type == BitStreamInfo::VERTICAL_STREAM_FORMAT)
	{
		result = new VBitStream();
	}else
	{
		result = new WAHStructure();
	}
	result->CompressWords(temp);
	return result;
}

EncodedAttributeInfo * DataSourceGenerator::CreateAttribute(std::vector<dynamic_bitset<> > &_bitsets, BitStreamInfo::vertical_bit_type _type)
{
	EncodedAttributeInfo * pAttribute = new EncodedAttributeInfo();
	vector<dynamic_bitset<>>::iterator iter = _bitsets.begin();
	vector<BitStreamInfo *> bitstreams;
	while(iter != _bitsets.end())
	{
		bitstreams.push_back(CreateBitStreamInfo(*(iter),_type));
		iter++;
	}
	pAttribute->setVBitStreams(bitstreams);
	return pAttribute;
}

BitStreamInfo * DataSourceGenerator::CreateBitStreamInfo(boost::dynamic_bitset<> &_bit_stream, BitStreamInfo::vertical_bit_type _type)
{
	BitStreamInfo * presult  = NULL;
	if (_type == BitStreamInfo::VERTICAL_STREAM_FORMAT)
	{
		presult = new VBitStream();
		presult->Type(BitStreamInfo::VERTICAL_STREAM_FORMAT);
		
	}else if(_type == BitStreamInfo::WAH_COMPRESSION)
	{
		presult = new WAHStructure();
		presult->Type(BitStreamInfo::WAH_COMPRESSION);		
	}
	presult->CompressWords(_bit_stream);
	return presult;
}
WrapDataSource * DataSourceGenerator::CreateDataSource(std::vector<vector<dynamic_bitset<>>> &_input,BitStreamInfo::vertical_bit_type _type)
{
	WrapDataSource * pdata_source = new WrapDataSource();
	vector<EncodedAttributeInfo *> temp;
	for (int i = 0; i < _input.size() ; i++)
	{
		vector<dynamic_bitset<>> bitsets = _input.at(i);
		temp.push_back(CreateAttribute(bitsets,_type));
	}
	pdata_source->CodedAtts(temp);
	return pdata_source;
}