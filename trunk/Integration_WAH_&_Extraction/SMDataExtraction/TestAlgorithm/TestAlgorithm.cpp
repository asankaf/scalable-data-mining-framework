// TestAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestAprioriOpt.h"
#include <vld.h>
#include <iostream>
using namespace std;

void TestAprioriAlgo();

int _tmain(int argc, _TCHAR* argv[])
{
	TestAprioriAlgo();
	return 0;
}

void TestAprioriAlgo()
{
	TestAprioriOpt test_opt;
	test_opt.TestSuite();
}