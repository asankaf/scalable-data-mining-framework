#include "StdAfx.h"
#include "Apriori.h"



namespace Algorithm{
	Apriori::Apriori(void)
	{
	}

	Apriori::~Apriori(void)
	{
	}

	void Apriori::GetUniqueItems(WrapDataSource * _wrapped,AlgoUtils & _utils)
	{
		
		vector<BitStreamHolder *> holder;
		map<int,vector<int>> pre_att_index_map;
		_utils.GetUniqueBitmaps(_wrapped,m_unique_bitmap_holders,pre_att_index_map,m_unique_values_index_map);
		m_index_attribute_map = _utils.CreateIndexAttributeMap(pre_att_index_map);
		CalculateCountsForUniqueBitmaps();

	}
// 
// 	void Apriori::GenerateRules()
// 	{
// 
// 	}

	void Apriori::GenerateRulesForHolder(BitStreamHolder * _holder)
	{
		//Handle exceptions(division by zero is a must)
		int support_for_holder = m_bitmap_count_holder[_holder->Hash()];
		if (support_for_holder > 0)
		{
			for (size_t index = 0; index < _holder->Bit_stream_no().size(); index++)
			{

				vector<int> antecedant = GenerateAntedecent(index,_holder->Bit_stream_no());
				vector<int> consequent;
				consequent.push_back(_holder->Bit_stream_no().at(index));
				int hash_val_antecedent = CalculateHash(antecedant);
				int confidence_ = (int)(support_for_holder/m_bitmap_count_holder[_holder->Bit_stream_no().at(index)]);
				if (confidence_ >= m_confidence)
				{
					AssociateRule * rule = new AssociateRule();
					rule->Antecedant(consequent);
					rule->Consequent(antecedant);
					rule->Support(support_for_holder);
					rule->Confidence(confidence_);
					m_rules.push_back(rule);
				}

				confidence_ = (int)(support_for_holder/hash_val_antecedent);
				if (confidence_ >= m_confidence)
				{
					AssociateRule * rule = new AssociateRule();
					rule->Antecedant(antecedant);
					rule->Consequent(consequent);
					rule->Support(support_for_holder);
					rule->Confidence(confidence_);
					m_rules.push_back(rule);
				}


			}
		}

	}

	vector<int> Apriori::GenerateAntedecent(int _index,vector<int> & _frequent_indices)
	{
		vector<int> result;
		//Very inefficient method. Should be replaced immediately.
		//Try to delete the element and perform a copy. 
		for (size_t index = 0; index < _frequent_indices.size() ; index++)
		{
			if (index != _index)
			{
				result.push_back(_frequent_indices.at(index));
			}
		}
		return result;
	}

	int Apriori::CalculateHash(vector<int> & _vector)
	{
		
		int factor = 1;
		int hash = 0;
		for (vector_iter start = _vector.begin(); start != _vector.end(); start++)
		{
			hash += (*(start)+1)*factor;
			factor *=10;
		}
		return hash;
	}


	void Apriori::CalculateCountsForUniqueBitmaps()
	{
		typedef vector<BitStreamHolder *>::const_iterator vector_iter;
		for (vector_iter start = m_unique_bitmap_holders.begin() ; start != m_unique_bitmap_holders.end(); start++)
		{
			BitStreamHolder * temp_holder = *(start);
			m_bitmap_count_holder[temp_holder->Hash()] = temp_holder->Count();
		}
	}

	int Apriori::GetCountForBitMap(int _hash_value)
	{
		map<int,int>::iterator iter = m_bitmap_count_holder.find(_hash_value);
		return iter->second;
	}

	bool Apriori::ContainCombination(int _combination)
	{
		return (m_bitmap_count_holder.find(_combination) != m_bitmap_count_holder.end());
	}

	void Apriori::GenerateCandidates(AlgoUtils & _utils)
	{
		vector<BitStreamHolder *> candidate_set_copy = m_candidate_set;
		m_candidate_set.clear();
		typedef vector<BitStreamHolder *>::const_iterator holder_iter;
		for (size_t index = 0; index < candidate_set_copy.size(); index++)
		{
			for (size_t inner_index = 0 ; inner_index < candidate_set_copy.size(); inner_index++ )
			{
				if (inner_index != index)
				{
					BitStreamHolder * main_holder = candidate_set_copy.at(index);
					vector<BitStreamHolder *> temp_holder_vect = main_holder->Difference(candidate_set_copy.at(inner_index),m_index_attribute_map);
					
					for (holder_iter iter = temp_holder_vect.begin(); iter != temp_holder_vect.end(); iter++)
					{
						BitStreamHolder * temp_holder = main_holder->Merge(*(iter),m_index_attribute_map,m_unique_bitmap_holders);
						if (temp_holder != NULL)
						{
							int temp_holder_hash = temp_holder->Hash();
							int holder_count =  temp_holder->Count();
							if (!ContainCombination(temp_holder_hash) && (holder_count >= m_support))
							{
								m_bitmap_count_holder[temp_holder_hash] = holder_count;
								m_candidate_set.push_back(temp_holder);
							}	
						}
					}

				}
			}
		}
		//Inefficient memory handling. Need to delete BitStreamInfoObjects, when no longer used.		
		_utils.CopyFirstToSecond(m_candidate_set,m_frequent_item_set);
	}

 	void Apriori::GenerateAllCandidateSets(AlgoUtils & _utils)
 	{
		do 
		{
			GenerateCandidates(_utils);
		} while (m_candidate_set.size() > 0);
 	}
 	

	void Apriori::CreateInitialCandidateSet(AlgoUtils & _utils)
	{		
		for (size_t index = 0; index < m_unique_bitmap_holders.size(); index++)
		{
			for (size_t inner_index = index +1 ; inner_index < m_unique_bitmap_holders.size(); inner_index++ )
			{
				if (inner_index != index)
				{
					BitStreamHolder * temp_holder = (m_unique_bitmap_holders.at(index))->Merge(m_unique_bitmap_holders.at(inner_index),m_index_attribute_map,m_unique_bitmap_holders);
					if (temp_holder != NULL)
					{
						int temp_holder_hash = temp_holder->Hash();
						int holder_count =  temp_holder->Count();
						if (!ContainCombination(temp_holder_hash) && (holder_count >= m_support))
						{
							m_bitmap_count_holder[temp_holder_hash] = holder_count;
							m_candidate_set.push_back(temp_holder);
						}	
					}
				
				}
			}
		}
		//Inefficient memory handling. Need to delete BitStreamInfoObjects, when no longer used.		
		_utils.CopyFirstToSecond(m_candidate_set,m_frequent_item_set);
	}	
}