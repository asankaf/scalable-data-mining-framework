#pragma once
#include "BitStreamHolder.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <vector>
#include <map>
#include "EncodedAttributeInfo.h"


namespace Algorithm{
	class AlgoUtils
	{
	public:
		AlgoUtils(void);
		~AlgoUtils(void);
		BitStreamHolder * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamHolder *> & _container);
		BitStreamInfo * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container);
		dynamic_bitset<> FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container);
		BitStreamHolder * WrapWithHolder(BitStreamInfo * _stream,int _attribute_id,int _bit_map_id);
		map<int,dynamic_bitset<>> GetUniqueBitmaps(EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map);
	};
}