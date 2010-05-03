#include "StdAfx.h"
#include "Commons.h"
#include "EncodedDoubleAttribute.h"

Commons::Commons(void)
{
}

Commons::~Commons(void)
{
}

/*
BitStreamInfo * Commons::UGreaterThan( EncodedAttributeInfo * attribute, unsigned long value,int rows )
{
	BitStreamInfo * info = NULL;	
	switch(attribute->attributeType())
	{
	case SIGNEDINT_VAL:
		{
			info = UGreaterThanInt(attribute,value,rows);
			break;
		}
	case DOUBLE_VAL:
		{
			EncodedDoubleAttribute * double_att =  static_cast<EncodedDoubleAttribute *>(attribute);
			long precission = double_att->Precision();

			info = UGreaterThanInt(attribute,value * precission , rows);
			break;
		}
	case DATE_VAL:
		{
			assert(false);

		}
	case MULTICAT_VAL:
		{
			assert(false);

		}
	}
	return info;
}

BitStreamInfo * Commons::UGreaterThanInt(EncodedAttributeInfo * attribute,unsigned long input_value,int noOfRows)
{
	dynamic_bitset<> bit_set(noOfRows);
	BitStreamInfo * bit_stream = BitStreamGenerator(attribute,bit_set);
	dynamic_bitset<> value_pattern((int)attribute->NoOfVBitStreams(),input_value);

	size_t k=0;
	while (value_pattern[k] == 1 && k < bit_set.size())
		k=k+1;

	if (k < value_pattern.size())
		bit_stream = attribute->bitStreamAt(k)->Clone();

	BitStreamInfo * prev_val = NULL;

	for (size_t i=k+1; i < value_pattern.size(); i++)
	{
		prev_val = bit_stream;
		if (value_pattern[i] == 1)
		{
			bit_stream = *(bit_stream) & *(attribute->bitStreamAt(i));
		}
		else 
		{
			bit_stream = *(bit_stream) | *(attribute->bitStreamAt(i));
		}
		delete prev_val;
	}
	return bit_stream;

}

BitStreamInfo * Commons::BitStreamGenerator(EncodedAttributeInfo * attribute,dynamic_bitset<> & _bit_stream)
{

	BitStreamInfo::vertical_bit_type type = attribute->bitStreamAt(0)->Type();		   
	BitStreamInfo * new_stream = NULL;
	switch(type)
	{
	case BitStreamInfo::VERTICAL_STREAM_FORMAT :
		{
			new_stream = new VBitStream();	   
		}
		break;
	case BitStreamInfo::WAH_COMPRESSION:
		{
			new_stream = new WAHStructure();
		}
		break;
	case BitStreamInfo::EWAH_COMPRESSION:
		{
			new_stream = new EWAH();
		}
		break;
	}
	new_stream->CompressWords(_bit_stream);
	return new_stream;
}

BitStreamInfo * Commons::UEq( EncodedAttributeInfo * attribute, unsigned long value )
{
	switch(attribute->attributeType())
	{
	case SIGNEDINT_VAL:
		{
			dynamic_bitset<> pattern_val((int)attribute->NoOfVBitStreams(),value);
			return FindPattern(pattern_val,attribute->vBitStreams());
		}
		break;

	case DOUBLE_VAL:
		{
			EncodedDoubleAttribute * double_att = static_cast<EncodedDoubleAttribute *>(attribute);
			value = value * double_att->Precision();
			dynamic_bitset<> pattern_val((int)attribute->NoOfVBitStreams(),value);
			return FindPattern(pattern_val,attribute->vBitStreams());
		}
	}

}*/