#include "StdAfx.h"
#include "AlgoUtils.h"
#include <iostream>

using namespace std;

	AlgoUtils::AlgoUtils(void)
	{
	}

	AlgoUtils::~AlgoUtils(void)
	{
	}

	BitStreamInfo * AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container)
	{
		// Replace This with an exception
		size_t pattern_size = _pattern.size();
		pattern_size > 0 && pattern_size == _container.size();
		assert(pattern_size > 0 && pattern_size == _container.size());
		BitStreamInfo * left_op = NULL;
		BitStreamInfo * right_op = NULL;
		BitStreamInfo * left_op_prev = NULL;

		bool left_op_zero = false;
		bool right_op_zero = false;
		size_t index = 0;
		// Flow is seperated according to the size since new BitStreamInfo objects
		//are created in binary operations
		if (_pattern.size() != 1)
		{
			if (_pattern[index] == 0)
			{
				left_op = ~(*(_container.at(index)));
				left_op_zero = true;
				left_op_prev = left_op;
			}else{
				left_op = _container.at(index);
			}

			for (index = 1; index < pattern_size;index++)
			{
				if (_pattern[index] == 0)
				{
					right_op = ~(*(_container.at(index)));
					right_op_zero = true;
				}else{
					right_op = _container.at(index);
				}

				left_op = (*(left_op) & *(right_op));
				if (left_op_zero)
				{
					delete left_op_prev;
					left_op_zero = false;
				}
				if (right_op_zero)
				{
					delete right_op;
					right_op_zero = false;
				}
				right_op = NULL;
			}
		} 
		else
		{
			if (_pattern[index] == 0)
			{
				//New BitStreamInfo object is returned with this unary operator
				left_op = ~(*(_container.at(index)));								
			}else{
				left_op = _container.at(index);
				//A new BitStreamInfo object is obtained via clone
				left_op = left_op->Clone();
			}
		}
		
		//cout << "Bit map : "<< left_op->Decompress() << endl;
		return left_op;
	}

	void AlgoUtils::PrintRules(vector<AssociateRule *> & _rules)
	{
		for (size_t i = 0; i < _rules.size(); i++)
		{
			cout << "Printing Rule: " << endl;
			PrintRule(_rules[i]);
		}
	}
	void AlgoUtils::PrintRule(AssociateRule *_rule)
	{
		cout << "Premise Support : " << _rule->Premise_count() << endl;
		cout << "Consequence Support : " << _rule->Consequence_count() << endl;
		cout << "Confidence : " << _rule->Confidence() << endl;
		cout << "Rule String : " << _rule->Rule() << endl;
		cout << endl;
	}

	void AlgoUtils::PrintAprioriItemSets(vector<vector<AprioriItemset *>> & _items, WrapDataSource * ws)
	{
		for (size_t i = 0; i < _items.size() ; i++)
		{
			cout << "Printing Itemsets of size : " << (i+1) << endl;
			PrintAprioriItemSets(_items.at(i),ws);
		}
	}
//Test for this method failed, need to test rigorously
	void AlgoUtils::GetUniqueBitmaps( EncodedAttributeInfo * _attribute,vector<BitStreamHolder *> & _bitmaps,vector<dynamic_bitset<>> & _unique_patterns,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map )
	{
		int attribute_index = _attribute->attributeID();
		int current_index = (_bitmaps.size() > 0)?(_bitmaps.size()):0;
		//map<int,dynamic_bitset<>> unique_val_map;
		vector<int> bit_streams_indices;
		vector<BitStreamInfo *> bit_streams = _attribute->vBitStreams();
		size_t bit_stream_size = _unique_patterns.size();
		dynamic_bitset<> temp ;
		for (size_t index = 0; index < bit_stream_size ; index++)
		{
			bit_streams_indices.push_back(current_index);
			temp = _unique_patterns.at(index);
			_pattern_index_map.push_back(temp.to_ulong());
			cout << "Bit Stream Index : " << current_index << endl;
			BitStreamHolder * holder = WrapWithHolder(FindPattern(temp,bit_streams),attribute_index,current_index);
			_bitmaps.push_back(holder);	
			//unique_val_map[current_index] = temp;
			current_index++;
		}
		_index_bitmap_map[attribute_index] = bit_streams_indices;
//return unique_val_map;
	}

	vector<BitStreamInfo *> AlgoUtils::FindDistinctValues(EncodedMultiCatAttribute * _attribute)
	{		
		// at() is said to be more efficient than iterator. Try to measure this afterwards.		
		vector<string> unique_vals = _attribute->uniqueValList();
		int max_number_of_bits = _attribute->NoOfVBitStreams();
		vector<BitStreamInfo *> result_bitstreams(unique_vals.size());
		for (size_t index = 0; index < unique_vals.size(); index++)
		{
			dynamic_bitset<> temp_pattern(max_number_of_bits,(unsigned long int)index);
			vector<BitStreamInfo *> temp_bit_info = _attribute->vBitStreams();
			result_bitstreams[index] = FindPattern(temp_pattern,temp_bit_info);
			
		}
	return result_bitstreams;
	}

	map<int,int> AlgoUtils::CreateIndexAttributeMap(map<int,vector<int>> & _index_att_map)
	{
		map<int,int> result_map;
		typedef map<int,vector<int>>::const_iterator map_vect_iter;
		typedef vector<int>::const_iterator vector_iter;
		vector<int> temp_vect;
		for (map_vect_iter start = _index_att_map.begin() ; start != _index_att_map.end(); start++)
		{
			temp_vect = start->second; 
			for (vector_iter iter_2 = temp_vect.begin();iter_2 != temp_vect.end(); iter_2++)
			{
				result_map[*(iter_2)] = start->first;
			}
			
		}
		return result_map;
	}

	void AlgoUtils::PrintHashMapVector( vector<hash_map<int, int>> & _map_vector )
	{
		cout <<"Printing Hash Map Vector :" << endl;
		for (size_t i = 0; i < _map_vector.size(); i++)
		{
			cout << "Index : " << i << endl;
			PrintHashMap(_map_vector.at(i));
		}

		cout << "=============================================" << endl;
		cout << endl;
	}

	void AlgoUtils::PrintHashMap( hash_map<int, int> & _hash_map )
	{
		hash_map<int,int>::iterator iter;
		for (iter = _hash_map.begin(); iter != _hash_map.end(); iter++)
		{
			int m_items = iter->first;
			cout << "Itemset hash (KEY) :" << m_items<<endl;						
			cout << "Value : " <<iter->second << endl;
		}
	}

	void AlgoUtils::GetUniqueBitmaps(WrapDataSource * _source,vector<BitStreamHolder *> & _bitmaps,map<int,vector<int>> & _index_bitmap_map,vector<unsigned long int> & _pattern_index_map)
	{
		vector<EncodedAttributeInfo *> attr = _source->codedAttributes();
		typedef vector<EncodedAttributeInfo *>::const_iterator attr_iter_t;
		EncodedMultiCatAttribute * multi_cat = NULL;
		for (attr_iter_t iter = attr.begin(); iter != attr.end() ; iter++)
		{
			EncodedAttributeInfo * current_att = *(iter);
			multi_cat= static_cast<EncodedMultiCatAttribute *>(current_att);
			vector<dynamic_bitset<>> unique_vals = GenerateUniqueValues(multi_cat->uniqueValList(),multi_cat->NoOfVBitStreams());
			GetUniqueBitmaps(current_att,_bitmaps,unique_vals,_index_bitmap_map,_pattern_index_map);
		}
	}
	
	vector<dynamic_bitset<>> AlgoUtils::GenerateUniqueValues(vector<string> & _string_map,int _no_of_bits)
	{
		vector<dynamic_bitset<>> temp_bitset;
		for (size_t index = 0; index < _string_map.size(); index++)
		{
			dynamic_bitset<> temp(_no_of_bits,(unsigned long)index);
			temp_bitset.push_back(temp);
		}

		return temp_bitset;
	}

	void AlgoUtils::PrintAprioriItemSets(vector<AprioriItemset *> _items)
	{
		cout << "Printing Apriori Item set"<<endl;
		for (size_t i = 0; i < _items.size() ; i++)
		{
			cout << "Printing itemset :"<< i << endl;
			AprioriItemset * temp_itemset = _items.at(i);
			int * m_array = temp_itemset->Items();
			int length = temp_itemset->Attribute_no();
			cout << "Printing Array Elements " << endl;
			for (size_t j = 0; j < length ; j++)
			{
				cout << m_array[j] <<" , " ;
			}

			cout << endl;
			cout << "Printing Bit Stream : " << endl;
			cout << temp_itemset->Bit_stream()->Decompress() << endl;			
			cout << "Printing count : " << temp_itemset->Count() << endl;
			cout << endl;
		}
	}

	void AlgoUtils::PrintAprioriItemSets(vector<AprioriItemset *> _items, WrapDataSource * ws)
	{
		cout << "Printing Apriori Item set"<<endl;
		for (size_t i = 0; i < _items.size() ; i++)
		{
			cout << "Printing itemset :"<< i << endl;
			AprioriItemset * temp_itemset = _items.at(i);
			int * m_array = temp_itemset->Items();
			int length = temp_itemset->Attribute_no();
			cout << "Printing Array Elements " << endl;
			for (size_t j = 0; j < length ; j++)
			{
				cout << m_array[j] <<" , " ;
			}
			cout << "Printing Attributes and values" << endl;
			for (size_t j = 0; j < length ; j++)
			{
				if (m_array[j] != -1)
				{
					EncodedAttributeInfo * attrib = ws->codedAttributes().at(j);
					string att_name = attrib->attributeName();
					EncodedMultiCatAttribute * multi_cat = static_cast<EncodedMultiCatAttribute *>(attrib);
					string val_name = multi_cat->uniqueValList().at(m_array[j]);
					cout << att_name << " : " << val_name<< endl;
				}
				
			}
			
			cout << endl;
			cout << "Printing Bit Stream : " << endl;
			cout << temp_itemset->Bit_stream()->Decompress() << endl;			
			cout << "Printing count : " << temp_itemset->Count() << endl;
			cout << endl;
		}
	}

	void AlgoUtils::CopyFirstToSecond(vector<BitStreamHolder *> & left,vector<BitStreamHolder *> & right)
	{
		back_insert_iterator<vector<BitStreamHolder *>> back_iter(right);
		copy(left.begin(),left.end(),back_iter);
	}

	void AlgoUtils::CopyFirstToSecond(vector<AssociateRule *> & left,vector<AssociateRule *> & right){
		back_insert_iterator<vector<AssociateRule *>> back_iter(right);
		copy(left.begin(),left.end(),back_iter);
	}
	
// 	template<class X> void AlgoUtils::CopyFirstToSecond(vector<X *> & left,vector<X *> & right)
// 	{
// 		back_insert_iterator<vector<X *>> back_iter(right);
// 		copy(left.begin(),left.end(),back_iter);
// 	}

	BitStreamHolder * AlgoUtils::WrapWithHolder(BitStreamInfo *_stream, int _attribute_id, int _bit_map_id)
	{
		BitStreamHolder * result = new BitStreamHolder();
		result->AddAttributeID(_attribute_id);
		result->AddStreamNo(_bit_map_id);
		result->Bit_stream(_stream);
		return result;

	}

// 	BitStreamHolder * AlgoUtils::MergeHolder(BitStreamHolder * _holder,vector<BitStreamHolder *> _index_holder_map)
// 	{
// 		BitStreamHolder * result_holder = new BitStreamHolder();
// 
// 	}

	dynamic_bitset<> AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container)
	{
		size_t pattern_size = _pattern.size();
		assert(pattern_size > 0 && pattern_size == _container.size());
		dynamic_bitset<> left_op;
		dynamic_bitset<> right_op;
		size_t index = 0;
		left_op = _container.at(index);
		if (_pattern[index] == 0)
		{
			left_op.flip();
		}
		for (index =1; index < pattern_size ; index++)
		{
			right_op = _container[index];
			if (_pattern[index] == 0)
			{
				right_op.flip();
			}
			left_op = (left_op & right_op);
		}

		return left_op;
	}
