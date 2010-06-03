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
#include <boost/exception/all.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include "ExceptionReader.h"
#include "Init.h"
using namespace std;
using namespace DBConnectionInfo;
//using namespace DBQueryExecutionInfo;
using namespace CSVConnectionInfo;
using namespace CsvDataExtraction;

void AutomateEncoding(vector<string> & _vect);
void EncodeForAll();


/*
static struct static_block {
static_block() {		
ExceptionReader::BuildFile(string("exception.exp"));		
}
} _STATIC_BLOCK_INSTANCE;*/

int _tmain(int argc, _TCHAR* argv[])
{

	//ExceptionReader::BuildFile(string("exception.exp"));		
	//cout << ExceptionReader::GetError("SM1001")<< endl;
	
	DBConnection cCon("zoo_123","","");
		try{
			cCon.initiateConnectionToDB();
		}
		catch(boost::exception &ex){
			cout<< boost::diagnostic_information(ex)<<endl;
		}
	
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