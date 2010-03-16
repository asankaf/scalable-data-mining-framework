#pragma once
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include "VBitStream.h"
#include "AttributeType.h"
#include "EncodedAttributeInfo.h"
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
	__declspec(dllexport) string decodeTheTuple(int tupleID);
	void mapStringDataToCategories(vector<string> _valueList,vector<string> _uniqueValList,int noOfRows);
	int binarySearch(vector<string> arr, string value, int left, int right);

private:
	int* _mappedIntVals;
	vector<dynamic_bitset<>> _mappedValList;
	vector<string> _uniqueValList;
	int _noOfUniqueVals;
};
