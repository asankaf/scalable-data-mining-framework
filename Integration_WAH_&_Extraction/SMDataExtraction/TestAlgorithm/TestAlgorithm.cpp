// TestAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestAprioriOpt.h"
//#include <vld.h>
#include <iostream>
#include "testc45nominal.h"
using namespace std;

void TestAprioriAlgo();
void TestC45Algo();

int _tmain(int argc, _TCHAR* argv[])
{
	TestAprioriAlgo();
	return 0;
}

void TestAprioriAlgo()
{
	TestAprioriOpt test_opt;
	test_opt.TestSuite();
	//test_opt.LoadAndPrintCSV();
}

void TestC45Algo()
{
	TestC45Nominal test_c45;
	test_c45.TestSuite();
}
