#include "StdAfx.h"
#include "CompressionHandler.h"

CompressionHandler::CompressionHandler(void)
{
}

CompressionHandler::~CompressionHandler(void)
{
}
void CompressionHandler::ConvertTo( WrapDataSource * _pdata_source, BitStreamInfo::vertical_bit_type _end_type )
{
	vector<EncodedAttributeInfo * > attributes = _pdata_source->codedAttributes();
	vector<EncodedAttributeInfo * >::iterator iter = attributes.begin();
	while(iter != attributes.end())
	{

		ConvertAttributeTo(*(iter),_end_type);
		iter++;
	}
}
void CompressionHandler::ConvertAttributeTo( EncodedAttributeInfo * _attribute,BitStreamInfo::vertical_bit_type _end_type )
{
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
BitStreamInfo * CompressionHandler::ConvertBitStreamTo( BitStreamInfo * _bit_stream,BitStreamInfo::vertical_bit_type _end_type )
{
	dynamic_bitset<> bit_stream = _bit_stream->Decompress();	
	delete _bit_stream;
	switch(_end_type)
	{
	case BitStreamInfo::WAH_COMPRESSION:
		{
			
			_bit_stream = new WAHStructure();
			_bit_stream->Type(BitStreamInfo::WAH_COMPRESSION);
			break;
		}
	case BitStreamInfo::VERTICAL_STREAM_FORMAT:
		{
			_bit_stream = new VBitStream();
			_bit_stream->Type(BitStreamInfo::VERTICAL_STREAM_FORMAT);
			break;
		}

	default:{
		break;
			}
	}
	_bit_stream->CompressWords(bit_stream);
	return _bit_stream;
}
