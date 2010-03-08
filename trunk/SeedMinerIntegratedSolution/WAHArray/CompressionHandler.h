#pragma once
#include "WrapDataSource.h"
#include "DataSources.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include "WAHStructure.h"
#include <vector>


using namespace CompressedStructure;
//using namespace CompressedStructure::TestStructure;

class CompressionHandler
{
public:	
	void ConvertTo(WrapDataSource * _pdata_source, BitStreamInfo::vertical_bit_type _end_type);
	CompressionHandler(void);
	~CompressionHandler(void);

private :	
	void ConvertAttributeTo(EncodedAttributeInfo * _attribute,BitStreamInfo::vertical_bit_type _end_type);
	BitStreamInfo * ConvertBitStreamTo( BitStreamInfo * _bit_stream,BitStreamInfo::vertical_bit_type _end_type );
};
