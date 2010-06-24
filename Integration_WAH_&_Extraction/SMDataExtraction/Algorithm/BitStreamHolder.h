#ifndef _BITSTREAMHOLDER_H
#define _BITSTREAMHOLDER_H

#include <vector>
#include "BitStreamInfo.h"
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

	/*
	 * Holds the bit streams needed for the Apriori algorithm. This class 
	 * gives the ability to merge all the bit streams needed to merge
	 */
	class BitStreamHolder
	{
	public:
		_declspec(dllexport) BitStreamHolder(void);
		_declspec(dllexport) ~BitStreamHolder(void);


		_declspec(dllexport) BitStreamInfo * Bit_stream() const { return m_bit_stream; }
		_declspec(dllexport) void Bit_stream(BitStreamInfo * val) { m_bit_stream = val; }
		_declspec(dllexport) void AddAttributeID(int _att_id);
		_declspec(dllexport) bool ContainsAttID(int _att_id);
		_declspec(dllexport) void AddStreamNo(int _stream_no);
		_declspec(dllexport) bool ContainsStreamNo(int _stream_no);
		_declspec(dllexport) vector<int> Attribute_no() const { return m_attribute_no; }
		_declspec(dllexport) void Attribute_no(vector<int> val) { m_attribute_no = val; }
		_declspec(dllexport) vector<int> Bit_stream_no() const { return m_bit_stream_no; }
		_declspec(dllexport) void Bit_stream_no(vector<int> val) { m_bit_stream_no = val; }	
		_declspec(dllexport) int Count();
		_declspec(dllexport) void DeleteBitStream();
		_declspec(dllexport) int Hash();
		_declspec(dllexport) void Print();

		/*
		 * Merging bit streams to get the candidate sets
		 */
		_declspec(dllexport) BitStreamHolder * Merge(BitStreamHolder * _holder,map<int,int> & _index_attribute_map,vector<BitStreamHolder *> & _index_holder);

		_declspec(dllexport) vector<BitStreamHolder *> Difference(BitStreamHolder * _holder,map<int,int> & _index_attribute_map);

	private:
		vector<int> m_attribute_no;		
		vector<int> m_bit_stream_no;		
		BitStreamInfo * m_bit_stream;
		int m_count;
		bool Contains(int _value,vector<int> & _container);
		static const int MULTIPLICATIVE_UNIT = 10;
		
	};
#endif