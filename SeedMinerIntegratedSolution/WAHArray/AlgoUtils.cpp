#include "StdAfx.h"
#include "AlgoUtils.h"
namespace Algorithm{
	AlgoUtils::AlgoUtils(void)
	{
	}

	AlgoUtils::~AlgoUtils(void)
	{
	}

	BitStreamInfo * AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container)
	{
		// Replace This with an exception
		size_t pattern_size = _pattern.size();
		assert(pattern_size > 0 && pattern_size == _container.size());
		BitStreamInfo * left_op = NULL;
		BitStreamInfo * right_op = NULL;	
		size_t index = 0;
		if (_pattern[index] == 0)
		{
			left_op = ~(*(_container.at(index)));
		}else{
			left_op = _container.at(index);
		}

		for (index = 1; index < pattern_size;index++)
		{
			if (_pattern[index] == 0)
			{
				right_op = ~(*(_container.at(index)));
			}else{
				right_op = _container.at(index);
			}

			left_op = (*(left_op) & *(right_op));
			right_op = NULL;
		}
		return left_op;
	}

	map<int,dynamic_bitset<>>  AlgoUtils::GetUniqueBitmaps(EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map)
	{
		int attribute_index = _attribute->attributeID();
		int current_index = (_bitmaps.size() > 0)?(_bitmaps.size()):0;
		map<int,dynamic_bitset<>> unique_val_map;
		vector<int> bit_streams_indices;
		vector<BitStreamInfo *> bit_streams = _attribute->vBitStreams();
		size_t bit_stream_size = _unique_patterns.size();
		dynamic_bitset<> temp ;
		for (size_t index = 0; index < bit_stream_size ; index++)
		{
			bit_streams_indices.push_back(current_index);
			temp = _unique_patterns.at(index);
			BitStreamHolder * holder = WrapWithHolder(FindPattern(temp,bit_streams),attribute_index,current_index);
			_bitmaps.push_back(holder);	
			unique_val_map[current_index] = temp;
			current_index++;
		}
		_index_bitmap_map[attribute_index] = bit_streams_indices;
return unique_val_map;
	}

	BitStreamHolder * AlgoUtils::WrapWithHolder(BitStreamInfo *_stream, int _attribute_id, int _bit_map_id)
	{
		BitStreamHolder * result = new BitStreamHolder();
		result->AddAttributeID(_attribute_id);
		result->AddStreamNo(_bit_map_id);
		result->Bit_stream(_stream);
		return result;

	}

	dynamic_bitset<> AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container)
	{
		size_t pattern_size = _pattern.size();
		assert(pattern_size > 0 && pattern_size == _container.size());
		dynamic_bitset<> left_op;
		dynamic_bitset<> right_op;
		size_t index = 0;
		left_op = _container.at(index);
		if (_pattern[index] == 0)
		{
			left_op.flip();
		}
		for (index =1; index < pattern_size ; index++)
		{
			right_op = _container[index];
			if (_pattern[index] == 0)
			{
				right_op.flip();
			}
			left_op = (left_op & right_op);
		}

		return left_op;
	}
}