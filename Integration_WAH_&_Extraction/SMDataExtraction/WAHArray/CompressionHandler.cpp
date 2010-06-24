#include "StdAfx.h"
#include "CompressionHandler.h"
#include "smcompressionexceptions.h"
#include "ExceptionCodes.h"
#include "nullcomp.h"
CompressionHandler::CompressionHandler(void)
{
}

CompressionHandler::~CompressionHandler(void)
{
}
void CompressionHandler::ConvertTo( WrapDataSource * _pdata_source, BitStreamInfo::vertical_bit_type _end_type ) throw(compression_exception)
{
	if (_pdata_source == NULL)
	{
		BOOST_THROW_EXCEPTION(null_operand_exception(SM2002));
	}
	vector<EncodedAttributeInfo * > attributes = _pdata_source->codedAttributes();
	vector<EncodedAttributeInfo * >::iterator iter = attributes.begin();
	while(iter != attributes.end())
	{

		ConvertAttributeTo(*(iter),_end_type);
		iter++;
	}
}
void CompressionHandler::ConvertAttributeTo( EncodedAttributeInfo * _attribute,BitStreamInfo::vertical_bit_type _end_type ) throw (compression_exception)
{
	if (_attribute == NULL)
	{
		BOOST_THROW_EXCEPTION(null_operand_exception(SM2002));
	}
	vector<BitStreamInfo *> bit_streams = _attribute->vBitStreams();
	vector<BitStreamInfo *> new_bits;
	vector<BitStreamInfo *>::iterator iter = bit_streams.begin();
	while(iter != bit_streams.end())
	{

		new_bits.push_back(ConvertBitStreamTo(*(iter),_end_type));
		iter++;
	}
	_attribute->setVBitStreams(new_bits);
}
BitStreamInfo * CompressionHandler::ConvertBitStreamTo( BitStreamInfo * _bit_stream,BitStreamInfo::vertical_bit_type _end_type ) throw(compression_exception)
{
	if (_bit_stream == NULL)
	{
		BOOST_THROW_EXCEPTION(null_operand_exception(SM2002));
	}

	//Determine whether the old BitStreamInfo pointers needed to  be deleted
	dynamic_bitset<> bit_stream = _bit_stream->Decompress();
	BitStreamInfo * new_val = NULL;
	switch(_end_type)
	{
	case BitStreamInfo::WAH_COMPRESSION:
		{

			new_val = new WAHStructure();
			new_val->Type(BitStreamInfo::WAH_COMPRESSION);
			break;
		}
	case BitStreamInfo::VERTICAL_STREAM_FORMAT:
		{		
			new_val = new VBitStream();
			new_val->Type(BitStreamInfo::VERTICAL_STREAM_FORMAT);
			break;
		}
	case BitStreamInfo::EWAH_COMPRESSION:
		{
			new_val = new EWAH();
			new_val->Type(BitStreamInfo::EWAH_COMPRESSION);
			break;
		}
	
	default:{
		invalid_compression_type_exception comp(SM2001);
		BOOST_THROW_EXCEPTION(comp);
		break;
			}
	}
	//Each bitstream is compressed by the format defined in the particular structure
	new_val->CompressWords(bit_stream);	

	//Information in the old bitstream is copied to the new bitstream
	_bit_stream->Clone(new_val);
	delete _bit_stream;
	return new_val;
}
