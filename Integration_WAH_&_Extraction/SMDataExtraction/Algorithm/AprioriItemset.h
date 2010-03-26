#pragma once
#include "BitStreamInfo.h"
#include <hash_map>
#include <map>

using namespace stdext;
class AprioriItemset
{
		
public:
// 	static struct itemset_comp{
// 		bool operator()(const AprioriItemset * _item1, AprioriItemset * _item2) const
// 		{
// 			return memcmp(_item1->Items(),_item2->Items(),_item1->Attribute_no()) != 0;
// 		}
// 	};
	_declspec(dllexport) AprioriItemset(void);
	_declspec(dllexport) ~AprioriItemset(void);
	_declspec(dllexport) int Count() const { return m_count; }
	_declspec(dllexport) void Count(int val) { m_count = val; }
	_declspec(dllexport) int * Items() const { return m_items; }
	_declspec(dllexport) void Items(int * val) { m_items = val; }
	_declspec(dllexport) int Attribute_no() const { return m_attribute_no; }
	_declspec(dllexport) void Attribute_no(int val) { m_attribute_no = val; }
	_declspec(dllexport) BitStreamInfo * Bit_stream() const { return m_bit_stream; }
	_declspec(dllexport) void Bit_stream(BitStreamInfo * val) { m_bit_stream = val; }
	_declspec(dllexport) static hash_map<int,int> GetHashtable(vector<AprioriItemset *> & _itemsets,vector<hash_map<int,AprioriItemset *>> & _hash_itemset_vector);
	_declspec(dllexport) int HashCode();
	_declspec(dllexport) inline static int GetHashValue(int _no_of_attrib,int * _int_arr);
	_declspec(dllexport) void ClearPreviousItemsSet(vector<hash_map<int,AprioriItemset *>> & _int_item_set_vector);

private :
	// Keeps the length of the m_items
	int m_attribute_no;	
	
	//Keeps the count of the rows containing the item combination given by m_items. 
	int m_count;

	// m_items keep the combination of attribute values.
	int * m_items;	

	//Keeps the result bit stream corresponding to the attribute value combination.
	BitStreamInfo * m_bit_stream;	
};
