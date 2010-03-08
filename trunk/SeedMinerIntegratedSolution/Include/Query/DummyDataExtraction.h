#ifndef DATAEX
#define DATAEX

#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Operand.h"

using namespace std;

typedef boost::dynamic_bitset<> bitstring;

class DataExtraction
{
public:
	DataExtraction();
	~DataExtraction();

	bitstring getBitString(string dataSourceID,int column,int bit);

	bitstring getBitString(Operand* op);

private:
	vector<bitstring> data;
};
#endif
