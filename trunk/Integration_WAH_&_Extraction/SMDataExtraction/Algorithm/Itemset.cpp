#include "StdAfx.h"
#include "Itemset.h"

Itemset::Itemset(void)
{
}

Itemset::~Itemset(void)
{
}

int Itemset::GetHashValue(int _no_of_attrib,int * _int_arr)
{
	long result = 0;
	int factor = 1;
	int val = 0;
	for (size_t i = 0; i< _no_of_attrib; i++)
	{
		val = _int_arr[i]+1;		
		result +=  val * factor;
		factor *= 10;
	}

	return (int)result;
}

int Itemset::HashCode()
{
	return GetHashValue(m_attribute_no,m_items);
}

