#pragma once
#include "EncodedAttributeInfo.h"
#include "CompressionHandler.h"
#include "BitsetGenerator.h"
#include "boost/dynamic_bitset.hpp"
#include "VBitStream.h"
#include "BitStreamInfo.h"
#include <vector>
using namespace CompressedStructure;
using namespace CompressedStructure::TestStructure;
class DataSourceGenerator
{
public:
	DataSourceGenerator(void);
	~DataSourceGenerator(void);
	WrapDataSource * CreateDataSource(std::vector<vector<dynamic_bitset<>>> &_input,BitStreamInfo::vertical_bit_type _type);
	BitStreamInfo * CreateBitStreamInfo(string & _bit_stream,BitStreamInfo::vertical_bit_type _type);
	BitStreamInfo * CreateBitStreamInfo(dynamic_bitset<> & _bit_stream,BitStreamInfo::vertical_bit_type _type);
	EncodedAttributeInfo * CreateAttribute(std::vector<string> &_string_vector,BitStreamInfo::vertical_bit_type _type);
	EncodedAttributeInfo * CreateAttribute(vector<dynamic_bitset<>> &_bitsets,BitStreamInfo::vertical_bit_type _type);
};
