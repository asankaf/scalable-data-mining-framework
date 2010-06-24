#pragma once
#include "BitStreamHolder.h"
#include <vector>
#include <iostream>
#include <map>
#include "WrapDataSource.h"
#include "algoutils.h"
#include "associaterule.h"

	/*
	 * Apriori Algorithm which generates the Association Rules 
	 */

	class Apriori
	{
	public:
		 typedef vector<int>::const_iterator vector_iter;		
		_declspec(dllexport) Apriori(void);
		_declspec(dllexport) ~Apriori(void);

		/* Get support */
		_declspec(dllexport) int Support() const { return m_support; }

		/* Combination */
		_declspec(dllexport) bool ContainCombination(int _combination);

		/* Set minimum support level */
		_declspec(dllexport) void Support(int val) { m_support = val; }

		/* Get, Set confidence */
		_declspec(dllexport) float Confidence() const { return m_confidence; }
		_declspec(dllexport) void Confidence(float val) { m_confidence = val; }

		/* Get, Set index vector */
		_declspec(dllexport) map<int,int> Index_vector_collection() const { return m_index_vector_collection; }
		_declspec(dllexport) void Index_vector_collection(map<int,int> val) { m_index_vector_collection = val; }

		/* Get, Set frequent item sets */
		_declspec(dllexport) vector<BitStreamHolder *> Frequent_item_set() const { return m_frequent_item_set; }
		_declspec(dllexport) void Frequent_item_set(vector<BitStreamHolder *> val) { m_frequent_item_set = val; }

		/* Get, Set unique bitmap holders */
		_declspec(dllexport) vector<BitStreamHolder *> Unique_bitmap_holders() const { return m_unique_bitmap_holders; }
		_declspec(dllexport) void Unique_bitmap_holders(vector<BitStreamHolder *> val) { m_unique_bitmap_holders = val; }

		/* Get, Set index attribute map */
		_declspec(dllexport) map<int,int> Index_attribute_map() const { return m_index_attribute_map; }
		_declspec(dllexport) void Index_attribute_map(map<int,int> val) { m_index_attribute_map = val; }

		/* Get, Set bitmap count holder */
		_declspec(dllexport) map<int,int> Bitmap_count_holder() const { return m_bitmap_count_holder; }
		_declspec(dllexport) void Bitmap_count_holder(map<int,int> val) { m_bitmap_count_holder = val; }

		/* Get, Set unique values index map */
		_declspec(dllexport) vector<unsigned long int> Unique_values_index_map() const { return m_unique_values_index_map; }
		_declspec(dllexport) void Unique_values_index_map(vector<unsigned long int> val) { m_unique_values_index_map = val; }

		/* Candidate set */
		_declspec(dllexport) vector<BitStreamHolder *> Candidate_set() const { return m_candidate_set; }
		_declspec(dllexport) void Candidate_set(vector<BitStreamHolder *> val) { m_candidate_set = val; }
		_declspec(dllexport) int GetCountForBitMap(int _hash_value);
		_declspec(dllexport) void CreateInitialCandidateSet(AlgoUtils & _utils);
		_declspec(dllexport) void GenerateCandidates(AlgoUtils & _utils);
		_declspec(dllexport) void GetUniqueItems(WrapDataSource * _wrapped,AlgoUtils & _utils);
		_declspec(dllexport) void GenerateAllCandidateSets(AlgoUtils & _utils);
		
		/* Antecedent */
		_declspec(dllexport) void CalculateCountsForUniqueBitmaps();
		_declspec(dllexport) vector<int> GenerateAntedecent(int _index,vector<int> & _frequent_indices);
		_declspec(dllexport) int CalculateHash(vector<int> & _vector);

		/* Generating association rules */
		_declspec(dllexport) vector<AssociateRule *> Rules() const { return m_rules; }
		_declspec(dllexport) void Rules(vector<AssociateRule *> val) { m_rules = val; }
		_declspec(dllexport) void GenerateRules();
		_declspec(dllexport) void RunAlgorithm(WrapDataSource * source);
		_declspec(dllexport) void GenerateRulesForHolder(BitStreamHolder * _holder);

		/* Rules */
		_declspec(dllexport) string GetStringForVector(vector<EncodedAttributeInfo *> & _atrributes,vector<int> & _index_vector);
		_declspec(dllexport) void CreateRuleStrings(WrapDataSource * source);
		_declspec(dllexport) void CreateStringForRule(AssociateRule * _rule , vector<EncodedAttributeInfo *> _atrributes);
		_declspec(dllexport) int No_of_rules() const { return m_no_of_rules; }
		_declspec(dllexport) void No_of_rules(int val) { m_no_of_rules = val; }
		_declspec(dllexport) int No_of_items() const { return m_no_of_items; }
		_declspec(dllexport) void No_of_items(int val) { m_no_of_items = val; }
		_declspec(dllexport) bool ContainsRule(int _hash);

	private:
		/* Association measures */
		int m_support;
		float m_confidence;
		int m_no_of_rules;		
		int m_no_of_items;		

		/* Generations of rules */
		vector<BitStreamHolder *> m_frequent_item_set;	
		vector<BitStreamHolder *> m_unique_bitmap_holders;
		vector<int> m_added_rules;
		map<int,int> m_index_vector_collection;	
		map<int,int> m_index_attribute_map;	
		map<int,int> m_bitmap_count_holder;	
		vector<unsigned long int> m_unique_values_index_map;		
		vector<BitStreamHolder *> m_candidate_set;	

		/* Association Rules */
		vector<AssociateRule *> m_rules;
		
	};
