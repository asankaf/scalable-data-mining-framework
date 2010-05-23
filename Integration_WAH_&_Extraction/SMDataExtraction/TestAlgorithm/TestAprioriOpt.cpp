#include "StdAfx.h"
#include "TestAprioriOpt.h"
#include "DataSourceGenerator.h"
#include "csvconnection.h"
#include "CompressionHandler.h"
#include "DataSources.h"
#include "LoadSavedDataSources.h"
#include "DecodedTuple.h"
#include "utils.h"

using namespace CSVConnectionInfo;
TestAprioriOpt::TestAprioriOpt(void)
{
	
}

TestAprioriOpt::~TestAprioriOpt(void)
{
}
void TestAprioriOpt::TestSuite()
{
	clock_t comp_start,comp_end;
	cout << "Starting to load data : " << endl;
	comp_start = clock();
	
	WrapDataSource * ds = Utils::CreateDataSource("poker_hand_data","poker_hand_metadata","poker_hand");
	
		comp_end = clock();
		cout << "Time to load data : " << (comp_end -comp_start) << endl;
		AlgoUtils utils;	
		cout << "Finished Loading Data "<< endl;

		/*
		cout << "Without Compression " << endl;
				//	clock_t comp_start,comp_end;
				cout << endl;
				cout << "Time Spent : " ;
				for (size_t i = 0 ; i < 5; i++)
				{
					AprioriOpt opt;	
					clock_t begin,end;
					begin = clock();
					TestBuildAssociations(ds,opt);
					end = clock();
					cout << (end - begin) << " ";
				}
				cout << endl;	*/
		
		cout << "Befor Comp : " << ds->SpaceUtilsation() << endl;
		CompressionHandler comp;		
		comp_start = clock();
		comp.ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
		comp_end = clock();
		cout << "After Compression : " << ds->SpaceUtilsation() << endl;
		cout << "Compression Time : " << comp_end - comp_start << endl;
		cout << "With Compression " << endl;
		cout << "Time Spent : "; 
		for (size_t i = 0 ; i < 5; i++)
		{
			AprioriOpt opt;	
			clock_t begin,end;
			begin = clock();
			TestBuildAssociations(ds,opt);
			end = clock();
			 cout<< (end - begin) << " ";
		}
		cout << endl;
		
		
		//delete cConcsv;
	// 	delete lsd;
	// 	delete dsLoaded;
	//	delete ds;
		//TestAprioriAlgo();
/*
		CsvConnection cConcsv("C:\\Data\\soyaTest.csv",',','\n','""');	
		//CsvConnection cConcsv(path.c_str(),',','\n','""');
		ExtractedCsvDTO *dat = cConcsv.extractData();
		WrapDataSource *ds = new WrapDataSource(*dat,"0");	
		ds->encodeAtrributes();
	
		//Setting confidence, minimum support  
		AprioriOpt apriori;
		apriori.Confidence(0.9);
		apriori.MinSupport(0.01);
		apriori.NumRules(20);
	
		//Starting to run the algorithm
		apriori.BuildAssociations(ds);*/
	
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
	cout << "Start Creating unique item sets" << endl;
	_ap_opt.FindUniqueItemSets(_wrapped);
// 	cout << "Finished Creating unique item sets" << endl;
// 	vector<AprioriItemset *> items = _ap_opt.UniqueItems();
// 	AlgoUtils utils;
// 	cout << "Start printing Unique Item set"  << endl;
// 	utils.PrintAprioriItemSets(_ap_opt.UniqueItems(),_wrapped);
}

void TestAprioriOpt::TestBuildAssociations(WrapDataSource * _wrapped, AprioriOpt & _ap_opt)
{
	//cout << "Starting to Build Associations " << endl;
	_ap_opt.Confidence(0.9);
	_ap_opt.NumRules(10);
	_ap_opt.MinSupport(0.5);
	_ap_opt.BuildAssociations(_wrapped);
//	AlgoUtils::PrintRules(_ap_opt.Rules());
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

// 	CsvConnection cConcsv("C:\\Data\\soyaTest.csv",',','\n','""');	
// 	ExtractedCsvDTO *dat = cConcsv.extractData();
//	WrapDataSource *ds = new WrapDataSource(*dat,"0");	
//	ds->encodeAtrributes();
	cout <<ds->generateCSVStringofDecodedData() << endl;
	//cout << t->decodedStringAtts()[0]<< endl;
}

WrapDataSource * TestAprioriOpt::CreateWrappedDataSource()
{
	DBConnection cCon("SOY","","");
	cCon.initiateConnectionToDB();
	char* command = "SELECT  * FROM soyabeantest ";
	//char* command = "SELECT  precip,date,class FROM soyabeantest LIMIT 100";
	DBQueryExecution * cExec = new DBQueryExecution(command);
	cout << "Loaded Data" << endl;
	cExec->ExecuteQueryAndBindData(cCon.DBConnectionPtr());

	WrapDataSource *ds = new WrapDataSource(cExec,0);	
ds->encodeAtrributes();
	return ds;
}
