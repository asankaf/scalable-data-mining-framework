#pragma once
#include "BitStreamHolder.h"
#include "testalgoutil.h"
#include <vector>
using namespace Algorithm;
class TestHolderOperations
{
public:
	TestHolderOperations(void);
	~TestHolderOperations(void);
	BitStreamHolder * CreateHolder(vector<int> & _attribute_array,vector<int> & _index_array);
	void TestHash();
	void TestDifferenceOperator();
	void TestHolderSuite();
	void PrintBitInfoVector(vector<BitStreamHolder * > & _holder);
};
