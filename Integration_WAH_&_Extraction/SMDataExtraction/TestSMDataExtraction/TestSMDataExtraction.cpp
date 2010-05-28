// TestSMDataExtraction.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CsvConnection.h"
#include "extractedCsvDTO.h"
#include "DBConnection.h"
#include "DBQueryExecution.h"
#include "WrapDataSource.h"
#include "DecodedTuple.h"
#include "EncodedAttributeInfo.h"
#include "DataSources.h"
#include "SaveDataSources.h"
#include "LoadSavedDataSources.h"
#include <iostream>


using namespace std;
using namespace DBConnectionInfo;
//using namespace DBQueryExecutionInfo;
using namespace CSVConnectionInfo;
using namespace CsvDataExtraction;

void AutomateEncoding(vector<string> & _vect);
void EncodeForAll();

int _tmain(int argc, _TCHAR* argv[])
{
	EncodeForAll();
	
	CsvConnection *csv = new CsvConnection("E:\\University\\Level 4\\CS4200 - Data Mining\\Data Mining\\DataFiles\\dataSets\\small_data.txt",',','\n');
	vector<int> r_order;
	r_order.push_back(0);
	r_order.push_back(2);
	r_order.push_back(1);
	r_order.push_back(0);
	r_order.push_back(0);
	r_order.push_back(1);
	r_order.push_back(2);

	ExtractedCsvDTO *r_csv = csv->readCSV(r_order);
	WrapDataSource *r_ds = new WrapDataSource(r_csv,"small_data");
	r_ds->encodeAtrributes();
	DataSources *r_dss = new DataSources();
	r_dss->insertDataSources(r_ds);
	DataSourceSerialization *source_ser = new DataSourceSerialization(r_dss,"small_metadata","small_data");
	source_ser->serializeDataSource();
	
	//load from multiple files with a limit of 3 rows...
	LoadSavedDataSources *lds = new LoadSavedDataSources(3,"small_data","small_metadata");
	DataSources *dss = lds->loadSavedEncodedDataFromMultipleFiles(true);
	WrapDataSource *wds = (*dss)("small_data");
	cout<<wds->generateCSVStringofDecodedData();
	delete wds;
	
	
	/*
  	time_t start_1,end_1;	

	//Code Snippet to load data from a saved encoded file. 
	//Important : Files should be saved in ../Reports/ folder.
 	//LoadSavedDataSources *lsd = new LoadSavedDataSources("test_Data_Large_metadata","test_Data_Large_data");
 	//DataSources *dsLoaded = lsd->loadSavedEncodedData();
	
	//intval2mill.csv
	//CsvConnection *connection = new CsvConnection("C:\\Data\\doubval.csv",',','\n');
	
	//CsvConnection cConcsv("C:\\Data\\soyaTest.csv",',','\n','""');
	//J:\\pokerHand\\poker-hand-training-true.csv
	//CsvConnection *connection = new CsvConnection("J:\\pokerHand\\poker-hand-testing.csv",',','\n','""');
	//soyabean-10cols-2mill.txt
	//soyabean-10cols-1mill.csv
	//CsvConnection *connection = new CsvConnection("J:\dataSets\soyaTestlarge\column1_date.csv",',','\n','""');
	CsvConnection *connection = new CsvConnection("C:\\Data\\weather.nominal.csv",',','\n','""');
	
	
	//ExtractedCsvDTO *exDTO = connection->readCSV();
	ExtractedCsvDTO *exDTO = connection->extractData();
	WrapDataSource *dsSBN = new WrapDataSource(exDTO,"column1_date_5mil");
	dsSBN->encodeAtrributes();
	//vector<EncodedAttributeInfo*> vals = dsSBN->codedAttributes();
//	delete dsSBN;
	
DataSources *SBDss = new DataSources();
	cout<<"Finished Encoding data"<<endl;
	SBDss->insertDataSources(dsSBN);
	DataSourceSerialization *dss =  new DataSourceSerialization(SBDss,"column1_date_5mil_metadata","column1_date_5mil_data");
	dss->serializeDataSource();
delete connection;
//delete exDTO;
delete dsSBN;
delete SBDss;
delete dss;
*/
	return 0;
}

void AutomateEncoding(vector<string> & _vect)
{
	CsvConnection *connection = new CsvConnection(_vect[0].data(),',','\n','""');
	
	ExtractedCsvDTO *exDTO = connection->extractData();
	WrapDataSource *dsSBN = new WrapDataSource(exDTO,_vect[1]);
	dsSBN->encodeAtrributes();
	
	DataSources *SBDss = new DataSources();
	cout<<"Finished Encoding data"<<endl;
	SBDss->insertDataSources(dsSBN);
	DataSourceSerialization *dss =  new DataSourceSerialization(SBDss,_vect[1]+"metadata",_vect[1]+"data");
	dss->serializeDataSource();

	cout << dsSBN->generateCSVStringofDecodedData(100) << endl;
	delete connection;
	delete dsSBN;
	delete SBDss;
	delete dss;
}

void EncodeForAll()
{
vector<vector<string>> vect;
/*

vector<string> vect1(2);
vect1[0] = "J:\\dataSets\\soyaTestlarge\\column1_date.csv";
vect1[1] = "column1_date_5mil";

vector<string> vect2(2);
string ss;*/
ifstream infile("test.txt");
while(!infile.eof()) // To get you all the lines.
{
	vector<string> temp_vect(2);
	string temp_string;
	getline(infile,temp_string);
	temp_vect[0] = temp_string;
	
	getline(infile,temp_string);
	temp_vect[1] = temp_string;
	vect.push_back(temp_vect);
}
for (size_t i = 0 ; i < vect.size();i++)
{
	//cout << vect[i][0] <<" , " <<vect[i][1]<<endl;
	cout <<"Starting to encode file "  << i+1 << endl;
	AutomateEncoding(vect[i]);
	cout << endl;
}

}