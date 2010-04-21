#pragma once
#include "EncodedAttributeInfo.h"
#include <vector>
#include "aprioriitemset.h"
#include "WrapDataSource.h"
#include "algoutils.h"
#include "BitStreamInfo.h"
#include "associaterule.h"
//#include "boost/lexical_cast.hpp"
#include <string>
#include <algorithm>
//using namespace Algorithm;
class AprioriOpt
{


public:

	_declspec(dllexport) AprioriOpt(void);
	_declspec(dllexport) ~AprioriOpt(void);
	typedef vector<EncodedAttributeInfo *>::iterator attribute_iterator;
	_declspec(dllexport) vector<AprioriItemset *> UniqueItems() const { return m_uniqueItems; }
	_declspec(dllexport) void UniqueItems(vector<AprioriItemset *> val) { m_uniqueItems = val; }
	_declspec(dllexport) void FindUniqueItemSets(WrapDataSource * _instances);
	_declspec(dllexport) void FindLargeItemSets();	
	_declspec(dllexport) vector<AprioriItemset *> DeleteItemSets(vector<AprioriItemset *> & _itemsets,int _nec_support, int _max_support,bool _not_initial_set);
	_declspec(dllexport) vector<AprioriItemset *> MergeAllItemSets(vector<AprioriItemset *> & _itemSets, int size);
	_declspec(dllexport) vector<AprioriItemset *> PruneItemSets(vector<AprioriItemset *> & _ksets,hash_map<int,int> & _kMinusOne);
	_declspec(dllexport) void BuildAssociations(WrapDataSource * _instances);
	_declspec(dllexport) int Compare(const void * arg1, const void * arg2);
	_declspec(dllexport) void SortRules();
	_declspec(dllexport) void UpdateCounters(vector<AprioriItemset *> & _ksets,int _kminusize);
	_declspec(dllexport) static void UpDateCounters(vector<AprioriItemset *> & _ksets, vector<AprioriItemset *> & _kMinusSets);
// 	_declspec(dllexport) double MinimumSupport() const { return m_minimumSupport; }
// 	_declspec(dllexport) void MinimumSupport(double val) { m_minimumSupport = val; }
	_declspec(dllexport) vector<vector<AprioriItemset *>> LargeItemSets() const { return m_largeItemSets; }
	_declspec(dllexport) void LargeItemSets(vector<vector<AprioriItemset *>> val) { m_largeItemSets = val; }
	_declspec(dllexport) int Cycles() const { return m_cycles; }
	_declspec(dllexport) void Cycles(int val) { m_cycles = val; }
	_declspec(dllexport) double MinSupport() const { return m_minSupport; }
	_declspec(dllexport) void MinSupport(double val) { m_minSupport = val; }
	//	vector<AssociateRule *> GenerateRules(int numItemsInSet,vector<double> & _confidences, AprioriItemset * _itemset);
	_declspec(dllexport) vector<AssociateRule *> GenerateRules( int numItemsInSet, AprioriItemset * _itemset );
	//void PruneRules(vector<AssociateRule *> & _rules);
	_declspec(dllexport) double Confidence() const { return m_confidence; }
	_declspec(dllexport) void Confidence(double val) { m_confidence = val; }
	//vector<AssociateRule *> PruneRules( vector<AssociateRule *> & _rules , vector<double> & _confidence);
	_declspec(dllexport) vector<AssociateRule *> PruneRules( vector<AssociateRule *> & _rules);
	_declspec(dllexport) void Clear();
	_declspec(dllexport) void FindRulesQuickly(vector<AssociateRule *> & _rules);
	_declspec(dllexport) vector<AssociateRule *> Rules() const { return m_rules; }
	_declspec(dllexport) void Rules(vector<AssociateRule *> val) { m_rules = val; }
	_declspec(dllexport) void BuildStrings();
	_declspec(dllexport) int NumRules() const { return m_numRules; }
	_declspec(dllexport) void NumRules(int val) { m_numRules = val; }

private:
	void ClearlargeItemSets();
	void ClearUniqueItems();
	void ClearHashTable();
	void ClearRules();
	void ClearAll();
	vector<AprioriItemset *> m_uniqueItems;
	//	hash_map<AprioriItemset *,int,hash_compare<AprioriItemset *,AprioriItemset::itemset_comp>> m_hashTable;
	vector<hash_map<int,int>> m_hashTables;
	vector<hash_map<int,AprioriItemset *>> m_hashItemSets;
	vector<vector<AprioriItemset *>> m_largeItemSets;
	vector<AssociateRule *> m_rules;	
	//double m_minimumSupport;
	double m_upperBoundMinSupport;
	double m_delta;
	double m_lowerBoundMinSupport;
	// Number of rules to be derived
	int m_numRules;	
	int m_cycles;
	double m_minSupport;
	double m_confidence;		
	int m_numberOfAttributes;		
	WrapDataSource * m_instances;		
};
