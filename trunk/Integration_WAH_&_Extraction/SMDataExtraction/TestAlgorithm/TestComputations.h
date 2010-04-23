#pragma once
#include "WrapDataSource.h"
#include "LoadSavedDataSources.h"
#include "DataSources.h"

class TestComputations
{
public:
	TestComputations(void);
	~TestComputations(void);
	void TestSuite();
	void TestAttributeSum(WrapDataSource * source);
	void IntAttributeSum();
	void TestRangeQueryInt( WrapDataSource * source );
	WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);
	void CreateIntArray(vector<int> & vect,EncodedAttributeInfo * attr);
	void IntAttributeSumSquare();
	void TestAttributeSumSquare( WrapDataSource * source );
	void IntAttributeRangeQuery();

private:


};
