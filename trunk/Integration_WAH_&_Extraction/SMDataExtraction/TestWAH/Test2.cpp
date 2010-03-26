#include "stdafx.h"
#include <iostream>
#include "WAHStructure.h"
#include <vector>

using namespace std;
using namespace boost;
int tmain(int argc, _TCHAR* argv[])
{

	WAHStructure * st1 = new WAHStructure();
	WAHStructure * st2 = new WAHStructure();

	dynamic_bitset<> x(93);
	for (int i = 0 ; i < x.size() ; i++)
	{
		if (i < 62)
		{
			x[i] = 1;
		}		
	}
	cout << "Original Stream st1 : " << x << endl;
	
	cout << "st1 Array : " << endl;
	

	//x.flip();
	for (int i = 0 ; i < x.size() ; i++)
	{
		if (i%2 == 0)
		{
			x[i] = 1;
		}		else{
			x[i] = 0;
		}
	}
	cout << "Original Stream st1 : " << x << endl;
	st2->buildArray(x);	

	st1->compressWords(x);
	st1->printArray();
	cout << "st1 Print Vector : " <<endl;
	st1->printVector();
	st2->compressWords();	
	cout << "Compressed Structure st1 : " << endl;	
	st1->printCompressedStream();	
	cout << "Compressed Structure st2 : " <<  endl;
	st2->printCompressedStream();
	//st1->alignBitStreams(0,)
	//st1->printVector();
	//WAHStructure * st3 = *st1 | *st2;
	//st3->printCompressedStream();
	/*
	int n =0;
	int v = -1073741824;
	int dd = n | v;
	cout << dd << endl;

	unsigned long int v = 2147483648;
	int v2 = -2147483648;
	int v3 = -2147483648;
	boost::dynamic_bitset<> v1(32,v);
	cout << v1 << endl;	
	*/	
	return 0;
}

