#pragma once
#include <map>
#include <vector>
#include "AlgoUtils.h"
#include "testalgoutil.h"
#include "Apriori.h"

using namespace Algorithm;
class TestApriori
{
public:
	TestApriori(void);
	~TestApriori(void);
	void TestUniqueBitMapGeneration();
	void PrintMap(map<int,int> & _map);
	void PrintVector(vector<unsigned long int> & _vet);
	void PrintVector(vector<int> & _vet);
	void AprioriTestSuite();
	void TestUniqueBitMapGeneration(Apriori & algo);
	void TestInitialItemGeneration(Apriori & _algo, AlgoUtils & _utils);
	void TestCandidateGeneration(Apriori & _algo,AlgoUtils & _utils);
	void TestAntecedentGeneration();
};
