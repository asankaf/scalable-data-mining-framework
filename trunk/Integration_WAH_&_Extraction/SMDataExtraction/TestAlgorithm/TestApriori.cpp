#include "StdAfx.h"
#include "TestApriori.h"


TestApriori::TestApriori(void)
{
}

TestApriori::~TestApriori(void)
{
}

void TestApriori::TestUniqueBitMapGeneration(Apriori & algo)
{
	
	TestAlgoUtil test_algo_util;
	WrapDataSource * wrapped = test_algo_util.GetTestWrappedDataSource();
	AlgoUtils utils;
	algo.GetUniqueItems(wrapped,utils);
	cout << "Printing unique values : " << endl;
	test_algo_util.PrintBitStreamHolderVector(algo.Unique_bitmap_holders());
	cout << endl;
	cout << "Printing index attribute map :" << endl;
	PrintMap(algo.Index_attribute_map());
	cout << "Finished printing index att map"<<endl;
	cout << endl;
	cout << "Printing unique value map :" << endl;
	PrintVector(algo.Unique_values_index_map());
	cout << "Finished printing unique value map"<<endl;
	cout << endl;
	cout << "Printing count map :" << endl;
	PrintMap(algo.Bitmap_count_holder());
	cout << "Finished printing count map"<<endl;
	cout << endl;

	
}

void TestApriori::PrintRules(vector<AssociateRule *> & _rules)
{
	typedef vector<AssociateRule *>::const_iterator rule_iter;
	size_t index = 0;
	for (rule_iter start = _rules.begin(); start != _rules.end() ; start++)
	{
		cout << "Index : "<<++index << endl;
		PrintAssociateRule(*(start));
	}
}

void TestApriori::PrintAssociateRule(AssociateRule * rule)
{
	TestAlgoUtil test_util;
	cout << "Confidence : " << rule->Confidence() << endl;
	cout << "Support : " << rule->Support() << endl;
	cout << "Rule : " << rule->Rule() << endl;
	cout << endl;
}

void TestApriori::PrintMap(map<int,int> & _map)
{
	typedef map<int,int>::const_iterator map_iter;
	for (map_iter _iter = _map.begin();_iter != _map.end();_iter++)
	{
		cout << "Key : " << _iter->first << " Value : "<< _iter->second << endl;
	}
	cout << endl;
}

void TestApriori::PrintVector(vector<unsigned long int> & _vet)
{

	typedef vector<unsigned long int>::iterator vect_iter;
	for (vect_iter iter = _vet.begin(); iter != _vet.end(); iter++)
	{
		cout << *(iter) <<" , ";
	}
	cout << endl;
}

void TestApriori::PrintVector(vector<int> & _vet)
{

	typedef vector<int>::iterator vect_iter;
	for (vect_iter iter = _vet.begin(); iter != _vet.end(); iter++)
	{
		cout << *(iter) <<" , ";
	}
	cout << endl;
}

void TestApriori::TestInitialItemGeneration(Apriori & _algo, AlgoUtils & _utils)
{
	_algo.Support(2);
	_algo.CreateInitialCandidateSet(_utils);
	
	cout << "Printing candidate set : "<< endl;
	TestAlgoUtil test_algo_utils;
	test_algo_utils.PrintBitStreamHolderVector(_algo.Candidate_set());
	cout << "Printing candidate set finished: "<< endl;
	cout << endl;

	cout << "Start Printing hash value counts : "<< endl;
	PrintMap(_algo.Bitmap_count_holder());
	cout << "Finished Printing hash value counts : "<< endl;
	cout << endl;

	cout << "Start Printing frequent itemsets : "<< endl;
	test_algo_utils.PrintBitStreamHolderVector(_algo.Frequent_item_set());
	cout << "Finished Printing frequent itemsets  : "<< endl;
	cout << endl;

	cout << "Printing candidate set : "<< endl;
	test_algo_utils.PrintBitStreamHolderVector(_algo.Candidate_set());
	cout << "Printing candidate set finished: "<< endl;
	cout << endl;
}

void TestApriori::TestRuleGeneration(Apriori & _algo,AlgoUtils & _util)
{	
	TestAlgoUtil test_algo_utils;
	cout << "Printing Candidate Holder " << endl;
	test_algo_utils.PrintBitStreamHolderVector(_algo.Frequent_item_set());
	cout <<"Finished Printing Holder" << endl;	
	_algo.GenerateRules();
	cout << endl;
	cout << "Printing Rules : " << endl;
	PrintRules(_algo.Rules());
	
}

void TestApriori::TestTotalExecution(Apriori & _algo, AlgoUtils & _utils)
{
	TestAlgoUtil test_algo_utils;
	cout <<"Starting to run the Algo" << endl;
	WrapDataSource * wrapped = test_algo_utils.GetTestWrappedDataSource();
	_algo.RunAlgorithm(wrapped);
	cout << "Printing Candidate Holder " << endl;
	//test_algo_utils.PrintBitStreamHolder(_algo.Frequent_item_set().at(0));
	test_algo_utils.PrintBitStreamHolderVector(_algo.Frequent_item_set());
	cout <<"Finished Printing Holder" << endl;	
	//_algo.GenerateRulesForHolder(_algo.Frequent_item_set().at(0));
	_algo.GenerateRules();
	cout << endl;
	cout << "Printing Rules : " << endl;
	PrintRules(_algo.Rules());

}

void TestApriori::TestAntecedentGeneration()
{
	vector<int> original;
	original.push_back(1);
	original.push_back(11);
	original.push_back(12);
	original.push_back(14);
	original.push_back(15);
	original.push_back(18);
	cout << "Printing Original Vector : " << endl;
	PrintVector(original);
	cout << endl;
	Apriori algo;
	vector<int> second = algo.GenerateAntedecent(1,original);
	cout << "Printing Original Vector : " << endl;
	PrintVector(original);
	cout << endl;
	cout << "Printing Antecedent Vector : " << endl;
	PrintVector(second);
	cout << endl;


}

void TestApriori::TestCandidateGeneration(Apriori & _algo,AlgoUtils & _utils)
{
	TestAlgoUtil test_algo_utils;
	//_algo.GenerateCandidates(_utils);
	_algo.GenerateAllCandidateSets(_utils);
	cout << "Start Printing frequent itemsets : "<< endl;
	test_algo_utils.PrintBitStreamHolderVector(_algo.Frequent_item_set());
	cout << "Finished Printing frequent itemsets  : "<< endl;
}

void TestApriori::AprioriTestSuite()
{
	//TestAntecedentGeneration();
	//Tests the unique bitmap generation part
 	Apriori algo;
 	AlgoUtils utils;
	TestTotalExecution(algo,utils);
//  	TestUniqueBitMapGeneration(algo);
//  	TestInitialItemGeneration(algo,utils);
// 	
// 	cout << "----------------------------------" << endl;
// 	cout <<"Starting Item generation : " << endl;
//  	TestCandidateGeneration(algo,utils);
// 	TestRuleGeneration(algo,utils);
}
