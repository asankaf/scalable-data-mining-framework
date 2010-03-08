#include "StdAfx.h"
#include "BitStreamHolder.h"

namespace Algorithm{
	BitStreamHolder::BitStreamHolder(void)
	{
	}

	BitStreamHolder::~BitStreamHolder(void)
	{
	}

	void BitStreamHolder::AddAttributeID(int _att_id)
	{
		m_attribute_no.push_back(_att_id);		
	}

	void BitStreamHolder::AddStreamNo(int _stream_no)
	{
		m_bit_stream_no.push_back(_stream_no);
	}

	bool BitStreamHolder::Contains(int _value, std::vector<int> &_container)
	{
		
		sort(_container.begin(),_container.end());
		return binary_search(_container.begin(),_container.end(),_value);
		
	}

	bool BitStreamHolder::ContainsAttID(int _att_id)
	{
		return Contains(_att_id,m_attribute_no);
	}

	bool BitStreamHolder::ContainsStreamNo(int _stream_no)
	{
		return Contains(_stream_no,m_bit_stream_no);
	}
}