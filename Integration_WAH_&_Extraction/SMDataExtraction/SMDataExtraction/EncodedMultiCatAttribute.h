#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <algorithm>
#include "VBitStream.h"
#include "AttributeType.h"
#include "EncodedAttributeInfo.h"
#include "StringSpecAssignVals.h"
#include "boost/dynamic_bitset.hpp";

using namespace boost;
using namespace std;

class EncodedMultiCatAttribute : public EncodedAttributeInfo
{
public:
	__declspec(dllexport) EncodedMultiCatAttribute(void);
	__declspec(dllexport) ~EncodedMultiCatAttribute(void);
	__declspec(dllexport) void setMappedValList(vector<dynamic_bitset<>> & _mapped_vals);
	__declspec(dllexport) vector<dynamic_bitset<>> mappedValList();
	__declspec(dllexport) vector<string> uniqueValList();
	__declspec(dllexport) int* getMappedIntVals();
	__declspec(dllexport) int noOfUniqueValues();
	__declspec(dllexport) void setUniqueValList(vector<string> uniques){this->_uniqueValList = uniques;this->_noOfUniqueVals = uniques.size();}
	__declspec(dllexport) string decodeTheTuple(int tupleID);
	__declspec(dllexport) dynamic_bitset<>* mapStringDataToCategories(vector<string> _valueList,std::set<string> _uniqueValList,int noOfRows);
	__declspec(dllexport) dynamic_bitset<>* mapStringDataToCategories(TempStringObjects* _tempStrs,int NoRows,int NoUniqueVals);
	int binarySearch(vector<string> arr, string value, int left, int right);
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
