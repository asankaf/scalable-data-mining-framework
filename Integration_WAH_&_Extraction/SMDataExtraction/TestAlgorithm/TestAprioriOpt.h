#pragma once
#include "aprioriopt.h"
#include "algoutils.h"
#include <vector>
#include "aprioriitemset.h"
#include "DBConnection.h"
// #include "DBQueryExecution.h"
#include "WrapDataSource.h"

//using namespace Algorithm;
using namespace DBConnectionInfo;
class TestAprioriOpt
{
public:
	TestAprioriOpt(void);
	~TestAprioriOpt(void);
	void TestSuite();
	void TestUniqueItemSetGeneration(WrapDataSource * _wrapped, AprioriOpt & _ap_opt);
	void TestHashMapBuilding(WrapDataSource * _wrapped, AprioriOpt & _ap_opt);
	void TestUniqueItemsetDeletion(AprioriOpt & _ap_opt, WrapDataSource * ws);
	void TestFindFrequentItemSets(AprioriOpt & _ap_opt,WrapDataSource * ws);
	void TestBuildAssociations(WrapDataSource * _wrapped, AprioriOpt & _ap_opt);
	void SingleExecution(WrapDataSource * ds,vector<double> & _configuation);
	void RunAlgorithm();
	void LoadAndPrintCSV();
	void PerformMemoryTest();
	WrapDataSource * CreateWrappedDataSource();
	void SingleExecution(WrapDataSource * ds);

private:
	string m_apriori_config;
	string m_execution_config;
	
	string m_property_confidence;
	string m_property_support;
	string m_property_rules;
	string m_property_loop_cnt;
	string m_property_file_name;
	string m_property_meta_file_name;
	string m_property_data_file_name;
	string m_property_row_no;
	string m_property_compression;
		
	string m_meta_file_name;
	string m_file_name;
	string m_data_file_name;
	BitStreamInfo::vertical_bit_type m_compression;
	int m_no_of_rows;
	int m_no_of_loops;

	int m_rules;
	double m_confidence;
	double m_min_sup;
	
};
