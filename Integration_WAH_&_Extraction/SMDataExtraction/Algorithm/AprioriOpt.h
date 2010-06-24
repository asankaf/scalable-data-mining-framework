#pragma once
#include "EncodedAttributeInfo.h"
#include <vector>
#include "aprioriitemset.h"
#include "WrapDataSource.h"
#include "algoutils.h"
#include "BitStreamInfo.h"
#include "associaterule.h"
#include <string>
#include <algorithm>
#include "smalgorithmexceptions.h"

/************************************************************************
*    Class  :AprioriOpt	  
*    Author :Amila De Silva
*    Subj   :
* Class implementing an Apriori-type algorithm. 
* Iteratively reduces the minimum support until it finds the 
* required number of rules with the given minimum confidence
* The algorithm has an option to mine class association rules.
*
*    Version: 1
************************************************************************/
class AprioriOpt
{


public:

	/***
	* Constructor
	*/
	_declspec(dllexport) AprioriOpt(void);

	/***
	* Destructor
	*/
	_declspec(dllexport) ~AprioriOpt(void);

	/***
	* Iterator for iterating through attributes
	*/
	typedef vector<EncodedAttributeInfo *>::iterator attribute_iterator;

	/***
	* Returns the set of distinct values for all the attributes in the data source.
	*/
	_declspec(dllexport) vector<AprioriItemset *> UniqueItems() const { return m_uniqueItems; }


	/***
	* Sets the unique items for the data source.
	*/
	_declspec(dllexport) void UniqueItems(vector<AprioriItemset *> val) { m_uniqueItems = val; }

	/***
	* Finds the distinct value for each attribute in the data source and store them in a vector.
	*/
	_declspec(dllexport) void FindUniqueItemSets(WrapDataSource * _instances) throw (algorithm_exception);

	/***
	* Top level function to find itemsets larger than a given minimum support and a confidence.
	*/
	_declspec(dllexport) void FindLargeItemSets();	

	/***
	* Delete itemsets that are lower than the _nec_support value. 
	* If the itemset is the initial itemset it is not deleted.
	*/
	_declspec(dllexport) vector<AprioriItemset *> DeleteItemSets(vector<AprioriItemset *> & _itemsets,int _nec_support, int _max_support,bool _not_initial_set);

	/***
	* Procedure to merge all k-1-itemsets to create k itemsets
	*/
	_declspec(dllexport) vector<AprioriItemset *> MergeAllItemSets(vector<AprioriItemset *> & _itemSets, int size);

	/***
	* Itemsets whose subsets are not large, are deleted by this method.
	* _ksets indicates the collection of itemsets to be tested
	* _kMinusOne contains the support values of k-1 itemsets
	*/
	_declspec(dllexport) vector<AprioriItemset *> PruneItemSets(vector<AprioriItemset *> & _ksets,hash_map<int,int> & _kMinusOne);

	/***
	* Create the Associate rules using the large itemsets.
	* _instances provide the header information required for decoding data.
	*/
	_declspec(dllexport) void BuildAssociations(WrapDataSource * _instances);

	/***
	* Implementation of the comparison function to compare two associate rules
	*/
	_declspec(dllexport) int Compare(const void * _arg1, const void * _arg2);

	/***
	* Method used for sorting the rules according to theri support values.
	*/
	_declspec(dllexport) void SortRules();

	/***
	* Updates the support values for newly created k-itemsets, by merging.
	*/
	_declspec(dllexport) void UpdateCounters(vector<AprioriItemset *> & _ksets,int _kminusize);
	
	/***
	* Updates the support values for newly created k-itemsets, by merging.
	*/
	_declspec(dllexport) static void UpDateCounters(vector<AprioriItemset *> & _ksets, vector<AprioriItemset *> & _kMinusSets);

	/***
	* Gives the string representation of all the rules.
	*/
	_declspec(dllexport) string ToString();

	/***
	* Prints the current collection of large itemsets.
	*/
	_declspec(dllexport) void PrintItemsets();

	/***
	* Returns the large itemsets.
	*/
	_declspec(dllexport) vector<vector<AprioriItemset *>> LargeItemSets() const { return m_largeItemSets; }

	/***
	* Sets the large itemsets.
	*/
	_declspec(dllexport) void LargeItemSets(vector<vector<AprioriItemset *>> val) { m_largeItemSets = val; }

	/***
	* Returns the number of cycles performed in the algorithm.
	*/
	_declspec(dllexport) int Cycles() const { return m_cycles; }

	/***
	* Sets the number of cycles performed.
	*/
	_declspec(dllexport) void Cycles(int val) { m_cycles = val; }

	/***
	* Returns the minimum support. 
	*/
	_declspec(dllexport) double MinSupport() const { return m_minSupport; }

	/***
	* Sets the minimum support.
	*/
	_declspec(dllexport) void MinSupport(double val) { m_minSupport = val; }

	/***
	* Generate Associate Rules from the itemset. 
	*/
	_declspec(dllexport) vector<AssociateRule *> GenerateRules( int numItemsInSet, AprioriItemset * _itemset );

	/***
	* Returns the confidence. 
	*/
	_declspec(dllexport) double Confidence() const { return m_confidence; }

	/***
	* Sets the confidence.
	*/
	_declspec(dllexport) void Confidence(double val) { m_confidence = val; }

	/***
	* Delete rules which are lower than a particular confidence level 
	*/
	_declspec(dllexport) vector<AssociateRule *> PruneRules( vector<AssociateRule *> & _rules);

	/***
	* Releases all the member variables used for computation. 
	*/
	_declspec(dllexport) void Clear();

	/***
	* Method used to find simple associate rules.
	* These rules will only contain one side of the the relations. 
	*/
	_declspec(dllexport) void FindRulesQuickly(vector<AssociateRule *> & _rules);

	/***
	* Returns the created set of associate rules.
	*/
	_declspec(dllexport) vector<AssociateRule *> Rules() const { return m_rules; }

	/***
	* Sets the set of Association Rules.
	*/
	_declspec(dllexport) void Rules(vector<AssociateRule *> val) { m_rules = val; }

	/***
	* Generate the string representation of the association rule. 
	*/
	_declspec(dllexport) void BuildStrings();

	/***
	* Returns the number of rules.
	*/
	_declspec(dllexport) int NumRules() const { return m_numRules; }

	/***
	* Sets the number of rules.
	*/
	_declspec(dllexport) void NumRules(int val) { m_numRules = val; }

	/***
	* Gets the string representation of an Itemset.
	*/
	_declspec(dllexport) string GetItemSetString(AprioriItemset * _itemset);

	/***
	* Clears the bitstreams for k-2itemsets.
	* Initial itemsets are preserved.
	*/
	_declspec(dllexport) void ClearPreviousBitStreams(vector<AprioriItemset *> & _prev_set);

private:
	/***
	* Deletes large itemset vector (m_largeItemSets)
	*/
	void ClearlargeItemSets();

	/***
	* Deletes unique itemset vector (m_uniqueItems)
	*/
	void ClearUniqueItems();

	/***
	* Deletes hash table vector (m_hashTables)
	*/
	void ClearHashTable();

	/***
	* Deletes the set of rules created (m_rules)
	*/
	void ClearRules();

	/***
	* Calls all the clear functions.
	*/
	void ClearAll();

	/* Vector to store distinct itemsets*/
	vector<AprioriItemset *> m_uniqueItems;

	/** Hashtable to store count of each itemset*/
	vector<hash_map<int,int>> m_hashTables;

	/** Hashtable to store count of each itemset*/
	vector<hash_map<int,AprioriItemset *>> m_hashItemSets;

	/** Structure to hold each large k-itemsets */
	vector<vector<AprioriItemset *>> m_largeItemSets;

	/** Vector holding Association Rules*/
	vector<AssociateRule *> m_rules;	

	/** Stores the maximum value the support can have*/
	double m_upperBoundMinSupport;

	/** Unit to increment support at each iteration*/
	double m_delta;

	/** Stores the minimum value the support can have*/
	double m_lowerBoundMinSupport;

	/** Number of rules to be derived*/ 
	int m_numRules;	

	/** Counts the number of iterations performed*/ 
	int m_cycles;

	/** Minimum support for rules*/ 
	double m_minSupport;

	/** Confidence for rules.*/ 
	double m_confidence;	

	/** Number of attributes in the data source*/ 
	int m_numberOfAttributes;		

	/** Data source to be used*/ 
	WrapDataSource * m_instances;		
};
