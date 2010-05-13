#pragma once
#include "WrapDataSource.h"
#include <xstring>
#include "csvconnection.h"
#include "extractedCsvDTO.h"
#include "LoadSavedDataSources.h"
#include "DataSources.h"
#include "classifiertestsource.h"

using namespace std;
using namespace CSVConnectionInfo;
class TestClassifierTestSource
{
public:
	TestClassifierTestSource(void);
	~TestClassifierTestSource(void);
	void TestSuite();
	void TestBuilding();
	WrapDataSource * CreateDataSource(string csv_path);
	WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);
	
};
