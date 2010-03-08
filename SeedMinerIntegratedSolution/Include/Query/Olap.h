#ifndef OLAP
#define OLAP

#include <stack>
#include <iostream>

#include "DummyDataExtraction.h"
#include "Query.h"
#include "Expression.h"
#include "Symbol.h"
#include "WrapDataSource.h"
#include "BitStreamInfo.h"

using namespace std;

class Olap
{
public:
	Olap();
	~Olap();
	int count(Query* query,WrapDataSource* dataExtractor);
};
#endif