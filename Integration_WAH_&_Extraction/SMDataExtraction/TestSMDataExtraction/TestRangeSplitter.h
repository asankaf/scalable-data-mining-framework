#pragma once
#include "EncodedAttributeInfo.h"
#include <vector>
#include "WrapDataSource.h"

class TestRangeSplitter
{
public:
	TestRangeSplitter(void);
	~TestRangeSplitter(void);
	void TestSuite();
	void CreateIntArray(vector<int> & vect,EncodedAttributeInfo * attr);
	WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);
	void TestUniqueBitmapCreation(WrapDataSource * _source);
	void UniqueBitmapCreation();
};
