#ifndef _ALGOUTILS_H
#define _ALGOUTILS_H

#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <vector>
#include <map>
#include "EncodedAttributeInfo.h"
#include "EncodedMultiCatAttribute.h"
#include "WrapDataSource.h"
#include <iostream>
#include <math.h>
#include "aprioriitemset.h"
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

		/**Computes the sum of a numeric attribute*/
		_declspec(dllexport) static double USum(EncodedAttributeInfo * attribute);		

		/**Computes the sum square of a numeric attribute*/
		_declspec(dllexport) static double SumSquare(EncodedAttributeInfo * attribute);		

		/**Returns the bitmap of results greater than the  given value*/
		_declspec(dllexport) static BitStreamInfo * UGreaterThan(EncodedAttributeInfo * attribute, unsigned long value,int rows);		

		/**Returns the bitmap of results greater than or equal to the  given value*/
		_declspec(dllexport) static BitStreamInfo * UGreaterThanOrEq(EncodedAttributeInfo * attribute, unsigned long value,int rows);

		/**Returns the bitmap of results less than or equal to the  given value*/
		_declspec(dllexport) static BitStreamInfo * ULessThanOrEq(EncodedAttributeInfo * attribute, unsigned long value,int rows);

		/**Returns the bitmap of results less than the  given value*/
		_declspec(dllexport) static BitStreamInfo * ULessThan(EncodedAttributeInfo * attribute, unsigned long value,int rows);

		/**Returns the bitmap of results equal to the  given value*/
		_declspec(dllexport) static BitStreamInfo * UEq(EncodedAttributeInfo * attribute, unsigned long value);

		/**Generates a BitstreamInfo as same the type of attribute */
		_declspec(dllexport) static BitStreamInfo * BitStreamGenerator(EncodedAttributeInfo * attribute,dynamic_bitset<> & _bit_stream);		
		
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

	private :
		static double SumOfInt(EncodedAttributeInfo * attribute);

		/**Handles the sum square obtaining function for int values*/
		static double SumSquareOfInt(EncodedAttributeInfo * attribute);

		/**Finds value greater than the given value (the given value should be unsigned)*/
		static BitStreamInfo * UGreaterThanInt(EncodedAttributeInfo * attribute,unsigned long input_value,int noOfRows);
	};

#endif