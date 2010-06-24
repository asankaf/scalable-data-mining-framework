#include "StdAfx.h"
#include "AlgoUtils.h"
#include <iostream>
#include "AttributeType.h"
#include "WAHStructure.h"
#include "ewah.h"

using namespace CompressedStructure;
using namespace std;

	AlgoUtils::AlgoUtils(void)
	{
	}

	AlgoUtils::~AlgoUtils(void)
	{
	}

	BitStreamInfo * AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container) throw (algorithm_exception)
	{
		size_t pattern_size = _pattern.size();
		pattern_size > 0 && pattern_size == _container.size();

		if(pattern_size > 0 && pattern_size != _container.size())
		{
			BOOST_THROW_EXCEPTION(invalid_pattern_exception());
		}
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
				left_op = _container.at(index)->Clone();
				left_op_prev = left_op;
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
				delete left_op_prev;
				left_op_prev = left_op;
				if (left_op_zero)
				{					
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
		
		return left_op;
	}

	double AlgoUtils::ANDCount( BitStreamInfo * left_op, BitStreamInfo * right_op ) throw (algorithm_exception)
	{
		if (left_op == NULL || right_op == NULL)
		{
			BOOST_THROW_EXCEPTION(null_parameter_exception());
		}
		double result = 0;
		BitStreamInfo * inf = *(left_op) & *(right_op);
		result = inf->Count();
		delete inf;
		return result;
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

	BitStreamInfo * AlgoUtils::UGreaterThan( EncodedAttributeInfo * attribute, double value,int rows ) throw(algorithm_exception)
	{
		BitStreamInfo * info = NULL;	
		switch(attribute->attributeType())
		{
		case SIGNEDINT_VAL:
			{
				unsigned long max_val = static_cast<EncodedIntAttribute *>(attribute)->maxAttVal();
				info = UGreaterThanInt(attribute,(unsigned long long)value,rows,max_val);
				break;
			}
		case DOUBLE_VAL:
			{
				EncodedDoubleAttribute * double_att =  static_cast<EncodedDoubleAttribute *>(attribute);
				long precission = double_att->Precision();
				unsigned long long max_value = (unsigned long long)double_att->maxAttVal() * precission;
				info = UGreaterThanInt(attribute,value * precission , rows,max_value);
				break;
			}
		case DATE_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				
			}
		case MULTICAT_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				
			}
		}
		return info;
	  }

	BitStreamInfo * AlgoUtils::BitStreamGenerator( EncodedAttributeInfo * attribute,dynamic_bitset<> & _bit_stream ) throw (algorithm_exception)
	{

		if (attribute == NULL)
		{
			BOOST_THROW_EXCEPTION(null_parameter_exception());
		}
		   BitStreamInfo::vertical_bit_type type = attribute->bitStreamAt(0)->Type();		   
			BitStreamInfo * new_stream = NULL;
		   switch(type)
		   {
		   case BitStreamInfo::VERTICAL_STREAM_FORMAT :
			   {
				new_stream = new VBitStream();	   
			   }
			   break;
		   case BitStreamInfo::WAH_COMPRESSION:
			   {
				   new_stream = new WAHStructure();
			   }
			   break;
		   case BitStreamInfo::EWAH_COMPRESSION:
			   {
				   new_stream = new EWAH();
			   }
			   break;		   
		   }
		   new_stream->CompressWords(_bit_stream);
		    return new_stream;
	   }

	   BitStreamInfo * AlgoUtils::UEq( EncodedAttributeInfo * attribute, double value, int rows )
	   {
		   switch(attribute->attributeType())
		   {
		   case SIGNEDINT_VAL:
			   {
				unsigned long max_value = static_cast<EncodedIntAttribute *>(attribute)->maxAttVal();
				   if (value > max_value)
				   {
						dynamic_bitset<> result_map(rows);
					   BitStreamInfo * result = BitStreamGenerator(attribute,result_map);
					   return result;
				   }
				   dynamic_bitset<> pattern_val((int)attribute->NoOfVBitStreams(),value);
				   return FindPattern(pattern_val,attribute->vBitStreams());
			   }
			   break;

		   case DOUBLE_VAL:
			   {

				   EncodedDoubleAttribute * double_att = static_cast<EncodedDoubleAttribute *>(attribute);				   
				   unsigned long long max_value = static_cast<EncodedDoubleAttribute *>(attribute)->maxAttVal();

				   if ((unsigned long long)value > max_value)
				   {
					   dynamic_bitset<> result_map(rows);
					   BitStreamInfo * result = BitStreamGenerator(attribute,result_map);
					   return result;
				   }
				   value = value * double_att->Precision();
				   dynamic_bitset<> pattern_val((int)attribute->NoOfVBitStreams(),value);
				   return FindPattern(pattern_val,attribute->vBitStreams());
			   }
		   }
		   
	   }

	   BitStreamInfo * AlgoUtils::UGreaterThanOrEq(EncodedAttributeInfo * attribute, unsigned long value,int rows)
	   {
		   BitStreamInfo * result = NULL;
		   switch(attribute->attributeType())
		   {
		   case SIGNEDINT_VAL:
			   {
					BitStreamInfo * greater_than = UGreaterThan(attribute,value,rows);
					BitStreamInfo * equal = UEq(attribute,value,rows);	
					result = *(greater_than) | *(equal);					
			   }
			   break;
		   case DOUBLE_VAL:
			   {
				   value *= static_cast<EncodedDoubleAttribute *>(attribute)->Precision();
				   BitStreamInfo * greater_than = UGreaterThan(attribute,value,rows);
				   BitStreamInfo * equal = UEq(attribute,value,rows);	
				   result = *(greater_than) | *(equal);					
			   }
		   }
			return result;
	   }

	   BitStreamInfo * AlgoUtils::ULessThan( EncodedAttributeInfo * attribute, double value,int rows )
	   {
		   
		   BitStreamInfo * less_or_eq = UGreaterThanOrEq(attribute,value,rows);
		   BitStreamInfo * prev = less_or_eq;
		   less_or_eq = ~(*(less_or_eq));
		   delete prev;
		   return less_or_eq;		   
	   }

	   BitStreamInfo * AlgoUtils::ULessThanOrEq(EncodedAttributeInfo * attribute, unsigned long value,int rows)
	   {
		   BitStreamInfo * info = UGreaterThan(attribute,value,rows);
		   BitStreamInfo * prev_info = info;
		   info = ~(*(info));
		   delete prev_info;
		   return info;
	   }

	   BitStreamInfo * AlgoUtils::UGreaterThanInt(EncodedAttributeInfo * attribute,unsigned long input_value,int noOfRows) throw (null_parameter_exception)
	   {
		   if (attribute == NULL)
		   {
			   BOOST_THROW_EXCEPTION(null_parameter_exception());
		   }
		   dynamic_bitset<> bit_set(noOfRows);
		   BitStreamInfo * bit_stream = BitStreamGenerator(attribute,bit_set);
		   dynamic_bitset<> value_pattern((int)attribute->NoOfVBitStreams(),input_value);
			
		   size_t k=0;
		   while (value_pattern[k] == 1 && k < bit_set.size())
			   k=k+1;

		   //Possible memory leak
		   if (k < value_pattern.size())
			   bit_stream = attribute->bitStreamAt(k)->Clone();

		   BitStreamInfo * prev_val = NULL;

		   for (size_t i=k+1; i < value_pattern.size(); i++)
		   {
			   prev_val = bit_stream;
			   if (value_pattern[i] == 1)
			   {
				   bit_stream = *(bit_stream) & *(attribute->bitStreamAt(i));
			   }
			   else 
			   {
				   bit_stream = *(bit_stream) | *(attribute->bitStreamAt(i));
			   }
			   delete prev_val;
		   }


		   return bit_stream;
	 
	   }

	   BitStreamInfo * AlgoUtils::UGreaterThanInt(EncodedAttributeInfo * attribute,unsigned long long input_value,int noOfRows,unsigned long long _max_value) throw (null_parameter_exception)
	   {
		   if (attribute == NULL)
		   {
			   BOOST_THROW_EXCEPTION(null_parameter_exception());
		   }
		   dynamic_bitset<> bit_set(noOfRows);
		   BitStreamInfo * bit_stream = BitStreamGenerator(attribute,bit_set);
		   dynamic_bitset<> value_pattern((int)attribute->NoOfVBitStreams(),input_value);
		   BitStreamInfo * prev_val = NULL;
			
		   if (input_value > _max_value)
			{
				return bit_stream;
			}

		   size_t k=0;
		   while (value_pattern[k] == 1 && k < bit_set.size())
			   k=k+1;

		   if (k < value_pattern.size())
			   bit_stream = attribute->bitStreamAt(k)->Clone();

		   

		   for (size_t i=k+1; i < value_pattern.size(); i++)
		   {
			   prev_val = bit_stream;
			   if (value_pattern[i] == 1)
			   {
				   bit_stream = *(bit_stream) & *(attribute->bitStreamAt(i));
			   }
			   else 
			   {
				   bit_stream = *(bit_stream) | *(attribute->bitStreamAt(i));
			   }
			   delete prev_val;
		   }


		   return bit_stream;

	   }

	   BitStreamInfo * AlgoUtils::ULessThanInt( EncodedAttributeInfo * attribute,unsigned long long input_value,int noOfRows,unsigned long long max_value ) throw(null_parameter_exception)
	   {
		   if (attribute == NULL)
		   {
			   BOOST_THROW_EXCEPTION(null_parameter_exception());
		   }
		   dynamic_bitset<> bit_set(noOfRows);
		   BitStreamInfo * bit_stream = BitStreamGenerator(attribute,bit_set);
		   dynamic_bitset<> value_pattern((int)attribute->NoOfVBitStreams(),input_value);

		   BitStreamInfo * prev_val = NULL;

		   if (input_value > max_value)
		   {
			   prev_val = bit_stream;
			   bit_stream = ~(*bit_stream);
			   delete prev_val;
			   return bit_stream;
		   }

		   size_t k=value_pattern.size() -1 ;
		   while (value_pattern[k] == 0 && k > -1)
		   {
			   k--;
			   prev_val = bit_stream;
			   bit_stream = *(bit_stream) |  *(attribute->bitStreamAt(k));
			   delete prev_val;
		   }
		   if (k > -1 && value_pattern[k] == 1)
		   {
			   prev_val = bit_stream;
			   bit_stream = *(bit_stream) |  *(attribute->bitStreamAt(k));
			   delete prev_val;
			   prev_val = bit_stream;
			   bit_stream = ~(*(bit_stream));
			   delete prev_val;
		   }


		   return bit_stream;

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

	vector<BitStreamInfo *> AlgoUtils::FindDistinctValues( EncodedMultiCatAttribute * _attribute ) throw(null_parameter_exception)
	{		
		// at() is said to be more efficient than iterator. Try to measure this afterwards.		
		if (_attribute == NULL)
		{
			BOOST_THROW_EXCEPTION(null_parameter_exception());
		}
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

	
	double AlgoUtils::USum(EncodedAttributeInfo * attribute)
	{
		if (attribute == NULL)
		{
			BOOST_THROW_EXCEPTION(null_parameter_exception());
		}
		switch(attribute->attributeType())
		{
		case SIGNEDINT_VAL:
		{
		return SumOfInt(attribute);
		break;
		}
		case DOUBLE_VAL:
		{
			EncodedDoubleAttribute * double_att =  static_cast<EncodedDoubleAttribute *>(attribute);
			double double_val = SumOfInt(attribute)/double_att->Precision();

			return double_val;
			break;
		}
		case DATE_VAL:
		{
			BOOST_THROW_EXCEPTION(incompatible_operand_exception());
			return 0;
		}
		case MULTICAT_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		}
		
	}

	double AlgoUtils::USum(EncodedAttributeInfo * attribute, BitStreamInfo * _existence) throw (null_parameter_exception,incompatible_operand_exception)
	{
		switch(attribute->attributeType())
		{
		case SIGNEDINT_VAL:
			{
				return SumOfInt(attribute,_existence);
				break;
			}
		case DOUBLE_VAL:
			{
				EncodedDoubleAttribute * double_att =  static_cast<EncodedDoubleAttribute *>(attribute);
				double double_val = SumOfInt(attribute,_existence)/double_att->Precision();

				return double_val;
				break;
			}
		case DATE_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		case MULTICAT_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		}

	}

	double AlgoUtils::SumSquare(EncodedAttributeInfo * attribute) throw (null_parameter_exception,incompatible_operand_exception)
	{

		if (attribute == NULL)
		{
			BOOST_THROW_EXCEPTION(null_parameter_exception());
		}
		switch(attribute->attributeType())
		{
		case SIGNEDINT_VAL:
			{
				return SumSquareOfInt(attribute);
				break;
			}
		case DOUBLE_VAL:
			{
				EncodedDoubleAttribute * double_att =  static_cast<EncodedDoubleAttribute *>(attribute);
				long precission = double_att->Precision();
				precission *= precission;
				double double_val = SumSquareOfInt(attribute)/precission;

				return double_val;
				break;
			}
		case DATE_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		case MULTICAT_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		}
	}

	double AlgoUtils::Variance( EncodedAttributeInfo * attribute,BitStreamInfo * _existence ) throw (division_by_zero_exception)
	{
		unsigned long count = _existence->Count();
		if (!count)
		{
			BOOST_THROW_EXCEPTION(division_by_zero_exception());
		}
		double sum = USum(attribute,_existence);
		double val = SumSquare(attribute,_existence) - ( sum * sum )/_existence->Count();
		return val/_existence->Count();
	}

	double AlgoUtils::SumSquare(EncodedAttributeInfo * attribute,BitStreamInfo * _existence)throw (null_parameter_exception,incompatible_operand_exception)
	{
		if (attribute == NULL)
		{
			BOOST_THROW_EXCEPTION(null_parameter_exception());
		}

		switch(attribute->attributeType())
		{
		case SIGNEDINT_VAL:
			{
				return SumSquareOfInt(attribute,_existence);
				break;
			}
		case DOUBLE_VAL:
			{
				EncodedDoubleAttribute * double_att =  static_cast<EncodedDoubleAttribute *>(attribute);
				long precission = double_att->Precision();
				precission *= precission;
				double double_val = SumSquareOfInt(attribute,_existence)/precission;

				return double_val;
				break;
			}
		case DATE_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		case MULTICAT_VAL:
			{
				BOOST_THROW_EXCEPTION(incompatible_operand_exception());
				return 0;
			}
		}
	}

	double AlgoUtils::SumSquareOfInt(EncodedAttributeInfo * attribute)
	{
		//for j =num_bits-1 downto 0
		//k=j*2
		double sum = 0;
		int k = 0;
		int l = 0;
		for (int j =  attribute->NoOfVBitStreams() -1; j >=0 ; j--)
		{
			k = j*2;
			sum += pow((double)2,k) * attribute->bitStreamAt(j)->Count();
			l = j-1;
			while (l >= 0)
			{
				BitStreamInfo * result = *(attribute->bitStreamAt(j)) &  *(attribute->bitStreamAt(l));
				sum += pow((double)2,k) * result->Count();
				delete result;
				l=l-1;
				k=k-1;
			}
		}
			return sum;
	}

	double AlgoUtils::SumSquareOfInt(EncodedAttributeInfo * attribute,BitStreamInfo * _existence)
	{
		//for j =num_bits-1 downto 0
		//k=j*2
		double sum = 0;
		int k = 0;
		int l = 0;
		for (int j =  attribute->NoOfVBitStreams() -1; j >=0 ; j--)
		{
			k = j*2;
			sum += pow((double)2,k) * ANDCount(attribute->bitStreamAt(j),_existence);
			l = j-1;
			while (l >= 0)
			{
				BitStreamInfo * result = *(attribute->bitStreamAt(j)) &  *(attribute->bitStreamAt(l));
				sum += pow((double)2,k) * ANDCount(result,_existence);
				delete result;
				l=l-1;
				k=k-1;
			}
		}
		return sum;
	}


	double AlgoUtils::SumOfInt(EncodedAttributeInfo * attribute)
	{
		double val = 0;
		for (int i = 0 ; i < attribute->NoOfVBitStreams(); i++)
		{
			val = val + attribute->bitStreamAt(i)->Count() * pow((double)2,i);
		}
		return val;
	}

	double AlgoUtils::SumOfInt(EncodedAttributeInfo * attribute, BitStreamInfo * _existence)
	{
		double val = 0;
		for (int i = 0 ; i < attribute->NoOfVBitStreams(); i++)
		{
			val = val + ANDCount(attribute->bitStreamAt(i),_existence) * pow((double)2,i);
		}
		return val;
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
	
	
	BitStreamHolder * AlgoUtils::WrapWithHolder(BitStreamInfo *_stream, int _attribute_id, int _bit_map_id)
	{
		BitStreamHolder * result = new BitStreamHolder();
		result->AddAttributeID(_attribute_id);
		result->AddStreamNo(_bit_map_id);
		result->Bit_stream(_stream);
		return result;

	}

	dynamic_bitset<> AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container) throw (algorithm_exception)
	{
		size_t pattern_size = _pattern.size();
		if(pattern_size > 0 && pattern_size != _container.size())
		{
			BOOST_THROW_EXCEPTION(invalid_pattern_exception());
		}
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
