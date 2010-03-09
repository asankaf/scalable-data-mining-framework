#pragma once
#include <vector>
#include "BitStreamInfo.h"
#include <iostream>
#include <algorithm>
#include <map>
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
		int Count();
		void DeleteBitStream();
		int Hash();
		void Print();
		BitStreamHolder * Merge(BitStreamHolder * _holder,map<int,int> & _index_attribute_map,vector<BitStreamHolder *> & _index_holder);
		vector<BitStreamHolder *> Difference(BitStreamHolder * _holder,map<int,int> & _index_attribute_map);
		//map<int,int> Index_attribute_map() const { return m_index_attribute_map; }
		//void Index_attribute_map(map<int,int> val) { m_index_attribute_map = val; }
		//int GetAttNoForIndex(int _index){return m[_index];}		


	private:
		vector<int> m_attribute_no;		
		vector<int> m_bit_stream_no;		
		BitStreamInfo * m_bit_stream;
		int m_count;
		bool Contains(int _value,vector<int> & _container);
		static const int MULTIPLICATIVE_UNIT = 10;
		
	};


}