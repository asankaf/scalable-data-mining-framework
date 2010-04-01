#pragma once
#include "BitStreamInfo.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "EWAH.h"

class TestEWAH
{
public:
	TestEWAH(void);
	~TestEWAH(void);
	void TestSuite();
	void ANDTest();
};
