#pragma once

#include "PureIntAttInfo.h"
#include "PureStringAttInfo.h"
#include "PureDoubleAttInfo.h"
#include "PureAttInfo.h"
#include <vector>

/************************************************************************/
/*    Class  :DecodedTuple.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.03.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :DTO Class to hold the decoded raw data
/*    Version: 
/************************************************************************/

class Tuple{
public:

	#pragma region Constructors & Destructor
	
	__declspec(dllexport) Tuple();
	__declspec(dllexport) ~Tuple();

	#pragma endregion Constructors & Destructor

	#pragma region Getters & Setters

	__declspec(dllexport) vector<PureIntAttInfo*> decodedInts();
	__declspec(dllexport) vector<PureStringAttInfo*> decodedStringAtts();
	__declspec(dllexport) vector<PureDoubleAttInfo*> DecodedDoubleAtts() const { return _decodedDoubleAtts; }
	__declspec(dllexport) void DecodedDoubleAtts(vector<PureDoubleAttInfo*> val) { _decodedDoubleAtts = val; }
	__declspec(dllexport) void setDecodedInts(vector<PureIntAttInfo*> decodedInts);
	__declspec(dllexport) void setDecodedStrings(vector<PureStringAttInfo*> decodedStrings);

	#pragma endregion Getters & Setters


private:
	vector<PureIntAttInfo*> _decodedInts;
	vector<PureStringAttInfo*> _decodedStringAtts;
	vector<PureDoubleAttInfo*> _decodedDoubleAtts;	
};