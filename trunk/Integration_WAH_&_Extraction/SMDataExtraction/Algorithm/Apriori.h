#pragma once
#include "BitStreamHolder.h"
#include <vector>
#include <iostream>
#include <map>
#include "WrapDataSource.h"
#include "algoutils.h"
#include "associaterule.h"

	class Apriori
	{
	public:
		 typedef vector<int>::const_iterator vector_iter;		
		_declspec(dllexport) Apriori(void);
		_declspec(dllexport) ~Apriori(void);
		_declspec(dllexport) int Support() const { return m_support; }
		_declspec(dllexport) bool ContainCombination(int _combination);
		_declspec(dllexport) void Support(int val) { m_support = val; }
		_declspec(dllexport) float Confidence() const { return m_confidence; }
		_declspec(dllexport) void Confidence(float val) { m_confidence = val; }
		_declspec(dllexport) map<int,int> Index_vector_collection() const { return m_index_vector_collection; }
		_declspec(dllexport) void Index_vector_collection(map<int,int> val) { m_index_vector_collection = val; }
		_declspec(dllexport) vector<BitStreamHolder *> Frequent_item_set() const { return m_frequent_item_set; }
		_declspec(dllexport) void Frequent_item_set(vector<BitStreamHolder *> val) { m_frequent_item_set = val; }
		_declspec(dllexport) vector<BitStreamHolder *> Unique_bitmap_holders() const { return m_unique_bitmap_holders; }
		_declspec(dllexport) void Unique_bitmap_holders(vector<BitStreamHolder *> val) { m_unique_bitmap_holders = val; }
		_declspec(dllexport) map<int,int> Index_attribute_map() const { return m_index_attribute_map; }
		_declspec(dllexport) void Index_attribute_map(map<int,int> val) { m_index_attribute_map = val; }
		_declspec(dllexport) map<int,int> Bitmap_count_holder() const { return m_bitmap_count_holder; }
		_declspec(dllexport) void Bitmap_count_holder(map<int,int> val) { m_bitmap_count_holder = val; }
		_declspec(dllexport) vector<unsigned long int> Unique_values_index_map() const { return m_unique_values_index_map; }
		_declspec(dllexport) void Unique_values_index_map(vector<unsigned long int> val) { m_unique_values_index_map = val; }
		_declspec(dllexport) vector<BitStreamHolder *> Candidate_set() const { return m_candidate_set; }
		_declspec(dllexport) void Candidate_set(vector<BitStreamHolder *> val) { m_candidate_set = val; }
		_declspec(dllexport) int GetCountForBitMap(int _hash_value);
		_declspec(dllexport) void CreateInitialCandidateSet(AlgoUtils & _utils);
		_declspec(dllexport) void GenerateCandidates(AlgoUtils & _utils);
		_declspec(dllexport) void GetUniqueItems(WrapDataSource * _wrapped,AlgoUtils & _utils);
		_declspec(dllexport) void GenerateAllCandidateSets(AlgoUtils & _utils);
		_declspec(dllexport) void CalculateCountsForUniqueBitmaps();
		_declspec(dllexport) vector<int> GenerateAntedecent(int _index,vector<int> & _frequent_indices);
		_declspec(dllexport) int CalculateHash(vector<int> & _vector);
		_declspec(dllexport) vector<AssociateRule *> Rules() const { return m_rules; }
		_declspec(dllexport) void Rules(vector<AssociateRule *> val) { m_rules = val; }
		_declspec(dllexport) void GenerateRules();
		_declspec(dllexport) void RunAlgorithm(WrapDataSource * source);
		_declspec(dllexport) void GenerateRulesForHolder(BitStreamHolder * _holder);
		_declspec(dllexport) string GetStringForVector(vector<EncodedAttributeInfo *> & _atrributes,vector<int> & _index_vector);
		_declspec(dllexport) void CreateRuleStrings(WrapDataSource * source);
		_declspec(dllexport) void CreateStringForRule(AssociateRule * _rule , vector<EncodedAttributeInfo *> _atrributes);
		_declspec(dllexport) int No_of_rules() const { return m_no_of_rules; }
		_declspec(dllexport) void No_of_rules(int val) { m_no_of_rules = val; }
		_declspec(dllexport) int No_of_items() const { return m_no_of_items; }
		_declspec(dllexport) void No_of_items(int val) { m_no_of_items = val; }
		_declspec(dllexport) bool ContainsRule(int _hash);

	private:
		int m_support;
		float m_confidence;
		int m_no_of_rules;		
		int m_no_of_items;		
		vector<BitStreamHolder *> m_frequent_item_set;	
		vector<BitStreamHolder *> m_unique_bitmap_holders;
		vector<int> m_added_rules;
		map<int,int> m_index_vector_collection;	
		map<int,int> m_index_attribute_map;	
		map<int,int> m_bitmap_count_holder;	
		vector<unsigned long int> m_unique_values_index_map;		
		vector<BitStreamHolder *> m_candidate_set;	
		vector<AssociateRule *> m_rules;
		
	};
