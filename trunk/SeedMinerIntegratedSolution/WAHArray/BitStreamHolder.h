#pragma once
#include <vector>
#include "BitStreamInfo.h"
#include <iostream>
#include <algorithm>
using namespace std;
namespace Algorithm{
	class BitStreamHolder
	{
	public:
		BitStreamHolder(void);
		~BitStreamHolder(void);
		BitStreamInfo * Bit_stream() const { return m_bit_stream; }
		void Bit_stream(BitStreamInfo * val) { m_bit_stream = val; }
		void AddAttributeID(int _att_id);
		bool ContainsAttID(int _att_id);
		void AddStreamNo(int _stream_no);
		bool ContainsStreamNo(int _stream_no);
		vector<int> Attribute_no() const { return m_attribute_no; }
		void Attribute_no(vector<int> val) { m_attribute_no = val; }
		vector<int> Bit_stream_no() const { return m_bit_stream_no; }
		void Bit_stream_no(vector<int> val) { m_bit_stream_no = val; }				

	private:
		vector<int> m_attribute_no;		
		vector<int> m_bit_stream_no;		
		BitStreamInfo * m_bit_stream;
		
		bool Contains(int _value,vector<int> & _container);
		
	};


}