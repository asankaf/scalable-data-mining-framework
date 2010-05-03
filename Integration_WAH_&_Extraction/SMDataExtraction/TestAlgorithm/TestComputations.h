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
	int VectorGreaterThan(vector<double> & _vect,int _lower_bound);
	int VectorGreaterThanOrEq(vector<double> & _vect,int _lower_bound);
	int VectorLessThanOrEq(vector<double> & _vect,int _upper_bound);
	void TestRangeQueryAccuracy(WrapDataSource * _source);
	void RangeQueryAccuracy();
	void CreateDoubleArray(vector<double> & vect,EncodedAttributeInfo * attr);
	WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);
	int VectorLessThan(vector<double> & _vect,int _upper_bound);
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
