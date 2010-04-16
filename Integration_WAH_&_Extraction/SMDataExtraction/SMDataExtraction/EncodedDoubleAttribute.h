#pragma once
#include "EncodedAttributeInfo.h"
#include <vector>

using namespace std;

class EncodedDoubleAttribute : public EncodedAttributeInfo
{
public:
	__declspec(dllexport) EncodedDoubleAttribute(void);
	__declspec(dllexport) ~EncodedDoubleAttribute(void);
	__declspec(dllexport) vector<bool> SignBitMap() const { return _signBitMap; }
	__declspec(dllexport) void SignBitMap(vector<bool> val) { _signBitMap = val; }
	__declspec(dllexport) double maxAttVal(){return this->_maxVal;}
	__declspec(dllexport) double minAttVal(){return this->_minVal;}
	__declspec(dllexport) void setMaxVal(double val){this->_maxVal = val;}
	__declspec(dllexport) void setMinVal(double val){this->_minVal = val;}
	__declspec(dllexport) long Precision() const { return precision; }
	__declspec(dllexport) void Precision(long val) { precision = val; }
	__declspec(dllexport) double decodeTheTuple(int tupleID,bool asAnInteger=false);
	__declspec(dllexport) int getTheSignOf(int tupleID);
	__declspec(dllexport) dynamic_bitset<> signMapAsInt();
	__declspec(dllexport) void setTheSignBitMap(vector<double> values,int valSet);

private:
	long precision;
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.	
	double _maxVal;
	double _minVal;
};

