#pragma once
#include <cstring>
#include <string>
using namespace std;

class PureAttInfo
{
public:
	PureAttInfo(void);
	~PureAttInfo(void);
	int type;
	std::string attName;
	int attID;
};
