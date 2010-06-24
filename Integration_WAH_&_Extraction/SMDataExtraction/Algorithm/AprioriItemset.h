#ifndef APRIOIRI_ITEMSET_H
#define APRIOIRI_ITEMSET_H
#include "BitStreamInfo.h"
#include "utils.h"
#include <hash_map>
#include <map>

using namespace stdext;
/************************************************************************
*    Class  :AprioriItemset	  
*    Author :Amila De Silva
*    Subj   :
* Class for storing a set of items. Item sets are stored in a lexicographic
* order, which is determined by the header information of the set of instances
* used for generating the set of items. All methods in this class assume that
* item sets are stored in lexicographic order.
* The class provides methods that are used in the Apriori algorithm to construct
* association rules.
*    Version: 1
************************************************************************/
class AprioriItemset
{
		
public:

	/*
	* Constructor
	*/
	_declspec(dllexport) AprioriItemset(void);

	/***
	* Destructor
	*/
	_declspec(dllexport) ~AprioriItemset(void);

	/***
	* Returns the count of 1s in the bitmap. This represents the
	* number of instances having a particular set of items in common.
	*/
	_declspec(dllexport) int Count() const { return m_count; }

	/***
	* Sets the number of count
	*/
	_declspec(dllexport) void Count(int val) { m_count = val; }

	/***
	* Gives the items stored in the current itemsets. 
	* Missing items are represented by -1.
	*/
	_declspec(dllexport) int * Items() const { return m_items; }

	/***
	* Sets the itemsets array 	
	*/
	_declspec(dllexport) void Items(int * val) { m_items = val; }

	/***
	* Gives the number of attributes stored in this itemsets.
	* This number is equal to the number of elements in the array.
	*/
	_declspec(dllexport) int Attribute_no() const { return m_attribute_no; }

	/***
	* Sets the number of attributes held by this itemset	
	*/
	_declspec(dllexport) void Attribute_no(int val) { m_attribute_no = val; }

	/***
	* Gives the bitstream which represents the itemset.
	* This bitstream store the locations of the instances having
	* a particular itemset.
	*/
	_declspec(dllexport) BitStreamInfo * Bit_stream() const { return m_bit_stream; }

	/***
	* Sets the bitstream held by this itemset
	*/
	_declspec(dllexport) void Bit_stream(BitStreamInfo * val) { m_bit_stream = val; }

	/***
	* Takes a set of itemsets indicated by _itemsets and adds the element to the hashmap
	* indicated by _hash_itemset_vector. This hash map enables accessing itemsets by their
	* hash-values
	*/
	_declspec(dllexport) static hash_map<int,int> GetHashtable(vector<AprioriItemset *> & _itemsets,vector<hash_map<int,AprioriItemset *>> & _hash_itemset_vector);

	/***
	* Gives the hash value for this itemsets.	
	*/
	_declspec(dllexport) int HashCode();

	/***
	* Algorithm to compute hash value for a given integer array of length _no_of_attrib	
	*/
	_declspec(dllexport) inline static int GetHashValue(int _no_of_attrib,int * _int_arr);

	/***
	* Clears the hashmap for previous iteration. This is invoked to save space during algorithm
	* execution. The initial set of itemsets are not deleted by this method.
	*/
	_declspec(dllexport) void ClearPreviousItemsSet(vector<hash_map<int,AprioriItemset *>> & _int_item_set_vector);

	/***
	* Gives the string representation of the itemset	
	*/
	_declspec(dllexport) string ToString();

	/***
	* Deletes the bitstream held by the itemset.	
	*/
	_declspec(dllexport)void ClearBitStream();

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
#endif