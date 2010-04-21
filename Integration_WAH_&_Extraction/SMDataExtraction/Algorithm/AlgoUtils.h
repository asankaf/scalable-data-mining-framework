#ifndef _ALGOUTILS_H
#define _ALGOUTILS_H

#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <vector>
#include <map>
#include "EncodedAttributeInfo.h"
#include "EncodedMultiCatAttribute.h"
#include "WrapDataSource.h"
#include <iostream>
#include "..\..\..\..\..\mydocuments\visual studio 2008\projects\smdataextraction\algorithm\aprioriitemset.h"
#include "associaterule.h"
#include "BitStreamHolder.h"

using namespace std;

	class AlgoUtils
	{
	public:
		_declspec(dllexport) AlgoUtils(void);
		
		_declspec(dllexport) ~AlgoUtils(void);
		
		/**Function for obtaining bit patterns for distinct values. Distinct values are obtained as bitsets*/
		_declspec(dllexport) static vector<dynamic_bitset<>> GenerateUniqueValues(vector<string> & _string_map,int _no_of_bits);
		
		/**Finds the pattern defined by the _pattern from the BitStreams provided as BitStreamHolders*/
		_declspec(dllexport) static BitStreamHolder * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamHolder *> & _container);
		
		/**Finds the pattern defined by the _pattern from the BitStreams provided as BitStreamInfo objects*/
		_declspec(dllexport) static BitStreamInfo * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container);
		
		/**Finds the pattern defined by the _pattern from the BitStreams provided as dynamic_bitset*/
		_declspec(dllexport) static dynamic_bitset<> FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container);
		
		/**Finds the corresponding bitmaps of distinct values and return as a vector*/
		_declspec(dllexport) static vector<BitStreamInfo *> FindDistinctValues(EncodedMultiCatAttribute * _attribute);
		
		/**Creates a BitStreamHolder with the given parameters*/
		_declspec(dllexport) static BitStreamHolder * WrapWithHolder(BitStreamInfo * _stream,int _attribute_id,int _bit_map_id);
		
		_declspec(dllexport) static map<int,int> CreateIndexAttributeMap(map<int,vector<int>> & _index_att_map);
		
		_declspec(dllexport) static void GetUniqueBitmaps(WrapDataSource * _source,vector<BitStreamHolder *> & _bitmaps,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		
		_declspec(dllexport) static void GetUniqueBitmaps(EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		
		_declspec(dllexport) static void CopyFirstToSecond(vector<BitStreamHolder *> & left,vector<BitStreamHolder *> & right);
		
		_declspec(dllexport) void PrintAprioriItemSets(vector<AprioriItemset *> _items);
		
		_declspec(dllexport) void PrintAprioriItemSets(vector<AprioriItemset *> _items, WrapDataSource * ws);
		
		_declspec(dllexport) void PrintAprioriItemSets(vector<vector<AprioriItemset *>> & _items, WrapDataSource * ws);

		_declspec(dllexport) void PrintHashMapVector(vector<hash_map<int, int>> & _map_vector);
	
		_declspec(dllexport) static void CopyFirstToSecond(vector<AssociateRule *> & left,vector<AssociateRule *> & right);

		_declspec(dllexport) void PrintHashMap(hash_map<int, int> & _hash_map);

		_declspec(dllexport) static void PrintRules(vector<AssociateRule *> & _rules);

		_declspec(dllexport) static void PrintRule(AssociateRule * _rule);
			//BitStreamHolder * MergeHolder(BitStreamHolder * _holder,vector<BitStreamHolder *> _index_holder_map);
	};

#endif