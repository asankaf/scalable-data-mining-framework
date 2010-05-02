#include "TestMemLeaks.h"


TestMemLeaks::TestMemLeaks(void)
{
}

TestMemLeaks::~TestMemLeaks(void)
{
}

void TestMemLeaks::TestSuite()
{
	ArrayTest();
}

void TestMemLeaks::ArrayTest()
{
	TestArray tt;
	tt.TestSuite();
	size_t j = 0; 
	for (size_t i = 0 ; i < 10000 ; i++)
	{
		j += 1;
	}
	//tt.CleanArray();
}

