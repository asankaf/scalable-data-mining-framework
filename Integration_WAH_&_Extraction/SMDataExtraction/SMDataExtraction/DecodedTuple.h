#pragma once

#include "PureIntAttInfo.h"
#include "PureStringAttInfo.h"
#include "PureDoubleAttInfo.h"
#include "PureAttInfo.h"
#include <vector>

class Tuple{
public:
	Tuple();
	~Tuple();
	__declspec(dllexport) vector<PureIntAttInfo*> decodedInts();
	__declspec(dllexport) vector<PureStringAttInfo*> decodedStringAtts();
	__declspec(dllexport) vector<PureDoubleAttInfo*> DecodedDoubleAtts() const { return _decodedDoubleAtts; }
	__declspec(dllexport) void DecodedDoubleAtts(vector<PureDoubleAttInfo*> val) { _decodedDoubleAtts = val; }
	__declspec(dllexport) void setDecodedInts(vector<PureIntAttInfo*> decodedInts);
	__declspec(dllexport) void setDecodedStrings(vector<PureStringAttInfo*> decodedStrings);


private:
	vector<PureIntAttInfo*> _decodedInts;
	vector<PureStringAttInfo*> _decodedStringAtts;
	vector<PureDoubleAttInfo*> _decodedDoubleAtts;	
};