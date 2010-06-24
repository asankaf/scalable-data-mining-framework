#pragma once
#include "EncodedAttributeInfo.h"
#include "SEEDMinerExceptions.h"

#include <vector>

using namespace std;

/************************************************************************/
/*    Class  :EncodedDoubleAttribute.h
/*	  Started:25.02.2010 21:45:12
/*    Updated:21.04.2010 23:23:09
/*    Author :SEEDMiner
/*    Subj   :An Attribute class that is inheriting from the Attribute.h
/*			  class, this particular class stores whatever the specific
/*			  behaviours and attributes in double data type.
/*    Version: 
/************************************************************************/

class EncodedDoubleAttribute : public EncodedAttributeInfo
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) EncodedDoubleAttribute(void);
	__declspec(dllexport) ~EncodedDoubleAttribute(void);

	#pragma endregion Constructors & Destructor
	
	#pragma region Getters & Setters

	__declspec(dllexport) vector<bool> SignBitMap() const { return _signBitMap; }
	__declspec(dllexport) void setTheSignBitMap(vector<double> values,int valSet) throw(error_vector_out_of_range);
	__declspec(dllexport) dynamic_bitset<> signBitSet() const {return _signBitSet;}
	__declspec(dllexport) void SignBitMap(vector<bool> val) { _signBitMap = val; }
	__declspec(dllexport) void setSignBitSet(dynamic_bitset<> bitSet){this->_signBitSet = bitSet;}
	__declspec(dllexport) double maxAttVal(){return this->_maxVal;}
	__declspec(dllexport) double minAttVal(){return this->_minVal;}
	__declspec(dllexport) void setMaxVal(double val){this->_maxVal = val;}
	__declspec(dllexport) void setMinVal(double val){this->_minVal = val;}
	__declspec(dllexport) long Precision() const { return precision; }
	__declspec(dllexport) void Precision(long val) { precision = val; }

	#pragma endregion Getters & Setters

	/** Concrete implementation of the Decoding the tuple according to the double attribute type*/
	__declspec(dllexport) double decodeTheTuple(int tupleID,bool asAnInteger=false) throw(error_vector_out_of_range);

	/** Method to return the sign of a particular value in the double value set*/
	__declspec(dllexport) int getTheSignOf(int tupleID) throw(error_vector_out_of_range);

	/** Method to return the signbitmap as an integer value*/
	__declspec(dllexport) dynamic_bitset<> signMapAsInt();
	

private:
	long precision;
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.
	dynamic_bitset<> _signBitSet;
	double _maxVal;
	double _minVal;
};

