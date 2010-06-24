#pragma once
#include "boost/dynamic_bitset.hpp"

#include <cstring>
#include <string>
#include <vector>

using namespace std;

/************************************************************************/
/*    Class  :DecodedTuple.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.03.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :DTO Class to hold the common attribute properties
/*    Version: 
/************************************************************************/

class PureAttInfo
{
	
public:
	__declspec(dllexport) PureAttInfo(void);
	__declspec(dllexport) ~PureAttInfo(void);
	int type;
	std::string attName;
	int attID;
	static boost::dynamic_bitset<> existanceBitSet;
	static std::vector<bool> existanceBitMap;
	static int NULLVAL;
};
