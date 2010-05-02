#pragma once
#include "WrapDataSource.h"
#include "naivebayes.h"
#include "extractedCsvDTO.h"
#include "csvconnection.h"
#include "DataSources.h"
#include "LoadSavedDataSources.h"
#include <iostream>

using namespace std;
using namespace CSVConnectionInfo;
class TestNaiveBayes
{
public:
	TestNaiveBayes(void);
	void TestClassifier(WrapDataSource * ds);
	WrapDataSource * CreateDataSource(string csv_path);
	WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);
	void TestSuite();
	~TestNaiveBayes(void);
};
