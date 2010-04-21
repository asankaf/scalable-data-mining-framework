#pragma once
#include <vector>
#include "AssociateRule.h"
#include "WrapDataSource.h"
#include "DBConnection.h"
#include "DBQueryExecution.h"

using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;
//using namespace Algorithm;
class TestSeedMiner
{
public:
	TestSeedMiner(void);
	~TestSeedMiner(void);
	void TestSM();
	vector<AssociateRule *> TestAlgorithm(WrapDataSource * _wrapped,float _support,float _confidence);
};
