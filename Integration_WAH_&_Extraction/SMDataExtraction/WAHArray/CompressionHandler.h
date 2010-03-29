#pragma once
#include "WrapDataSource.h"
#include "DataSources.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include "WAHStructure.h"
#include "EWAH.h"
#include <vector>


using namespace CompressedStructure;
//using namespace CompressedStructure::TestStructure;

class CompressionHandler
{
public:	
	_declspec(dllexport) void ConvertTo(WrapDataSource * _pdata_source, BitStreamInfo::vertical_bit_type _end_type);
	_declspec(dllexport) CompressionHandler(void);
	_declspec(dllexport) ~CompressionHandler(void);

private :	
	void ConvertAttributeTo(EncodedAttributeInfo * _attribute,BitStreamInfo::vertical_bit_type _end_type);
	BitStreamInfo * ConvertBitStreamTo( BitStreamInfo * _bit_stream,BitStreamInfo::vertical_bit_type _end_type );
};
