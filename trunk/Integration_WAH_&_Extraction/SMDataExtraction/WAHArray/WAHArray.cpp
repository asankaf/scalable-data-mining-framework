// WAHArray.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "WAHStructure.h"
#include "BitsetGenerator.h"
#include "TestWAHArray.h"
#include <vector>
#include <algorithm>
#include <string>
#include "TimeTest.h"
//#include "DBQueryExecution.h"
#include "VBitStream.h"
#include "BitStreamInfo.h"
#include "bitstreaminfortest.h"
//#include "DBConnection.h"
#include "testcompressionhandler.h"

using namespace std;
using namespace boost;
using namespace  boost::detail;
using namespace CompressedStructure::TestStructure;
using namespace CompressedStructure;
//using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;
void randomBitStreamTester();
void WAHStructureBecnhmarkAND();
void WAHStructureBecnhmarkOR();

int _tmain(int argc, _TCHAR* argv[])
{

		
	return 0;
}


void WAHStructureBecnhmarkAND()
{
	TimeTest test;
	cout << "Initial Start Value : "<<endl;
	size_t init = 0;
	size_t end = 0;
	size_t increment = 0;
	cin >>init;
	cout << "Initial End Value : "<<endl;
	cin >> end;
	cout << "Increment Value : "<<endl;
	cin >> increment;
	test.RunAndTest(init,end,increment);
	cout <<"Executed Sucessfully" << endl;
}

void WAHStructureBecnhmarkOR()
{
	TimeTest test;
	cout << "Initial Start Value(OR) : "<<endl;
	size_t init = 0;
	size_t end = 0;
	size_t increment = 0;
	cin >>init;
	cout << "Initial End Value : "<<endl;
	cin >> end;
	cout << "Increment Value : "<<endl;
	cin >> increment;
	test.RunORTest(init,end,increment);
	cout <<"Executed Sucessfully" << endl;
}


void randomBitStreamTester()
{
	TestWAHArray test;
	int i = 0;
	while ( i < 10)
	{
		for (int j = 0; j < 110 ; j+=10)
		{
			cout << "Lendgth : 100 , Percent : "<<j << endl; 
			test.testRandomBitStream(100,1000,j);
			cout << ""<< endl;
		}

		i++;
	}	
}