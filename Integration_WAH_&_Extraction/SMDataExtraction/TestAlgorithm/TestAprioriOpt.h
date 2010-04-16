#pragma once
#include "aprioriopt.h"
#include "algoutils.h"
#include <vector>
#include "aprioriitemset.h"
#include "DBConnection.h"
// #include "DBQueryExecution.h"
#include "WrapDataSource.h"

using namespace Algorithm;
using namespace DBConnectionInfo;
class TestAprioriOpt
{
public:
	TestAprioriOpt(void);
	~TestAprioriOpt(void);
	void TestSuite();
	void TestUniqueItemSetGeneration(WrapDataSource * _wrapped, AprioriOpt & _ap_opt);
	void TestHashMapBuilding(WrapDataSource * _wrapped, AprioriOpt & _ap_opt);
	void TestUniqueItemsetDeletion(AprioriOpt & _ap_opt, WrapDataSource * ws);
	void TestFindFrequentItemSets(AprioriOpt & _ap_opt,WrapDataSource * ws);
	void TestBuildAssociations(WrapDataSource * _wrapped, AprioriOpt & _ap_opt);
	void LoadAndPrintCSV();
	WrapDataSource * CreateWrappedDataSource();
};
