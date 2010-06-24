#pragma once

#include "VBitStream.h"
#include "AttributeType.h"
#include "EncodedAttributeInfo.h"
#include "StringSpecAssignVals.h"
#include "boost/dynamic_bitset.hpp";
#include "SEEDMinerExceptions.h"

#include <vector>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <algorithm>

using namespace boost;
using namespace std;

/************************************************************************/
/*    Class  :EncodedMultiCatAttribute.h
/*	  Started:25.02.2010 21:45:12
/*    Updated:22.04.2010 06:13:19
/*    Author :SEEDMiner
/*    Subj   :An Attribute class that is inheriting from the Attribute.h
/*			  class, this particular class stores whatever the specific
/*			  behaviours and attributes in String data type.
/*    Version: 
/************************************************************************/

class EncodedMultiCatAttribute : public EncodedAttributeInfo
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) EncodedMultiCatAttribute(void);
	__declspec(dllexport) ~EncodedMultiCatAttribute(void);

	#pragma endregion Constructors & Destructor

	#pragma region Getters & Setters

	__declspec(dllexport) void setMappedValList(vector<dynamic_bitset<>> & _mapped_vals);
	__declspec(dllexport) vector<dynamic_bitset<>> mappedValList();
	__declspec(dllexport) vector<string> uniqueValList();
	__declspec(dllexport) void setUniqueValList(vector<string> uniques){this->_uniqueValList = uniques;this->_noOfUniqueVals = uniques.size();}
	__declspec(dllexport) int* getMappedIntVals();
	__declspec(dllexport) int noOfUniqueValues();

	#pragma endregion Getters & Setters

	/** Concrete implementation of the Decoding the tuple according to the string attribute type*/
	__declspec(dllexport) string decodeTheTuple(int tupleID) throw(error_vector_out_of_range);

	/** Method to map string data to integer values according to the positioning of those values in*/
	/** the unique value list*/
	__declspec(dllexport) dynamic_bitset<>* mapStringDataToCategories(vector<string> _valueList,std::set<string> _uniqueValList,int noOfRows);

	/** Binary Search Implementation to find a particular value from the unique value list*/
	int binarySearch(vector<string> arr, string value, int left, int right);

	/** Initialization method to instantiate instance variables*/
	void Init();

	/** Keeping unique value list as a map to access more efficiently*/
	void setUniqueMap();

private:
	int* _mappedIntVals;
	vector<dynamic_bitset<>> _mappedValList;
	dynamic_bitset<> *_mappedVals;
	vector<string> _uniqueValList;
	map<string,int> _uniqueValueMap;
	map<string,int>::iterator _it;
	int _noOfUniqueVals;
};
