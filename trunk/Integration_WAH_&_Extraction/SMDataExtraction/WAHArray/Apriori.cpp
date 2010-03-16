#include "StdAfx.h"
#include "Apriori.h"
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;
namespace Algorithm{
	Apriori::Apriori(void)
	{
		m_no_of_rules = 1000;
		m_no_of_items = 1000;
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
	 
	 	void Apriori::GenerateRules()
	 	{
			//Handle for empty occurance
			typedef vector<BitStreamHolder *>::const_iterator vector_iter;
			for (vector_iter start = m_frequent_item_set.begin(); (start != m_frequent_item_set.end() && m_rules.size() < m_no_of_rules ); start++)
			{
				GenerateRulesForHolder(*(start));
			}
	 	}



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
				int hash_val_consequent = CalculateHash(consequent);
				int rule_hash = hash_val_antecedent * 10 +hash_val_consequent;
if (!ContainsRule(rule_hash))
{
					m_added_rules.push_back(rule_hash);
					int confidence_ =0;
	// 				if (m_bitmap_count_holder[hash_val_consequent] > 0)
	// 				{
	// 					 confidence_= ((int)(support_for_holder/m_bitmap_count_holder[hash_val_consequent])*100);
	// 					if (confidence_ >= m_confidence)
	// 					{
	// 						AssociateRule * rule = new AssociateRule();
	// 						rule->Antecedant(consequent);
	// 						rule->Consequent(antecedant);
	// 						rule->Support(support_for_holder);
	// 						rule->Confidence(confidence_);
	// 						m_rules.push_back(rule);
	// 					}
	// 				}
	
					confidence_ = (((float)support_for_holder/(float)m_bitmap_count_holder[hash_val_antecedent])*(float)100);
					if (confidence_ >= m_confidence && confidence_ <=100)
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

	}

// 	void Apriori::GenerateStringRules()
// 	{
// 			
// 	}

	void Apriori::RunAlgorithm(WrapDataSource * source)
	{
		//time_t start,end1,end2,end3;
		AlgoUtils utils;

		cout<<"Start to run algorithm: "<<endl;
		//start = clock();
		GetUniqueItems(source,utils);
		cout<<"Finished creating unique bimaps: "<<endl;
		CreateInitialCandidateSet(utils);
		cout<<"Finished CreateInitialCandidateSet: "<<endl;
  		GenerateAllCandidateSets(utils);
		//end1 = clock();
  		cout<<"Finished GenerateAllCandidateSets :"<<endl;
		cout << "Generating Strings : " << endl;
		GenerateRules();
		//end2 = clock();
		CreateRuleStrings(source);
		cout<<"Finished creating rules: "<<endl;
		//cout<<"Time - Generating Frequent Item sets : "<<(end1 - start)<<endl;
		//cout<<"Time - Generating Rules : "<<(end2 - start)<<endl;
	}

	void Apriori::CreateRuleStrings(WrapDataSource * source)
	{
		vector<EncodedAttributeInfo *> _att_vector = source->codedAttributes();
		for (size_t index = 0; index < m_rules.size(); index++)
		{
			CreateStringForRule(m_rules.at(index),_att_vector);
		}
	}

	void Apriori::CreateStringForRule(AssociateRule * _rule , vector<EncodedAttributeInfo *> _atrributes)
	{
		vector<int> antecedent = _rule->Antecedant();
		string antecedant_str = GetStringForVector(_atrributes,antecedent);
		vector<int> consequent = _rule->Consequent();
		string consequent_str = GetStringForVector(_atrributes,consequent);
		string total_rule = "";
// 		strcat(total_rule,antecedant_str);
// 		strcat(total_rule," => ");
// 		strcat(total_rule,consequent_str);
		total_rule += antecedant_str;
		total_rule += " => ";
		total_rule += consequent_str;
		//cout << "Total Val : " << total_rule << endl;
		//cout << "Antecedent : " << antecedant_str << endl;
		_rule->Rule(total_rule);
	}

	bool Apriori::ContainsRule(int _hash)
	{
		sort(m_added_rules.begin(),m_added_rules.end());
		return binary_search(m_added_rules.begin(),m_added_rules.end(),_hash);
	}

	string Apriori::GetStringForVector(vector<EncodedAttributeInfo *> & _atrributes,vector<int> & _index_vector)
	{
		string result="";
		for (size_t index = 0; index < _index_vector.size();index++)
		{
			int attribute = m_index_attribute_map[_index_vector.at(index)];
			
			//Retrieving from attribute Id is handled this way
			int attribute_vect_id = (_atrributes.size()-1) -attribute;
			string attribute_name = _atrributes.at(attribute_vect_id)->attributeName();
			unsigned long int unique_val = m_unique_values_index_map.at(index);
			EncodedMultiCatAttribute * multi_cat = static_cast<EncodedMultiCatAttribute *>(_atrributes.at(attribute_vect_id));
			string val = multi_cat->uniqueValList().at(unique_val);
// 			strcat(result," , ");
// 			strcat(result,val);
			result += (attribute_name +" = "+val);
			if (index != _index_vector.size()-1)
			{
				result += " , ";
			}
			
		}
		return result;
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
		sort(_vector.begin(),_vector.end());
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
		int initial_size = 0;
		int termination_size = 0;
		do 
		{
			initial_size = m_candidate_set.size();
			GenerateCandidates(_utils);
			termination_size = m_candidate_set.size();
		} while (m_candidate_set.size() > 0 && (initial_size != termination_size) && m_frequent_item_set.size() < m_no_of_items);
	}


	void Apriori::CreateInitialCandidateSet(AlgoUtils & _utils)
	{		
		for (size_t index = 0; (index < m_unique_bitmap_holders.size() && m_candidate_set.size() < m_no_of_items); index++)
		{
			for (size_t inner_index = index +1 ; inner_index < m_unique_bitmap_holders.size() &&  m_candidate_set.size() < m_no_of_items; inner_index++ )
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