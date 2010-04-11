#include "StdAfx.h"
#include "AprioriItemset.h"

AprioriItemset::AprioriItemset(void)
{
}

AprioriItemset::~AprioriItemset(void)
{
	delete m_items;
	delete m_bit_stream;
}
int AprioriItemset::GetHashValue(int _no_of_attrib,int * _int_arr)
{
	long result = 0;
	int factor = 1;
	int val = 0;
	for (size_t i = 0; i< _no_of_attrib; i++)
	{
		val = _int_arr[i]+1;		
		result +=  val * factor;
		factor *= 10;
	}

	return (int)result;
}

int AprioriItemset::HashCode()
{
	return GetHashValue(m_attribute_no,m_items);
}

hash_map<int,int> AprioriItemset::GetHashtable(vector<AprioriItemset *> & _itemsets,vector<hash_map<int,AprioriItemset *>> & _hash_itemset_vector)
{
	//In the java counterpart they have set size initially.
	hash_map<int,int> result;
	hash_map<int,AprioriItemset *> item_set_result;
	AprioriItemset * item_set;
	int hash_val = 0;

	//Clearing previous itemset.
	//ClearPreviousItemsSet(_hash_itemset_vector);
	for (size_t i = 0; i < _itemsets.size() ; i++)
	{
		item_set = _itemsets.at(i);
		hash_val = item_set->HashCode();
		//Check if [] operators are fast enough
		result[hash_val] = item_set->Count();
		item_set_result[hash_val] = item_set;		
	}
	_hash_itemset_vector.push_back(item_set_result);
	return result;
}

void AprioriItemset::ClearPreviousItemsSet(vector<hash_map<int,AprioriItemset *>> & _int_item_set_vector)
{
	int size = _int_item_set_vector.size();
	if ( size < 2)
	{
		return;
	}

	hash_map<int,AprioriItemset *> result_map = _int_item_set_vector.at(size-1);
	hash_map<int,AprioriItemset *>::iterator iter;
	AprioriItemset * item_set;
	for (iter = result_map.begin(); iter != result_map.end() ; iter++)
	{
		item_set = iter->second;
		delete item_set->Bit_stream();
		item_set->Bit_stream(NULL);
		delete item_set;
	}	
	_int_item_set_vector.pop_back();
}
