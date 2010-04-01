#include "StdAfx.h"
#include "TestAprioriOpt.h"
#include "DataSourceGenerator.h"
#include "csvconnection.h"
#include "CompressionHandler.h"
#include "DataSources.h"
#include "LoadSavedDataSources.h"

using namespace CSVConnectionInfo;
TestAprioriOpt::TestAprioriOpt(void)
{
	
}

TestAprioriOpt::~TestAprioriOpt(void)
{
}
void TestAprioriOpt::TestSuite()
{
	// Testing unique itemset generation
	
	// Creating a wrapped data source
cout <<"Start Loading data" << endl;
	
cout <<"Finished Creating Data Source" << endl;	
//	CsvConnection cConcsv("C:\\soybeanTest4.csv",',','\n','""');	
//CsvConnection cConcsv("C:\\soyaTest-mod1.csv",',','\n','""');	
//	ExtractedCsvDTO *dat = cConcsv.extractData();
	
	cout << "Loaded Data" << endl;
//	cout << "No of Records : "<< dat->RowCount() << endl;
	

	//WrapDataSource *ds = new WrapDataSource(*dat,0);

	//ds->encodeAtrributes();
	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_metadata","soyabean_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	WrapDataSource * ds =  (*dsLoaded)("soyabean");
	cout << "Testing without Compression" << endl;
	
	
	//comp.ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);

	AprioriOpt opt;	
	clock_t begin,end;
	begin = clock();
	TestBuildAssociations(ds,opt);
	end = clock();
	cout << "Time Spent : " << (end - begin) << endl;
	cout << "No off cycles : " << opt.Cycles() << endl;
	opt.Clear();
// 	LoadSavedDataSources *lsd2 = new LoadSavedDataSources("soyabean_metadata","soyabean_data");
// 	DataSources *dsLoaded2 = lsd2->loadSavedEncodedData();	
// 	WrapDataSource * ds2 =  (*dsLoaded2)("soyabean");
// 
 	CompressionHandler comp;
	//comp.ConvertTo(ds2,BitStreamInfo::EWAH_COMPRESSION);
	comp.ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
	cout << endl;
	cout << "Testing with compression " << endl;
	begin = clock();
	AprioriOpt opt2;
	TestBuildAssociations(ds,opt2);
	end = clock();
	cout << "Time Spent : " << (end - begin) << endl;
}

// WrapDataSource * TestAlgoUtil::LoadData(){
// 	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_metadata","soyabean_data");
// 	DataSources *dsLoaded = lsd->loadSavedEncodedData();
// 	WrapDataSource * ds =  (*dsLoaded)("soyabean");
// 	return ds;
// }

// void TestAprioriOpt::TestBuildAssociations(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
// {
// 	cout << "Build Associations : " << endl;
// 	_ap_opt.BuildAssociations(_wrapped);
// 	cout << "Printing Large Item sets " << endl;
// 	AlgoUtils utils;
// 	utils.PrintAprioriItemSets(_ap_opt.LargeItemSets(),_wrapped);
// }

void TestAprioriOpt::TestUniqueItemSetGeneration(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
{
	cout << "Start Creating unique itemsets" << endl;
	_ap_opt.FindUniqueItemSets(_wrapped);
	cout << "Finished Creating unique itemsets" << endl;
	vector<AprioriItemset *> items = _ap_opt.UniqueItems();
	AlgoUtils utils;
	cout << "Start printing Unique Itemset"  << endl;
	utils.PrintAprioriItemSets(_ap_opt.UniqueItems(),_wrapped);
}

void TestAprioriOpt::TestBuildAssociations(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
{
	cout << "Starting to Build Associations " << endl;
	_ap_opt.Confidence(0.5);
	_ap_opt.BuildAssociations(_wrapped);
	AlgoUtils::PrintRules(_ap_opt.Rules());
}

void TestAprioriOpt::TestFindFrequentItemSets(AprioriOpt & _ap_opt,WrapDataSource * ws)
{
	cout << "Test frequent Itemset" << endl;
	_ap_opt.MinimumSupport(.8);
	_ap_opt.FindLargeItemSets();
	cout << "Printing Large Itemsets " << endl;
	AlgoUtils utils;
	vector<vector<AprioriItemset *>> items = _ap_opt.LargeItemSets();
	for (size_t i = 0; i < items.size() ; i++)
	{
		cout << "Printing Itemset : " << i << endl;
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



WrapDataSource * TestAprioriOpt::CreateWrappedDataSource()
{
	DBConnection cCon("SOY","","");
	cCon.initiateConnectionToDB();
	char* command = "SELECT  * FROM soyabeantest ";
	//char* command = "SELECT  precip,date,class FROM soyabeantest LIMIT 100";
	DBQueryExecution cExec(command);
	cout << "Loaded Data" << endl;
	cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr());

	WrapDataSource *ds = new WrapDataSource(cExec,0);	
ds->encodeAtrributes();
	return ds;
}
