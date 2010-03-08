#pragma once
#include "algoutils.h"
#include "BitsetGenerator.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include "DataSourceGenerator.h"
#include <iostream>

using namespace std;
using namespace Algorithm ;
using namespace CompressedStructure::TestStructure;
class TestAlgoUtil
{
public:
	TestAlgoUtil(void);
	~TestAlgoUtil(void);
	void TestPatternFindBitsetMethods();
	void TestPatternFindBitInfoMethod();
	void PrintBitSets(vector<dynamic_bitset<>> & _bit_sets);
	void PrintBitSets(vector<BitStreamInfo *> & _bit_sets);
	vector<dynamic_bitset<>> TestAlgoUtil::GetBitSetsForAttribute();
	void TestAttLevelUniquePatternFinding();
	void PrintBitStreamHolderVector(vector<BitStreamHolder *> _holder);
	void PrintBitStreamHolder(BitStreamHolder * _holder);
	void PrintBitStreamInfo(BitStreamInfo * _bit_stream);
	void PrintAttIdIndexMapping(map<int ,vector<int>> & _index_att_value_map);
	
};
