// WAHArray.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "WAHStructure.h"
#include "BitsetGenerator.h"
#include "TestWAHArray.h"
#include <vector>
#include <algorithm>
#include <string>
#include "TimeTest.h"
#include "DBQueryExecution.h"
#include "VBitStream.h"
#include "BitStreamInfo.h"
#include "bitstreaminfortest.h"
#include "DBConnection.h"
#include "testcompressionhandler.h"
#include "testalgoutil.h"
#include "testholderoperations.h"
#include "testapriori.h"

using namespace std;
using namespace boost;
using namespace  boost::detail;
using namespace CompressedStructure::TestStructure;
using namespace CompressedStructure;
using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;
void randomBitStreamTester();
void WAHStructureBecnhmarkAND();
void WAHStructureBecnhmarkOR();
void TestDataConnection();


int _tmain(int argc, _TCHAR* argv[])
{
	//TestHolderOperations test;
	//test.TestUniqueVlauesForDataSource();
	//test.TestAttLevelUniquePatternFinding();
	/*TestHolderOperations hl;
	hl.TestHash();*/
//test.TestHolderSuite();
	TestApriori test;
	test.AprioriTestSuite();
	
	return 0;
}

void TestDataConnection()
{
	//Testing the Library for initializing and connecting to the database.
	DBConnection cCon("CPU","","");
	if (cCon.initiateConnectionToDB())
	{
		cout<<"successfully connected"<<endl;
	}

	//Testing the Library for executing query statements and binding data.
	const char* statement="SELECT     `Test3.csv`.* 	FROM         `Test3.csv`";
	DBQueryExecution cExec(statement);
	if ((cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr())))
	{
		cout<<endl<<"Query executed & Fetched successfully"<<endl;
	}

	//Testing the retrieved Integer data.
	vector<PureIntAttInfo*> retrievedIntData = cExec.RetievedIntData();
	int i;
	int j;
	for (i=0 ; i<retrievedIntData.size() ; i++)
	{
		for (j=0 ; j<cExec.RowCount() ; j++)
		{
			cout<<retrievedIntData[i]->ValueList()[j]<<endl;
		}	
		cout<<"Max Value of " << retrievedIntData[i]->attName<< " : " << retrievedIntData[i]->Upper()<<endl; 
		cout<<"Min Value of " << retrievedIntData[i]->attName<< " : " << retrievedIntData[i]->Lower()<<endl;

	}

	//Testing the retrieved Double data.
	/*vector<PureDoubleAttInfo*>  retrievedDoubleData = cExec.RetrievedDoubleData();

	for (i=0 ; i<retrievedDoubleData.size() ; i++)
	{
	for (j=0 ; j<cExec.RowCount() ; j++)
	{
	cout<<retrievedDoubleData[i]->ValueList()[j]<<endl;
	}
	cout<<"Max Value of " << retrievedDoubleData[i]->attName <<" : " << retrievedDoubleData[i]->Upper()<<endl;
	cout<<"Min Value of " << retrievedDoubleData[i]->attName <<" : " << retrievedDoubleData[i]->Lower()<<endl;
	}
	*/
}


void WAHStructureBecnhmarkAND()
{
	TimeTest test;
	cout << "Initial Start Value : "<<endl;
	size_t init = 0;
	size_t end = 0;
	size_t increment = 0;
	cin >>init;
	cout << "Initial End Value : "<<endl;
	cin >> end;
	cout << "Increment Value : "<<endl;
	cin >> increment;
	test.RunAndTest(init,end,increment);
	cout <<"Executed Sucessfully" << endl;
}

void WAHStructureBecnhmarkOR()
{
	TimeTest test;
	cout << "Initial Start Value(OR) : "<<endl;
	size_t init = 0;
	size_t end = 0;
	size_t increment = 0;
	cin >>init;
	cout << "Initial End Value : "<<endl;
	cin >> end;
	cout << "Increment Value : "<<endl;
	cin >> increment;
	test.RunORTest(init,end,increment);
	cout <<"Executed Sucessfully" << endl;
}


void randomBitStreamTester()
{
	TestWAHArray test;
	int i = 0;
	while ( i < 10)
	{
		for (int j = 0; j < 110 ; j+=10)
		{
			cout << "Lendgth : 100 , Percent : "<<j << endl; 
			test.testRandomBitStream(100,1000,j);
			cout << ""<< endl;
		}

		i++;
	}	
}