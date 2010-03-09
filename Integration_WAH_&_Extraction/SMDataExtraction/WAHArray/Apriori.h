#pragma once
#include "BitStreamHolder.h"
#include <vector>
#include <iostream>
#include <map>
#include "WrapDataSource.h"
#include "algoutils.h"
#include "associaterule.h"
namespace Algorithm{
	class Apriori
	{
	public:
		typedef vector<int>::const_iterator vector_iter;		
		Apriori(void);
		~Apriori(void);
		int Support() const { return m_support; }
		bool ContainCombination(int _combination);
		void Support(int val) { m_support = val; }
		int Confidence() const { return m_confidence; }
		void Confidence(int val) { m_confidence = val; }
		map<int,int> Index_vector_collection() const { return m_index_vector_collection; }
		void Index_vector_collection(map<int,int> val) { m_index_vector_collection = val; }
		vector<BitStreamHolder *> Frequent_item_set() const { return m_frequent_item_set; }
		void Frequent_item_set(vector<BitStreamHolder *> val) { m_frequent_item_set = val; }
		vector<BitStreamHolder *> Unique_bitmap_holders() const { return m_unique_bitmap_holders; }
		void Unique_bitmap_holders(vector<BitStreamHolder *> val) { m_unique_bitmap_holders = val; }
		map<int,int> Index_attribute_map() const { return m_index_attribute_map; }
		void Index_attribute_map(map<int,int> val) { m_index_attribute_map = val; }
		map<int,int> Bitmap_count_holder() const { return m_bitmap_count_holder; }
		void Bitmap_count_holder(map<int,int> val) { m_bitmap_count_holder = val; }
		vector<unsigned long int> Unique_values_index_map() const { return m_unique_values_index_map; }
		void Unique_values_index_map(vector<unsigned long int> val) { m_unique_values_index_map = val; }
		vector<BitStreamHolder *> Candidate_set() const { return m_candidate_set; }
		void Candidate_set(vector<BitStreamHolder *> val) { m_candidate_set = val; }
		int GetCountForBitMap(int _hash_value);
		void CreateInitialCandidateSet(AlgoUtils & _utils);
		void GenerateCandidates(AlgoUtils & _utils);
		void GetUniqueItems(WrapDataSource * _wrapped,AlgoUtils & _utils);
		void GenerateAllCandidateSets(AlgoUtils & _utils);
		void CalculateCountsForUniqueBitmaps();
		vector<int> GenerateAntedecent(int _index,vector<int> & _frequent_indices);
		int CalculateHash(vector<int> & _vector);


	private:
		int m_support;
		int m_confidence;
		vector<BitStreamHolder *> m_frequent_item_set;	
		vector<BitStreamHolder *> m_unique_bitmap_holders;	
		map<int,int> m_index_vector_collection;	
		map<int,int> m_index_attribute_map;	
		map<int,int> m_bitmap_count_holder;	
		vector<unsigned long int> m_unique_values_index_map;		
		vector<BitStreamHolder *> m_candidate_set;	
		vector<AssociateRule *> m_rules;
	};
}