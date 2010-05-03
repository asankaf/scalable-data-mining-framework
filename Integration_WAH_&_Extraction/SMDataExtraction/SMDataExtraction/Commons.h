#include "BitStreamInfo.h"
#include "EncodedAttributeInfo.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#pragma once

class Commons
{
public:
	Commons(void);
	~Commons(void);
	
	template <typename iter_type>
	static void DeleteVector(iter_type first, iter_type last)
	{
		for (iter_type cur = first; cur != last; ++cur)
		{
			if((*cur) != NULL)
			{
				delete *cur;
				*cur = NULL;
			}
		}
	}

	template <typename iter_type>
	static void InitVector(iter_type first, iter_type last)
	{
		for (iter_type cur = first; cur != last; ++cur)
		{

			*cur = NULL;
		}
	}

	/*
	BitStreamInfo * BitStreamGenerator(EncodedAttributeInfo * attribute,dynamic_bitset<> & _bit_stream);
		BitStreamInfo * UGreaterThan( EncodedAttributeInfo * attribute, unsigned long value,int rows );
		BitStreamInfo * UGreaterThanInt(EncodedAttributeInfo * attribute,unsigned long input_value,int noOfRows);*/
	
};
