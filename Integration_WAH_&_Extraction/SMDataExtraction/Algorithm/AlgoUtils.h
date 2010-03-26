#pragma once
#include "BitStreamHolder.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <vector>
#include <map>
#include "EncodedAttributeInfo.h"
#include "EncodedMultiCatAttribute.h"
#include "WrapDataSource.h"
#include <iostream>
#include "..\..\..\..\..\mydocuments\visual studio 2008\projects\smdataextraction\algorithm\aprioriitemset.h"
#include "associaterule.h"

using namespace std;
namespace Algorithm{
	class AlgoUtils
	{
	public:
		_declspec(dllexport) AlgoUtils(void);
		_declspec(dllexport) ~AlgoUtils(void);
		_declspec(dllexport) vector<dynamic_bitset<>> GenerateUniqueValues(vector<string> & _string_map,int _no_of_bits);
		_declspec(dllexport) BitStreamHolder * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamHolder *> & _container);
		_declspec(dllexport) BitStreamInfo * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container);
		_declspec(dllexport) dynamic_bitset<> FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container);
		_declspec(dllexport) BitStreamHolder * WrapWithHolder(BitStreamInfo * _stream,int _attribute_id,int _bit_map_id);
		_declspec(dllexport) map<int,int> CreateIndexAttributeMap(map<int,vector<int>> & _index_att_map);
		_declspec(dllexport) void GetUniqueBitmaps(WrapDataSource * _source,vector<BitStreamHolder *> & _bitmaps,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		_declspec(dllexport) void GetUniqueBitmaps(EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		_declspec(dllexport) static void CopyFirstToSecond(vector<BitStreamHolder *> & left,vector<BitStreamHolder *> & right);
		_declspec(dllexport) void PrintAprioriItemSets(vector<AprioriItemset *> _items);
		_declspec(dllexport) void PrintAprioriItemSets(vector<AprioriItemset *> _items, WrapDataSource * ws);
		_declspec(dllexport) void PrintAprioriItemSets(vector<vector<AprioriItemset *>> & _items, WrapDataSource * ws);
		_declspec(dllexport) void PrintHashMapVector(vector<hash_map<int, int>> & _map_vector);
// 		_declspec(dllexport) template<class X> 
// 		static void CopyFirstToSecond(vector<X *> & left,vector<X *> & right){
// 			back_insert_iterator<vector<X *>> back_iter(right);
// 			copy(left.begin(),left.end(),back_iter);
// 		}	
		_declspec(dllexport) static void CopyFirstToSecond(vector<AssociateRule *> & left,vector<AssociateRule *> & right);
		_declspec(dllexport) void PrintHashMap(hash_map<int, int> & _hash_map);
		_declspec(dllexport) static void PrintRules(vector<AssociateRule *> & _rules);
		_declspec(dllexport) static void PrintRule(AssociateRule * _rule);
			//BitStreamHolder * MergeHolder(BitStreamHolder * _holder,vector<BitStreamHolder *> _index_holder_map);
	};
}