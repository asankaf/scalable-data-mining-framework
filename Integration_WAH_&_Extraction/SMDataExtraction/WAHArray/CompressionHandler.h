#pragma once
#include "WrapDataSource.h"
#include "DataSources.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include "WAHStructure.h"
#include "EWAH.h"
#include <vector>
#include "smcompressionexceptions.h"


using namespace CompressedStructure;

class CompressionHandler
{
public:	
	/** Compresses a data source to the compression format specified by _end_type*/
	_declspec(dllexport) static void ConvertTo(WrapDataSource * _pdata_source, BitStreamInfo::vertical_bit_type _end_type) throw(compression_exception);
	
	_declspec(dllexport) CompressionHandler(void);
	
	_declspec(dllexport) ~CompressionHandler(void);

private :	
	/**Compresses an attribute to the compression format specified by _end_type*/
	static void ConvertAttributeTo(EncodedAttributeInfo * _attribute,BitStreamInfo::vertical_bit_type _end_type) throw (compression_exception);

	/**Compresses a bitstream to the compression format specified by _end_type*/
	static BitStreamInfo * ConvertBitStreamTo( BitStreamInfo * _bit_stream,BitStreamInfo::vertical_bit_type _end_type ) throw(compression_exception);
};
