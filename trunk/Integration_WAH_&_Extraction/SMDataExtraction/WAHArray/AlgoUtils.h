#pragma once
#include "BitStreamHolder.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <vector>
#include <map>
#include "EncodedAttributeInfo.h"
#include "EncodedMultiCatAttribute.h"
#include "WrapDataSource.h"
#include <iostream>

using namespace std;
namespace Algorithm{
	class AlgoUtils
	{
	public:
		AlgoUtils(void);
		~AlgoUtils(void);
		vector<dynamic_bitset<>> GenerateUniqueValues(vector<string> & _string_map,int _no_of_bits);
		BitStreamHolder * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamHolder *> & _container);
		BitStreamInfo * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container);
		dynamic_bitset<> FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container);
		BitStreamHolder * WrapWithHolder(BitStreamInfo * _stream,int _attribute_id,int _bit_map_id);
		map<int,int> CreateIndexAttributeMap(map<int,vector<int>> & _index_att_map);
		void GetUniqueBitmaps(WrapDataSource * _source,vector<BitStreamHolder *> & _bitmaps,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		void GetUniqueBitmaps(EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		void CopyFirstToSecond(vector<BitStreamHolder *> & left,vector<BitStreamHolder *> & right);
			//BitStreamHolder * MergeHolder(BitStreamHolder * _holder,vector<BitStreamHolder *> _index_holder_map);
	};
}