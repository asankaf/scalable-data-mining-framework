#include "StdAfx.h"
#include "AprioriOpt.h"
#include <math.h>
#include <iosfwd>
#include "utils.h"
#include "smalgorithmexceptions.h"
#include "AttributeType.h"


using namespace std;

AprioriOpt::AprioriOpt(void)
{
	//m_minimumSupport = 0.1;
	m_minSupport = 0.1;
	m_upperBoundMinSupport = 1.0;
	m_lowerBoundMinSupport = 0.1;
	m_numberOfAttributes = 0;
	m_delta = 0.05;
	m_numRules = 10;
}

AprioriOpt::~AprioriOpt(void)
{
	ClearAll();
}

void AprioriOpt::FindUniqueItemSets( WrapDataSource * _instances ) throw (algorithm_exception)
{
	m_instances = _instances;
	if (_instances == NULL)
	{
		BOOST_THROW_EXCEPTION(empty_data_source_exception(SM3001));
	}
	vector<EncodedAttributeInfo *> attributes =  m_instances->codedAttributes();
	m_numberOfAttributes = attributes.size();
	AlgoUtils utils;
	// at() is said to be more efficient than iterator. Try to measure this afterwards.
	for (size_t attribute_index = 0; attribute_index < m_numberOfAttributes;attribute_index++)
	{
		if (attributes.at(attribute_index)->attributeType() != MULTICAT_VAL)
		{
			BOOST_THROW_EXCEPTION(incompatible_data_type_exception(SM3002));
		}
		EncodedMultiCatAttribute * multi_cat_attrib = static_cast<EncodedMultiCatAttribute *>(attributes.at(attribute_index));
		vector<string> unique_vals = multi_cat_attrib->uniqueValList();
		int max_number_of_bits = multi_cat_attrib->NoOfVBitStreams();
		for (size_t index = 0; index < unique_vals.size(); index++)
		{
			dynamic_bitset<> temp_pattern(max_number_of_bits,(unsigned long int)index);
			vector<BitStreamInfo *> temp_bit_info = multi_cat_attrib->vBitStreams();
			BitStreamInfo * info = utils.FindPattern(temp_pattern,temp_bit_info);
			AprioriItemset * item_set = new AprioriItemset();
			int * int_array = new int[m_numberOfAttributes];
			for (size_t i = 0; i < m_numberOfAttributes ; i++)
			{
				int_array[i] = -1;
			}
			int_array[attribute_index] = (int)index;
			item_set->Bit_stream(info);
			item_set->Count(info->Count());			
			item_set->Items(int_array);
			item_set->Attribute_no(m_numberOfAttributes);
			m_uniqueItems.push_back(item_set);
		}

	}
}

void AprioriOpt::BuildAssociations(WrapDataSource * _instances)
{

	if (m_minSupport <= 0 || 
	m_upperBoundMinSupport <= 0 ||
	m_lowerBoundMinSupport <= 0 ||	
	m_delta <= 0 ||
	m_numRules < 0)
	{
		BOOST_THROW_EXCEPTION(invalid_parameter_exception(SM3003));
	}

	if (m_minSupport > 1 || m_delta > 1)
	{
		BOOST_THROW_EXCEPTION(invalid_parameter_exception(SM3003));
	}
	vector<AssociateRule *> sortedRuleSet;
	double necSupport=0;	
	m_cycles = 0;
	//Calculating unique itemsets for the dataset.
	//Parameters like number of attributes are set in this function.

	FindUniqueItemSets(_instances);

	// make sure that the lower bound is equal to at least one instance
	double lower_bound_min_support_to_use = 
		(m_lowerBoundMinSupport * (double)_instances->noOfRows() < 1.0)
		? 1.0 / (double)(_instances->noOfRows()) 
		: m_lowerBoundMinSupport;	


	// Decrease minimum support until desired number of rules found.
	m_minSupport = m_upperBoundMinSupport - m_delta;
	m_minSupport = (m_minSupport < lower_bound_min_support_to_use) 
		? lower_bound_min_support_to_use 
		: m_minSupport;
	//FindUniqueItemSets(_instances);

	do {			
		//Clearing Results generated from previous iterations.
		ClearlargeItemSets();
		m_hashTables.clear();
		m_hashItemSets.clear();
		ClearRules();

		// Find large itemsets and rules
		FindLargeItemSets();
		//findRulesQuickly();
		FindRulesQuickly(m_rules);


		if(m_minSupport == lower_bound_min_support_to_use || m_minSupport - m_delta >  lower_bound_min_support_to_use)
			m_minSupport -= m_delta;
		else
			m_minSupport = lower_bound_min_support_to_use;


		necSupport = (int)(m_minSupport * (double)m_numberOfAttributes);

		m_cycles++;
	} while (
		(m_rules.size() < m_numRules) 
		&&
		m_minSupport >= lower_bound_min_support_to_use
		&&     
		(necSupport >= 1)
		);
	m_minSupport += m_delta;
	SortRules();
	BuildStrings();	
}

void AprioriOpt::SortRules()
{
	multimap<float,int> conf_index_map;
	pair<float,int> p;

	float sort_val = 0;
	int exponent = (int)ceil(log10((float)m_instances->noOfRows()));
	int factor = pow((float)10,exponent);

	for (int i = 0 ; i < m_rules.size(); i++)
	{
		sort_val = (float)(m_rules[i]->Confidence() * factor + m_rules[i]->Consequence_count());
		p = pair<float,int>(sort_val,i);
		conf_index_map.insert(p);
	}
	multimap<float,int>::iterator iter;
	vector<AssociateRule *> rules(m_numRules);
	size_t rule_no = 0;
	iter = conf_index_map.end();
	iter--;
	for (; iter != (conf_index_map.begin()) && rule_no < m_numRules; iter--,rule_no++)
	{
		rules[rule_no] = m_rules[iter->second];
	}
	//
	if (iter == conf_index_map.begin() && rule_no < m_numRules)
	{
		rules[rule_no++] = m_rules[iter->second];
	}
	rules.resize(rule_no);
	m_rules = rules;

}

int AprioriOpt::Compare(const void * arg1, const void * arg2)
{
	return (int)(*static_cast<const float *>(arg1) - *static_cast<const float *>(arg2));
}

void AprioriOpt::FindLargeItemSets(){

	vector<AprioriItemset *> kMinusOneSets, kSets;
	hash_map<int,int> hashtable;
	int necSupport, necMaxSupport,numOfRows,i = 0;
	numOfRows = this->m_instances->noOfRows();


	// Find large item sets

	// minimum support

	necSupport = (int)(this->m_minSupport * (double)numOfRows+0.5);
	necMaxSupport = (int)(this->m_upperBoundMinSupport * (double)numOfRows+0.5);

	kSets = m_uniqueItems;
	kSets = DeleteItemSets(kSets,necSupport,necMaxSupport,false); 	 
	if (kSets.size() == 0)
		return;
	do {
		m_largeItemSets.push_back(kSets);
		kMinusOneSets = kSets;
		kSets = MergeAllItemSets(kMinusOneSets, i);
		hashtable = AprioriItemset::GetHashtable(kMinusOneSets,m_hashItemSets);
		
		// Find if this step is really necessary
		m_hashTables.push_back(hashtable);
		kSets = PruneItemSets(kSets, hashtable);
		UpdateCounters(kSets, i);
		kSets = DeleteItemSets(kSets, necSupport, necMaxSupport,true);
		i++;
	} while (kSets.size() > 0);
}


string AprioriOpt::toString()
{
	string output = "";
	
	for (size_t i = 0 ; i < m_rules.size() ; i++)
	{
		string rule = m_rules[i]->Rule();
		output += rule +"\n";
	}
	return output;
}

void AprioriOpt::UpdateCounters(vector<AprioriItemset *> & _ksets,int _kminusize)
{
	// Logic is if a particular itemset is frequent then all of its sub sets should be frequent

	hash_map<int,AprioriItemset *> unique_val_itemset_hash_table = m_hashItemSets.at(0);
	hash_map<int,AprioriItemset *> kminus_itemset_hash_table = m_hashItemSets.at(m_hashItemSets.size() - 1);

	AprioriItemset * single_item_set ;
	AprioriItemset * kminus_item_set;
	BitStreamInfo * result_bit_stream ;

	BitStreamInfo * left_op;
	BitStreamInfo * right_op;
	BitStreamInfo * result;

	int help;
	int * unique_item_ptr;
	unique_item_ptr = new int[m_numberOfAttributes];
	for (size_t i = 0; i < _ksets.size() ; i++)
	{
		AprioriItemset * current_item_set = _ksets.at(i);
		int * current_m_items = current_item_set->Items();

		for (size_t j = 0;j < m_numberOfAttributes ; j++)
		{			
			if (current_m_items[j] != -1)
			{
				for (size_t k=0 ;k < m_numberOfAttributes;k++)
				{
					unique_item_ptr[k] = -1;
				}
				help = current_m_items[j];
				current_m_items[j] = -1;
				unique_item_ptr[j] = help;

				int single_hash_val = AprioriItemset::GetHashValue(m_numberOfAttributes,unique_item_ptr);
				single_item_set = unique_val_itemset_hash_table[single_hash_val];
				left_op = single_item_set->Bit_stream();

				kminus_item_set = kminus_itemset_hash_table[current_item_set->HashCode()];
				right_op = kminus_item_set->Bit_stream();

				result = *(left_op) & *(right_op);
				current_m_items[j] = help;
				current_item_set->Bit_stream(result);
				current_item_set->Count(result->Count());
				break;
			}
		}
	}
	delete unique_item_ptr;
}

vector<AprioriItemset *> AprioriOpt::PruneItemSets(vector<AprioriItemset *> & _ksets,hash_map<int,int> & _kMinusOne)
{
	//Java counterpart initializes the vector to a predefined size
	vector<AprioriItemset *> result;	
	int help, j;
	int * current_m_items;
	for (size_t i = 0; i < _ksets.size(); i++) {
		AprioriItemset * current = _ksets.at(i);
		for (j = 0; j < m_numberOfAttributes; j++){
			current_m_items = current->Items();
			if (current_m_items[j] != -1) {
				help = current_m_items[j];
				current_m_items[j] = -1;
				// Getting the hash could be inefficient as it is calculated at the time needed
				//Try to calculate hash and store it
				if (_kMinusOne[current->HashCode()] == 0) {
					current_m_items[j] = help;
					break;
				} else{ 
					current_m_items[j] = help;
				}
			}
		}
		// Checks if the array has come to its end
		// Length of the array is equal to the number of attributes in the table
		if (j == m_numberOfAttributes){ 
			result.push_back(current);
		}else
		{
			delete current;			
		}
	}

	return result;
}

vector<AprioriItemset *> AprioriOpt::MergeAllItemSets(vector<AprioriItemset *> & _itemSets, int size){

	vector<AprioriItemset *> new_vector;
	AprioriItemset  * result;
	int numFound, k;

	for (size_t i = 0; i < _itemSets.size(); i++) {
		AprioriItemset * first = _itemSets.at(i);
	
		for (int j = i+1; j < _itemSets.size(); j++) {
			AprioriItemset * second = _itemSets.at(j);
			result = new AprioriItemset();
			int * m_items = new int[m_numberOfAttributes];
			result->Items(m_items);

			// Find and copy common prefix of size 'size'
			// This assignment may impose certain overhead over the java counterpart
			// Make the m_items public if necessary
			int * first_items = first->Items();
			int * second_items = second->Items();
			numFound = 0;
			k = 0;
			bool external_for_break = false;
			while (numFound < size) {
				if (first_items[k] == second_items[k]) {
					if (first_items[k] != -1) 
						numFound++;
					m_items[k] = first_items[k];
				} else 
				{
					external_for_break = true;
					break;					
				}
				k++;
			}
			if (external_for_break)
			{
				delete result;
				break;
			}

			// Check difference
			// Each array has a length equal to the total attributes of the data source
			while (k < m_numberOfAttributes) {
				if ((first_items[k] != -1) && (second_items[k] != -1))
					break;
				else {
					if (first_items[k] != -1)
						m_items[k] = first_items[k];
					else
						m_items[k] = second_items[k];
				}
				k++;
			}
			if (k == m_numberOfAttributes) {
				result->Count(0);
				result->Attribute_no(m_numberOfAttributes);
				new_vector.push_back(result);
			}
		}
	}
	return new_vector;
}

vector<AprioriItemset *> AprioriOpt::DeleteItemSets(vector<AprioriItemset *> & _itemsets,int _nec_support, int _max_support,bool _not_initial_set)
{
	// Use this for now. If this method proves to be inefficient use a dynamic array instead
	vector<AprioriItemset *> result;
	for (size_t i = 0; i < _itemsets.size();i++)
	{
		AprioriItemset * item_set = _itemsets.at(i);
		// Mind if this assignment and trivial check becomes a performance hit
		int count = item_set->Count();
		if (count >= _nec_support && count <= _max_support)
		{
			result.push_back(item_set);
		}else{
			if (_not_initial_set)
			{
				delete item_set;
				item_set = NULL;
			}
		}
	}
	return result;
}

vector<AssociateRule *> AprioriOpt::GenerateRules( int numItemsInSet, AprioriItemset * _itemset )
{	
	hash_map<int,int> hashtable = m_hashTables.at(numItemsInSet - 2);
	int * premise_m_items;
	int * consequence_m_items;
	vector<AssociateRule *> rules;
	AssociateRule * rule = NULL;

	// Generate all rules with one item in the consequence.
	int * current_m_items = _itemset->Items();
	int m_items_length = m_numberOfAttributes;
	for (int i = 0; i < m_items_length; i++) {
		if (current_m_items[i] != -1) {
			premise_m_items = new int[m_items_length];
			consequence_m_items = new int[m_items_length];
			rule = new AssociateRule();
			rule->Premise(premise_m_items);
			rule->Consequence(consequence_m_items);

			rule->Consequence_count(_itemset->Count());

			for (int j = 0; j < m_items_length; j++) 
			{
				consequence_m_items[j] = -1;
			}
			// Check if there's a separate method for copying arrays. THis is for copying character arrays
			memcpy(premise_m_items,current_m_items,m_items_length * sizeof(int)); 				
			premise_m_items[i] = -1;

			consequence_m_items[i] = current_m_items[i];
			rule->Premise_count(hashtable[AprioriItemset::GetHashValue(m_items_length,premise_m_items)]);
			//_confidences.push_back(rule->CalculateConfidence());
			rule->CalculateConfidence();
			rules.push_back(rule);						
		}
	}
		rules = PruneRules(rules);		
		return rules;
}

void AprioriOpt::FindRulesQuickly(vector<AssociateRule *> & _rules)
{

	vector<AssociateRule *> rules;	

	// Build rules
	for (int j = 1; j < m_largeItemSets.size(); j++) {
		vector<AprioriItemset *> currentItemSets = m_largeItemSets.at(j);

		for (size_t index = 0; index < currentItemSets.size(); index++)
		{			
			AprioriItemset * currentItemSet = currentItemSets.at(index);
			rules = GenerateRules(j + 1,currentItemSet);			
			AlgoUtils::CopyFirstToSecond(rules,_rules);		

		}

	}
}
void AprioriOpt::ClearAll()
{	
	ClearlargeItemSets();
	ClearHashTable();
	ClearUniqueItems();		
	ClearRules();
}

void AprioriOpt::ClearlargeItemSets()
{		
	// First element refers to unique itemset, which is cleaned by a seperate function.
	size_t i = 1;
	
	for (; i < m_largeItemSets.size() ; i++)
	{
		for (size_t j = 0 ; j < m_largeItemSets[i].size(); j++)
		{
			m_hashItemSets[i][m_largeItemSets[i][j]->HashCode()] = NULL;
			delete m_largeItemSets[i][j];
		}
		m_largeItemSets[i].clear();
	}
	m_largeItemSets.clear();
}

void AprioriOpt::ClearUniqueItems()
{
	for (size_t i = 0 ; i < m_uniqueItems.size(); i++)
	{
		delete m_uniqueItems[i];	
	}
	m_uniqueItems.clear();
}

void AprioriOpt::ClearHashTable()
{	
	// i =0 contains pointers to unique items. So it cannot be deleted
	for (size_t i = 1 ; i < m_hashItemSets.size(); i++)
	{
		hash_map<int,AprioriItemset *> hash_table = m_hashItemSets[i];
		hash_map<int,AprioriItemset *>::iterator iter;
		for (iter = hash_table.begin(); iter != hash_table.end() ; iter++)
		{
			if (iter->second != NULL)
			{
				delete iter->second;
			}
			
		}
		hash_table.clear();
	}
	m_hashTables.clear();
	m_hashItemSets.clear();
}

void AprioriOpt::ClearRules()
{
	for (size_t i = 0 ; i < m_rules.size() ; i++)
	{
		delete m_rules[i];
	}
	m_rules.clear();
}

void AprioriOpt::BuildStrings()
{
	AssociateRule * rule;
	int * premise_items,* consequence_items;
	vector<EncodedAttributeInfo * > attribs = m_instances->codedAttributes();
	EncodedMultiCatAttribute * multi_cat = NULL;

	for (size_t i = 0; i < m_rules.size();i++)
	{
		string premise = "";
		rule = m_rules[i];
		premise_items = rule->Premise();
		consequence_items = rule->Consequence();

		size_t j = 0;
		for (; j < m_numberOfAttributes ; j++)
		{
			if (premise_items[j] != -1)
			{
				multi_cat = static_cast<EncodedMultiCatAttribute *>(attribs[j]);
				vector<string> str_vals = multi_cat->uniqueValList();
				premise += multi_cat->attributeName()+"="+str_vals[premise_items[j]]+" , ";				
			}
		}
		if (j >= m_numberOfAttributes)
		{
			//Find if this operation is fast
			premise.erase((premise.end()-1));
			premise.erase((premise.end()-1));			
			premise += "("+Utils::toStringVal(rule->Premise_count())+")";			
		}
		j=0;
		string consequence = "";
		for (; j < m_numberOfAttributes ; j++)
		{

			if (consequence_items[j] != -1)
			{
				multi_cat = static_cast<EncodedMultiCatAttribute *>(attribs[j]);
				consequence += multi_cat->attributeName()+"="+multi_cat->uniqueValList()[consequence_items[j]]+" , ";				
			}
		}
		if (j == m_numberOfAttributes)
		{
			//Find if this operation is fast
			consequence.erase((consequence.end()-1));
			consequence.erase((consequence.end()-1));
			consequence +="("+Utils::toStringVal(rule->Consequence_count())+")";			
		}
		consequence += " conf = " + Utils::toStringVal(rule->Confidence());
		rule->Rule(premise + " => "+consequence);
	}
}

void AprioriOpt::Clear()
{
	m_hashTables.clear();
	m_largeItemSets.clear();
	m_numberOfAttributes = 0;
	m_rules.clear();
}

vector<AssociateRule *> AprioriOpt::PruneRules( vector<AssociateRule *> & _rules)
{
	vector<AssociateRule * > new_rules;
	AssociateRule * temp_rule;
	size_t size = _rules.size();
	for (size_t i = 0; i < size; i++)
	{
		temp_rule = _rules.at(i);
		if (temp_rule->Confidence()>= m_confidence)
		{
			new_rules.push_back(_rules[i]);
		}else{
			delete temp_rule;
		}
	}
	return new_rules;
}
