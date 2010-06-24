#pragma once
#include "EncodedAttributeInfo.h"
#include <vector>
#include <string>
#include "EncodedMultiCatAttribute.h"
#include "BitStreamInfo.h"
#include "AlgoUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//Implementation of the Range Splitter of the Numerical Attributes
//With the corresponding Numeric attribute, the external entity
//has to provide the range values that has to be considered when
//splitting the numerical attribute values. ranges should be included in a double vector.
//For example: Let's say Min and Max values of the Numerical attribute is 3225 and 12234
//correspondingly. External entity can provide a double vector where elements are included as
//3225,5125,6100,8750,11500,12500. Then the ranges are automatically calculated as
//"3225-5125","5125-6100","6100-8750","8750-11500" and "11500-12500". These ranges will also
//be the unique value list of the corresponding EncodedMulticatAttribute which will be returned
//On successful conversion.
//Important : If the two extremes of the ranges provided are not within the actual bounds, they
//			  will automatically be adjusted to two extremes(i.e. max and min values.)
///////////////////////////////////////////////////////////////////////////////////////////////

class RangeSplitter
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) RangeSplitter(void);
	__declspec(dllexport) RangeSplitter(EncodedAttributeInfo* attribute,long rowCount){this->_attribute=attribute;this->_rowCount=rowCount;}
	__declspec(dllexport) RangeSplitter(EncodedAttributeInfo* attribute,vector<double> rangeVals,long rowCount){this->_attribute = attribute;this->_rangeVals = rangeVals;this->_rowCount = rowCount;this->_maxValAssigned = 0;}
	__declspec(dllexport) RangeSplitter(WrapDataSource* ds){this->_ds = ds;this->_rowCount = ds->noOfRows();this->_attribute = NULL;}
	__declspec(dllexport) ~RangeSplitter(void);

	#pragma endregion Constructors & Destructor

	/**	Split ranges provided in the constructor*/
	__declspec(dllexport) EncodedMultiCatAttribute* SplitRangesInNumericAtts();

	/**	Splitting the ranges here is provided only for a single numeric attribute*/
	__declspec(dllexport) EncodedMultiCatAttribute* SplitRanges();

	/**	Split the ranges into equal number of ranges*/
	__declspec(dllexport) EncodedMultiCatAttribute* SplitIntoEqualRanges(int no_of_ranges);

	/**	Split the ranges in all numeric attribute, into equal number of ranges*/
	__declspec(dllexport) void SplitAllNumericalAttsIntoEualRanges(int no_of_ranges);

	/** Getter and setter for the instance variable _rangeVals*/
	__declspec(dllexport) vector<double> RangeVals() const { return _rangeVals; }
	__declspec(dllexport) void RangeVals(vector<double> val) { _rangeVals = val; }
	
	/** Example illustrating how range queries are used for range splitting*/
	__declspec(dllexport) vector<BitStreamInfo *> Splitter(EncodedAttributeInfo * _attribute,int _rows);

	/**	Convert an integer value to a dyamic_bitset*/
	boost::dynamic_bitset<> convertInt(int val,int no_v_bitstreams);

	/**	Prepare the unique value list from ranges provided*/
	vector<string> prepareUniqueValList(bool isDoubleAtt=false,long precision = 0);


private:
	EncodedAttributeInfo* _attribute;
	WrapDataSource* _ds;
	vector<double> _rangeVals;
	long _rowCount;
	long _maxValAssigned;

};
