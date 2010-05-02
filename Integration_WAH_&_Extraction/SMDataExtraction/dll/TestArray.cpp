#include "TestArray.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include "wrappedbitvector.h"

TestArray::TestArray(void)
{

}

TestArray::~TestArray(void)
{

}

void TestArray::TestSuite()
{
	cout << "Array Generation phase : " << endl;
	
	dynamic_bitset<> test(500,23ul);
	WrappedBitVector vect(test);
vect.decompress();
	
	cout << "Array generatio finished" << endl;
	
}

void TestArray::CleanArray()
{
ibis::fileManager::instance().~fileManager();
}