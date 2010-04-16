#pragma once
#include <cstring>
#include <string>
#include <vector>
#include "boost/dynamic_bitset.hpp"
using namespace std;

class PureAttInfo
{
	
public:
	PureAttInfo(void);
	~PureAttInfo(void);
	int type;
	std::string attName;
	int attID;
	static boost::dynamic_bitset<> existanceBitSet;
	static std::vector<bool> existanceBitMap;
	static int NULLVAL;
};
