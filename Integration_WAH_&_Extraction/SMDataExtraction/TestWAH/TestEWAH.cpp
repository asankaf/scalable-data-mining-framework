#include "TestEWAH.h"

TestEWAH::TestEWAH(void)
{
}

TestEWAH::~TestEWAH(void)
{
}

void TestEWAH::TestSuite()
{
	ANDTest();
}

void TestEWAH::ANDTest()
{
	dynamic_bitset<> left(1000,176159445ul);
	dynamic_bitset<> right(1000,0ul);

	EWAH left_op;
	left_op.CompressWords(left);

	EWAH right_op;
	right_op.CompressWords(right);

	BitStreamInfo * result = left_op & right_op;
	cout << "Left Op : " << left<< endl;
	cout << "Right Op : " << right<< endl;
	result->Print();
dynamic_bitset<> result2 = result->Decompress();
cout << "Result : " << result2 << endl;
}
