#pragma once
#include "testarray.h"
class TestMemLeaks
{
public:
	TestMemLeaks(void);
	~TestMemLeaks(void);
	void TestSuite();
	void ArrayTest();
};
