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
//Test for this method failed, need to test rigorously
	void AlgoUtils::GetUniqueBitmaps( EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map )
	{
		int attribute_index = _attribute->attributeID();
		int current_index = (_bitmaps.size() > 0)?(_bitmaps.size()):0;
		//map<int,dynamic_bitset<>> unique_val_map;
		vector<int> bit_streams_indices;
		vector<BitStreamInfo *> bit_streams = _attribute->vBitStreams();
		size_t bit_stream_size = _unique_patterns.size();
		dynamic_bitset<> temp ;
		for (size_t index = 0; index < bit_stream_size ; index++)
		{
			bit_streams_indices.push_back(current_index);
			temp = _unique_patterns.at(index);
			_pattern_index_map.push_back(temp.to_ulong());
			BitStreamHolder * holder = WrapWithHolder(FindPattern(temp,bit_streams),attribute_index,current_index);
			_bitmaps.push_back(holder);	
			//unique_val_map[current_index] = temp;
			current_index++;
		}
		_index_bitmap_map[attribute_index] = bit_streams_indices;
//return unique_val_map;
	}

	map<int,int> AlgoUtils::CreateIndexAttributeMap(map<int,vector<int>> & _index_att_map)
	{
		map<int,int> result_map;
		typedef map<int,vector<int>>::const_iterator map_vect_iter;
		typedef vector<int>::const_iterator vector_iter;
		vector<int> temp_vect;
		for (map_vect_iter start = _index_att_map.begin() ; start != _index_att_map.end(); start++)
		{
			temp_vect = start->second; 
			for (vector_iter iter_2 = temp_vect.begin();iter_2 != temp_vect.end(); iter_2++)
			{
				result_map[*(iter_2)] = start->first;
			}
			
		}
		return result_map;
	}

	void AlgoUtils::GetUniqueBitmaps(WrapDataSource * _source,vector<BitStreamHolder *> & _bitmaps,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map)
	{
		vector<EncodedAttributeInfo *> attr = _source->codedAttributes();
		typedef vector<EncodedAttributeInfo *>::const_iterator attr_iter_t;
		EncodedMultiCatAttribute * multi_cat = NULL;
		for (attr_iter_t iter = attr.begin(); iter != attr.end() ; iter++)
		{
			EncodedAttributeInfo * current_att = *(iter);
			multi_cat= static_cast<EncodedMultiCatAttribute *>(current_att);
			GetUniqueBitmaps(current_att,_bitmaps,multi_cat->mappedValList(),_index_bitmap_map,_pattern_index_map);
		}
	}
	
	void AlgoUtils::CopyFirstToSecond(vector<BitStreamHolder *> & left,vector<BitStreamHolder *> & right)
	{
		back_insert_iterator<vector<BitStreamHolder *>> back_iter(right);
		copy(left.begin(),left.end(),back_iter);
	}

	BitStreamHolder * AlgoUtils::WrapWithHolder(BitStreamInfo *_stream, int _attribute_id, int _bit_map_id)
	{
		BitStreamHolder * result = new BitStreamHolder();
		result->AddAttributeID(_attribute_id);
		result->AddStreamNo(_bit_map_id);
		result->Bit_stream(_stream);
		return result;

	}

// 	BitStreamHolder * AlgoUtils::MergeHolder(BitStreamHolder * _holder,vector<BitStreamHolder *> _index_holder_map)
// 	{
// 		BitStreamHolder * result_holder = new BitStreamHolder();
// 
// 	}

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