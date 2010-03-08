#pragma once
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <vector>
#include "CompressionHandler.h"
#include "DataSourceGenerator.h"
#include <vector>
#include "BitsetGenerator.h"
#include "BitStreamInfo.h"
#include "WrapDataSource.h"
using namespace CompressedStructure::TestStructure;
using namespace  std;
class TestCompressionHandler
{
public:
	TestCompressionHandler(void);
	~TestCompressionHandler(void);
	vector<dynamic_bitset<>> CreateTestBitsets();
	void PrintWrapDataSource(WrapDataSource * _wrapper);
	void TestCompression();
	void PrintAttributes(EncodedAttributeInfo * _attribute);

};
