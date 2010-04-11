#pragma once
#include "BitStreamInfo.h"

class Itemset
{
public:
	
	_declspec(dllexport) Itemset(void);
	_declspec(dllexport) virtual ~Itemset(void);
	_declspec(dllexport) virtual int Count() const { return m_count; }
	_declspec(dllexport) virtual void Count(int val) { m_count = val; }
	_declspec(dllexport) virtual int * Items() const { return m_items; }
	_declspec(dllexport) virtual void Items(int * val) { m_items = val; }
	_declspec(dllexport) virtual int Attribute_no() const { return m_attribute_no; }
	_declspec(dllexport) virtual void Attribute_no(int val) { m_attribute_no = val; }
	_declspec(dllexport) virtual BitStreamInfo * Bit_stream() const { return m_bit_stream; }
	_declspec(dllexport) virtual void Bit_stream(BitStreamInfo * val) { m_bit_stream = val; }
	//_declspec(dllexport) static hash_map<int,int> GetHashtable(vector<AprioriItemset *> & _itemsets,vector<hash_map<int,AprioriItemset *>> & _hash_itemset_vector);
	_declspec(dllexport) virtual int HashCode();
	
	//_declspec(dllexport) void ClearPreviousItemsSet(vector<hash_map<int,AprioriItemset *>> & _int_item_set_vector);

private :
	int GetHashValue(int _no_of_attrib,int * _int_arr);
	// Keeps the length of the m_items
	int m_attribute_no;	

	//Keeps the count of the rows containing the item combination given by m_items. 
	int m_count;

	// m_items keep the combination of split values.
	int * m_items;	

	//Keeps the result bit stream corresponding to the attribute value combination.
	BitStreamInfo * m_bit_stream;	
};
