#pragma once
#include "WrapDataSource.h"
#include "LoadSavedDataSources.h"
#include "DataSources.h"
#include "csvconnection.h"
#include "extractedCsvDTO.h"
using namespace CSVConnectionInfo;

class TestComputations
{
public:
	TestComputations(void);
	~TestComputations(void);
	void TestSuite();
	void TestAttributeSum(WrapDataSource * source);
	void IntAttributeSum();
	void DoubleAttributeSum();
	void TestRangeQueryInt( WrapDataSource * source );
	void CreateDoubleArray(vector<double> & vect,EncodedAttributeInfo * attr);
	WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);
	double VectorSum(vector<int> & vect);
	double VectorSum(vector<double> & vect);
	void CreateIntArray(vector<int> & vect,EncodedAttributeInfo * attr);
	WrapDataSource * CreateDataSource(string csv_path);
	void TestMemoryLeaks();
	void IntAttributeSumSquare();
	void TestAttributeSumSquare( WrapDataSource * source );
	void IntAttributeRangeQuery();
	WrapDataSource * CreateDataSource();

private:
	DataSources * m_data_sources;
	WrapDataSource * m_wrapped;
	LoadSavedDataSources * m_lsd;

};
