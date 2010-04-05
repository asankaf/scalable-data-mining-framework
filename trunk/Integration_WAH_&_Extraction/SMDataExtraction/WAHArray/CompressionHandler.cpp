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
	//Determine whether the old BitStreamInfo pointers needed to  be deleted
	dynamic_bitset<> bit_stream = _bit_stream->Decompress();
	BitStreamInfo * new_val = NULL;
	//delete _bit_stream;
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
		break;
			}
	}
	new_val->CompressWords(bit_stream);	
	_bit_stream->Clone(new_val);
//	new_val->Decompress();
//	new_val->Print();
	if (_end_type != BitStreamInfo::EWAH_COMPRESSION)
	{
		delete _bit_stream;
	}
	
	return new_val;
}
