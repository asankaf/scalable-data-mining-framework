#pragma once

#include "PureIntAttInfo.h"
#include "PureStringAttInfo.h"
#include "PureAttInfo.h"
#include <vector>
#include <xstring>

class Tuple{
public:
	Tuple();
	~Tuple();
	__declspec(dllexport) vector<PureIntAttInfo*> decodedInts();
	__declspec(dllexport) vector<PureStringAttInfo*> decodedStringAtts();
	__declspec(dllexport) void setDecodedInts(vector<PureIntAttInfo*> decodedInts);
	__declspec(dllexport) void setDecodedStrings(vector<PureStringAttInfo*> decodedStrings);
	
private:
	vector<PureIntAttInfo*> _decodedInts;
	vector<PureStringAttInfo*> _decodedStringAtts;
};