// TestAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <vld.h>
/*
#define VLD_CONFIG_AGGREGATE_DUPLICATES
*/

#include "TestAprioriOpt.h"
#include <iostream>
#include "testc45nominal.h"
#include "utils.h"
#include "testcomputations.h"
#include "testnaivebayes.h"
#include <fstream>
#include "testclassifiertestsource.h"
using namespace std;

void TestAprioriAlgo();
void TestC45Algo();
void TestAlgoComp();
void TestNaiveBayesAlgo();
void MemleaksTest();
void ClassifierTestSourceTest();

int _tmain(int argc, _TCHAR* argv[])
{
	TestC45Algo();
	//TestAprioriAlgo();
	//TestAlgoComp();
	//TestNaiveBayesAlgo();
	//ClassifierTestSourceTest();
	
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

void TestAlgoComp()
{
	TestComputations comp;
	comp.TestSuite();
}

void TestNaiveBayesAlgo()
{
	TestNaiveBayes bayes;
	bayes.TestSuite();
	
}

void ClassifierTestSourceTest()
{
 TestClassifierTestSource test;
 test.TestSuite();
}

void MemleaksTest()
{
/*
TestMemLeaks leaks;
leaks.TestSuite();*/

}