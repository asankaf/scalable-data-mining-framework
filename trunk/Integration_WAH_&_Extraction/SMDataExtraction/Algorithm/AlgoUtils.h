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
#include "smalgorithmexceptions.h"

using namespace std;
/************************************************************************
*    Class  :AlgoUtils	  
*    Author :Amila De Silva
*    Subj   :
* This class is a utility class. Provides widely used common algorithms
* for main data mining algorithms. This class can be split to create 
* domain specific algorithms in future
*    Version: 1
************************************************************************/
	class AlgoUtils
	{
	public:
		_declspec(dllexport) AlgoUtils(void);
		
		_declspec(dllexport) ~AlgoUtils(void);
		
		/* *
		* Function for obtaining bit patterns for distinct values. Distinct values are obtained as bitsets
		* */
		_declspec(dllexport) static vector<dynamic_bitset<>> GenerateUniqueValues(vector<string> & _string_map,int _no_of_bits);
		
		/**Finds the pattern defined by the _pattern from the BitStreams provided as BitStreamHolders*/
		_declspec(dllexport) static BitStreamHolder * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamHolder *> & _container) throw (algorithm_exception);
		
		/**Finds the pattern defined by the _pattern from the BitStreams provided as BitStreamInfo objects*/
		_declspec(dllexport) static BitStreamInfo * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container) throw (algorithm_exception);
		
		/**Finds the pattern defined by the _pattern from the BitStreams provided as dynamic_bitset*/
		_declspec(dllexport) static dynamic_bitset<> FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container) throw (algorithm_exception);
		
		/**Finds the corresponding bitmaps of distinct values and return as a vector*/
		_declspec(dllexport) static vector<BitStreamInfo *> FindDistinctValues(EncodedMultiCatAttribute * _attribute) throw(null_parameter_exception);
		
		/**Performs the AND operation and return the count*/
		_declspec(dllexport) static double ANDCount(BitStreamInfo * left_op, BitStreamInfo * right_op) throw (algorithm_exception);

		/**Creates a BitStreamHolder with the given parameters*/
		_declspec(dllexport) static BitStreamHolder * WrapWithHolder(BitStreamInfo * _stream,int _attribute_id,int _bit_map_id);


		/**Computes the sum of a numeric _attribute*/
		_declspec(dllexport) static double USum(EncodedAttributeInfo * _attribute)throw (null_parameter_exception,incompatible_operand_exception);

		/**Computes the sum of a numeric _attribute sampled by the _existence bitmap*/
		_declspec(dllexport) static double USum(EncodedAttributeInfo * _attribute, BitStreamInfo * _existence)throw (null_parameter_exception,incompatible_operand_exception);

		/**Computes the sum square of a numeric _attribute*/
		_declspec(dllexport) static double SumSquare(EncodedAttributeInfo * _attribute) throw (null_parameter_exception,incompatible_operand_exception);		

		/**Computes the sum square of a numeric _attribute sampled by the _existence bitmap*/
		_declspec(dllexport) static double SumSquare(EncodedAttributeInfo * _attribute,BitStreamInfo * _existence) throw (null_parameter_exception,incompatible_operand_exception);

		/**Returns the bitmap of results greater than the  given value*/
		_declspec(dllexport) static BitStreamInfo * UGreaterThan(EncodedAttributeInfo * _attribute, double value,int _rows) throw(algorithm_exception);		

		/**Returns the bitmap of results greater than or equal to the  given value*/
		_declspec(dllexport) static BitStreamInfo * UGreaterThanOrEq(EncodedAttributeInfo * _attribute, unsigned long value,int _rows);

		/**Returns the bitmap of results less than or equal to the  given value*/
		_declspec(dllexport) static BitStreamInfo * ULessThanOrEq(EncodedAttributeInfo * _attribute, unsigned long value,int _rows);

		/**Returns the bitmap of results less than the  given value*/
		_declspec(dllexport) static BitStreamInfo * ULessThan(EncodedAttributeInfo * _attribute, double value,int _rows);

		/**Returns the bitmap of results equal to the  given value*/
		_declspec(dllexport) static BitStreamInfo * UEq(EncodedAttributeInfo * _attribute, double value, int _rows);

		/**Generates a BitstreamInfo as same the type of attribute */
		_declspec(dllexport) static BitStreamInfo * BitStreamGenerator(EncodedAttributeInfo * _attribute,dynamic_bitset<> & _bit_stream) throw (algorithm_exception);		

		/** Function for calculating the variance of an attribute*/
		_declspec(dllexport) static double Variance(EncodedAttributeInfo * _attribute,BitStreamInfo * _existence) throw (division_by_zero_exception);
		
		_declspec(dllexport) static map<int,int> CreateIndexAttributeMap(map<int,vector<int>> & _index_att_map);
		
		_declspec(dllexport) static void GetUniqueBitmaps(WrapDataSource * _source,vector<BitStreamHolder *> & _bitmaps,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		
		_declspec(dllexport) static void GetUniqueBitmaps(EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map);
		
		_declspec(dllexport) static void CopyFirstToSecond(vector<BitStreamHolder *> & _left,vector<BitStreamHolder *> & _right);
		
		_declspec(dllexport) void PrintAprioriItemSets(vector<AprioriItemset *> _items);
		
		_declspec(dllexport) void PrintAprioriItemSets(vector<AprioriItemset *> _items, WrapDataSource * ws);
		
		_declspec(dllexport) void PrintAprioriItemSets(vector<vector<AprioriItemset *>> & _items, WrapDataSource * ws);

		_declspec(dllexport) void PrintHashMapVector(vector<hash_map<int, int>> & _map_vector);
	
		_declspec(dllexport) static void CopyFirstToSecond(vector<AssociateRule *> & _left,vector<AssociateRule *> & _right);

		_declspec(dllexport) void PrintHashMap(hash_map<int, int> & _hash_map);

		_declspec(dllexport) static void PrintRules(vector<AssociateRule *> & _rules);

		_declspec(dllexport) static void PrintRule(AssociateRule * _rule);
		

	private :

		static double SumOfInt(EncodedAttributeInfo * _attribute, BitStreamInfo * _existence);

		static double SumOfInt(EncodedAttributeInfo * _attribute);

		/**Handles the sum square obtaining function for int values*/
		static double SumSquareOfInt(EncodedAttributeInfo * _attribute);

		/** Handles the sum square obtaining function for int values. The sample of which the sum is to be taken is
		given by the bitstream _existence
		*/
		static double SumSquareOfInt(EncodedAttributeInfo * _attribute,BitStreamInfo * _existence);

		/**Finds value greater than the given value (the given value should be unsigned)*/
		static BitStreamInfo * UGreaterThanInt(EncodedAttributeInfo * _attribute,unsigned long _input_value,int _no_of_rows);

		/**Finds value greater than the given value (the given value should be unsigned)*/
		static BitStreamInfo * UGreaterThanInt(EncodedAttributeInfo * _attribute,unsigned long long _input_value,int _no_of_rows,unsigned long long _max_value);

		/**Finds value less than the given value (the given value should be unsigned)
		@param attribute Numeric Attribute
		@param input_value upper limit of the range
		@param noOfRows No of _rows in the attribute
		*/
		static BitStreamInfo * ULessThanInt(EncodedAttributeInfo * _attribute,unsigned long long _input_value,int _no_of_rows,unsigned long long _max_value) throw(null_parameter_exception);
	};

#endif