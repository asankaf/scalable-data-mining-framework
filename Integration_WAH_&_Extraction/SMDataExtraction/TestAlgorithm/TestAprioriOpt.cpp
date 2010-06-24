#include "StdAfx.h"
#include "TestAprioriOpt.h"
#include "DataSourceGenerator.h"
#include "csvconnection.h"
#include "CompressionHandler.h"
#include "DataSources.h"
#include "LoadSavedDataSources.h"
#include "DecodedTuple.h"
#include "utils.h"
#include <winbase.h>
#include "pthread.h"
#include <stdlib.h>
#include <time.inl>
#include <fstream>
#include <string>
#include <vector>
#include <xstring>
#include "boost/tokenizer.hpp"
#include "commons.h"

using namespace CSVConnectionInfo;
TestAprioriOpt::TestAprioriOpt(void):m_apriori_config("apriori.cfg"),m_execution_config("algorithm.cfg"),m_property_confidence("confidence"),m_property_rules("rules"),m_property_support("support")
{
	m_property_loop_cnt = "loops";
	m_property_compression = "compression";
	m_property_meta_file_name = "metafile";
	m_property_data_file_name = "datafile";
	m_property_file_name = "filename";


	Commons::BuildFile(m_apriori_config);
	Commons::BuildFile(m_execution_config);
	m_no_of_loops = boost::lexical_cast<int>(Commons::ReadConfiguration(m_property_loop_cnt));
	if (Commons::ReadConfiguration(m_property_compression) == "NO")
	{
		m_compression = BitStreamInfo::VERTICAL_STREAM_FORMAT;
	} else if (Commons::ReadConfiguration(m_property_compression) == "EWAH")
	{
		m_compression = BitStreamInfo::EWAH_COMPRESSION;
	}
	else if (Commons::ReadConfiguration(m_property_compression) == "WAH")
	{
		m_compression = BitStreamInfo::WAH_COMPRESSION;
	}
	m_confidence = lexical_cast<double>(Commons::ReadConfiguration(m_property_confidence));
	m_min_sup= lexical_cast<double>(Commons::ReadConfiguration(m_property_support));
	m_rules = lexical_cast<int>(Commons::ReadConfiguration(m_property_rules));

	m_meta_file_name = Commons::ReadConfiguration(m_property_meta_file_name);
	m_data_file_name = Commons::ReadConfiguration(m_property_data_file_name);
	m_file_name = Commons::ReadConfiguration(m_property_file_name);
}

TestAprioriOpt::~TestAprioriOpt(void)
{
}


void TestAprioriOpt::TestSuite()
{
	RunAlgorithm();		
	//PerformMemoryTest();
	//cout << "Deleted Apriori " << clock() << endl;
}

void TestAprioriOpt::RunAlgorithm()
{
	clock_t comp_start,comp_end;
	cout << "Starting to load data : " << endl;

	//Creating the data source
	comp_start = clock();
	WrapDataSource * ds = Utils::CreateDataSource(m_data_file_name,m_meta_file_name,m_file_name);
	//WrapDataSource * ds = Utils::CreateDataSource("C:\\Data\\soyaTestsort.csv");
	comp_end = clock();
	cout << "Time to load data : " << (comp_end -comp_start) << endl;
	
	AlgoUtils utils;
	//Measuring Space Utilisation
	cout << "Before Compression : " << ds->SpaceUtilsation() << endl;
	CompressionHandler comp;		
	comp_start = clock();
	comp.ConvertTo(ds,m_compression);
	comp_end = clock();
	cout << "After Compression : " << ds->SpaceUtilsation() << endl;
	cout << "Compression Time : " << comp_end - comp_start << endl;

	cout << "With Compression " << endl;
	cout << "Time Spent : "; 
	for (size_t i = 0 ; i < m_no_of_loops; i++)
	{
		AprioriOpt opt;	
		clock_t begin,end;
		begin = clock();
		TestBuildAssociations(ds,opt);
		cout << opt.ToString() << endl;
		end = clock();
		cout<< (end - begin) << " ";
	}
	cout << endl;
	delete ds;
}

void TestAprioriOpt::TestUniqueItemSetGeneration(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
{
	cout << "Start Creating unique item sets" << endl;
	_ap_opt.FindUniqueItemSets(_wrapped);
}

void TestAprioriOpt::TestBuildAssociations(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
{
	_ap_opt.Confidence(m_confidence);
	_ap_opt.NumRules(m_rules);
	_ap_opt.MinSupport(m_min_sup);
	_ap_opt.BuildAssociations(_wrapped);
}

void TestAprioriOpt::TestFindFrequentItemSets(AprioriOpt & _ap_opt,WrapDataSource * ws)
{
	cout << "Test frequent Item set" << endl;
	_ap_opt.MinSupport(.8);
	_ap_opt.FindLargeItemSets();
	cout << "Printing Large Item sets " << endl;
	AlgoUtils utils;
	vector<vector<AprioriItemset *>> items = _ap_opt.LargeItemSets();
	for (size_t i = 0; i < items.size() ; i++)
	{
		cout << "Printing Item set : " << i << endl;
		utils.PrintAprioriItemSets(items.at(i),ws);
	}
	
}

void TestAprioriOpt::TestHashMapBuilding(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
{
	// Unique itemset is taken and its hashtable is constructed
	cout << "Creating Hash Map" << endl;
	AprioriItemset items;
	AlgoUtils utils;
	vector<hash_map<int,AprioriItemset *>> test;
	utils.PrintHashMap(items.GetHashtable(_ap_opt.UniqueItems(),test));
}

void TestAprioriOpt::TestUniqueItemsetDeletion(AprioriOpt & _ap_opt, WrapDataSource * ws){
	vector<AprioriItemset *> item_sets = _ap_opt.UniqueItems();
	item_sets = _ap_opt.DeleteItemSets(item_sets,200,257,false);
	AlgoUtils utils;
	cout << "Result Itemsets " << endl;
	utils.PrintAprioriItemSets(item_sets,ws);
	cout << endl;
	cout << "Original Itemsets" << endl;
	utils.PrintAprioriItemSets(_ap_opt.UniqueItems());
}

void TestAprioriOpt::LoadAndPrintCSV()
{
	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_0.25mil_metadata","soyabean_0.25mil_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	WrapDataSource * ds =  (*dsLoaded)("soyabean");
	cout <<ds->generateCSVStringofDecodedData() << endl;
}

void TestAprioriOpt::PerformMemoryTest()
{
	cout << "Algorithm : Apriori" << endl;
	DWORD process_id = GetCurrentProcessId();
	int ii =0 ;	
	cout << "Current Process ID : " << process_id<<endl;
	cin>>ii;
	cout << "Start Time :" << time(NULL)<< endl;
	
	WrapDataSource * ds = Utils::CreateDataSource(m_data_file_name,m_meta_file_name,m_file_name);	
	cout <<"Finished Creating Data Source : "  << time(NULL)<< endl;	
	CompressionHandler::ConvertTo(ds,m_compression);
	cout <<"Starting To Execute Algorithm : " << time(NULL)<< endl;
	//cin >>ii;
	for (size_t i = 0 ; i < m_no_of_loops ; i++)
	{
		SingleExecution(ds);
	}	
	cout <<"Finished Executing Algorithm : " << time(NULL) << endl;
	//cin>>ii;
	delete ds;
	cout <<"Deleted Data Source: " << time(NULL) << endl;
	//cin>>ii;
}
void TestAprioriOpt::SingleExecution(WrapDataSource * ds)
{
	AprioriOpt apriori;
	TestBuildAssociations(ds,apriori);
}


WrapDataSource * TestAprioriOpt::CreateWrappedDataSource()
{
	DBConnection cCon("SOY","","");
	cCon.initiateConnectionToDB();
	char* command = "SELECT  * FROM soyabeantest ";

	DBQueryExecution * cExec = new DBQueryExecution(command);
	cout << "Loaded Data" << endl;
	cExec->ExecuteQueryAndBindData(cCon.DBConnectionPtr());

	WrapDataSource *ds = new WrapDataSource(cExec,0);	
	ds->encodeAtrributes();
	return ds;
}
