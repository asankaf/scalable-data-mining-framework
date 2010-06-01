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
	
	void TestSuite();
	
	~TestNaiveBayes(void);

private:
	void TestClassifier(WrapDataSource * ds);
	void SingleExecution(WrapDataSource * _ds);
	void TestWekaNaiveBayes(WrapDataSource * _data_source, int _class_index,int _loop);
	void TestSMNaiveBayes(WrapDataSource * _data_source, int _class_index,int _loop);
	void PerformMemoryTest();

	string const m_execution_config;

	string const LOOP_COUNT;
	string const FILE_NAME;
	string const METAFILE_NAME;
	string const DATAFILE_NAME;
	string const ROW_NO;
	string const COMPRESSION;
	string const FOLDER_NAME;
	string const LOAD_TYPE;

	string const LOAD_TYPE_MULTI;
	string const LOAD_TYPE_SINGLE;
	string const LOAD_TYPE_CSV;

	string m_meta_file_name;
	string m_file_name;
	string m_data_file_name;
	string m_folder_name;
	BitStreamInfo::vertical_bit_type m_compression;
	int m_no_of_rows;
	int m_no_of_loops;
};
