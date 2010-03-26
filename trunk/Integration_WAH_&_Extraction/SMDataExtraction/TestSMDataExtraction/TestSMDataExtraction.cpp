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
#include <iostream>

using namespace std;
using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;
using namespace CSVConnectionInfo;
using namespace CsvDataExtraction;

int _tmain(int argc, _TCHAR* argv[])
{
	
	CsvConnection cConcsv("C:\\soyaTest-mod1.csv",',','\n','""');
	time_t start_1,end_1;
	start_1 = clock();
	ExtractedCsvDTO *dat = cConcsv.extractData();
	end_1 = clock();
	cout<<"Time to retrieve "<< dat->RowCount() <<" records from CSV file : "<<(end_1 - start_1)<<endl;

	//Testing the Library for initializing and connecting to the database.
// 	DBConnection cCon("SoyaBeanMySql","","");
// 	if (cCon.initiateConnectionToDB())
// 	{
// 		cout<<"successfully connected"<<endl;
// 	}
	
	//Testing the Library for executing query statements and binding data.
	const char* statement="SELECT * from soyabeanms";
	DBQueryExecution cExec(statement);
	start_1 = clock();
// 	if ((cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr())))
// 	{
// 		cout<<endl<<"Query executed & Fetched successfully"<<endl;
// 	}
	end_1 = clock();
	cout<<"Time to retrieve "<<cExec.RowCount()<< " records via ODBC DSN : "<<(end_1 - start_1)<<endl;
	
	//Testing the retrieved Integer data.
// 	vector<PureIntAttInfo*> retrievedIntData = cExec.RetievedIntData();
// 	int i;
// 	int j;
// 	for (i=0 ; i<retrievedIntData.size() ; i++)
// 	{
// 		for (j=0 ; j<cExec.RowCount() ; j++)
// 		{
// 			cout<<retrievedIntData[i]->valList()[j]<<endl;
// 		}	
// 		cout<<"Max Value of " << retrievedIntData[i]->attName<< " : " << retrievedIntData[i]->Upper()<<endl; 
// 		cout<<"Min Value of " << retrievedIntData[i]->attName<< " : " << retrievedIntData[i]->Lower()<<endl;
// 
// 	}
// 	
// 	//Testing the retrieved Double data.
// 	vector<PureDoubleAttInfo*>  retrievedDoubleData = cExec.RetrievedDoubleData();
// 
// 	for (i=0 ; i<retrievedDoubleData.size() ; i++)
// 	{
// 		for (j=0 ; j<cExec.RowCount() ; j++)
// 		{
// 			cout<<retrievedDoubleData[i]->ValueList()[j]<<endl;
// 		}
// 		cout<<"Max Value of " << retrievedDoubleData[i]->attName <<" : " << retrievedDoubleData[i]->Upper()<<endl;
// 		cout<<"Min Value of " << retrievedDoubleData[i]->attName <<" : " << retrievedDoubleData[i]->Lower()<<endl;
// 	}	

	//Testing the retrieved String data.
//	vector<PureStringAttInfo*> retrievedStringData = cExec.RetrievedStringData();

// 	for (i=0 ; i<retrievedStringData.size() ; i++)
// 	{
// 		for (j=0 ; j<cExec.RowCount() ; j++)
// 		{
// 			cout<<retrievedStringData[i]->ValueList().at(j)<<endl;
// 		}
// 
// 		cout<<endl<<"Testing Unique Data Received"<<endl;
// 
// 		//Represents the unique string data.
// 		vector<string> uniqueData = retrievedStringData[i]->uniqueValueList();
// 		for (j=0 ; j<uniqueData.size() ; j++)
// 		{
// 			cout<<uniqueData[j]<<endl;
// 		}
// 	}

	DataSources * dss= new DataSources();	
	WrapDataSource *dsCsv = new WrapDataSource(*dat,0);
	dsCsv->encodeAtrributes();
	WrapDataSource *ds = new WrapDataSource(cExec,1);
	dss->insertDataSources(ds);
	ds->encodeAtrributes();
	cout<<"Attributes Fetched : "<<ds->noOfAttributes()<<endl;
	cout<<"Rows Fetched and encoded : " << ds->noOfRows()<<endl;
	cout<<"No of Unique Data Items : "<<cExec.RetrievedStringData().at(0)->uniqueValueSet().size()<<endl;
	//cout<<"No of Unique Vals : "<<cExec.RetrievedStringData().at(0)->uniqueValueSet().size()<<endl;
	//BitStreamInfo* te = (*dss)(0,0,0);


	//Testing whether a particular attribute can be retrieved given the attribute ID.
	EncodedAttributeInfo* att = (*ds)(0);

	//Testing whether a particular vBitStream can be retrieved given the bitStream ID;
	//In fact there are two ways in doing so which is explaining in the below illustration.
	BitStreamInfo* bit_1 = (*ds)(0,0);
	BitStreamInfo* bit_2 = (*att)(0);

	for (int i = 0 ; i < ds->codedAttributes().size() ; i++)
	{
		switch (ds->codedAttributes()[i]->attributeType())
		{
			//use the attribute type to decide what kind of an abject is wrapped by
			//the higher level EncodedAttributeInfo object pointer.
		case (ATT_TYPE::SIGNEDINT_VAL):
			{
				EncodedIntAttribute *intAtt = static_cast<EncodedIntAttribute*> (ds->codedAttributes()[i]);
				cout<<intAtt->attributeName()<<endl;
				cout<<intAtt->attributeID()<<endl;
				for (int j = 0; j < intAtt->NoOfVBitStreams() ; j++)
				{
					cout<<"Attribute ID: "<<intAtt->vBitStreams()[j]->BitStreamAllocAttID()<<" ";
					cout<<intAtt->vBitStreams()[j]->getProcessedBitStream();
					cout<<endl;
				}

				//Testing the method for retrieving a bit stream on given index.
				//VBitStream* vBitStream = intAtt->bitStreamAt(2);

				BitStreamInfo* vBitStream = (*intAtt)(1);
				cout << vBitStream->getProcessedBitStream()<<endl;

				//Decode a particular tuple upon given a tuple ID.
				cout<<intAtt->decodeTheTuple(4)<<endl;;
				break;
			}

		case (ATT_TYPE::MULTICAT_VAL):
			{
				EncodedMultiCatAttribute* multiCatAtt = static_cast<EncodedMultiCatAttribute*> (ds->codedAttributes()[i]);
				cout<<endl<<endl<<multiCatAtt->attributeName()<<endl;
				cout<<multiCatAtt->attributeID()<<endl;
				cout<<"No Of Unique Data Values : " <<multiCatAtt->noOfUniqueValues()<<endl;

				for (int i = 0 ; i < multiCatAtt->noOfUniqueValues() ; i++)
				{
					cout<< i << " - " << multiCatAtt->uniqueValList()[i]<<endl;
				}

				for (int k = 0 ; k < multiCatAtt->NoOfVBitStreams() ; k++)
				{
					cout<<"Attribute ID: "<<multiCatAtt->vBitStreams()[k]->BitStreamAllocAttID()<<" ";
					cout<<multiCatAtt->vBitStreams()[k]->getProcessedBitStream();
					cout<<endl;
				}

				//Testing the method for retrieving a bit stream on given a bit stream index.
				//For convenience the same duty can be achieved by using parenthesis operator
				// eg: multicat(2)
				//VBitStream* vBitStream = multiCatAtt.bitStreamAt(1);
				BitStreamInfo* vBitStream = (*multiCatAtt)(1);
				cout<<vBitStream->getProcessedBitStream()<<endl;

				//Decode a particular tuple upon given a tuple ID.
				cout<<multiCatAtt->decodeTheTuple(3)<<endl;;
				break;
			}
		}
	}
	//Testing the location of 1's
	cout<<"No of 1's in the bit stream 0 : "<<bit_1->count()<<endl;

	//Testing the places of 1's
	for (int j=0 ; j<bit_1->getActiveBitIDs().size() ; j++)
	{
		cout<<bit_1->getActiveBitIDs()[j]<<" ";
	}
	cout<<endl;

	
	//Testing the decoding an entire tuple ability.
	int tupID = 3;
	Tuple* tuples = ds->DecodeTheTuple(tupID);
	cout<<"\nDecoding the Tuple ID: " << tupID<<endl;
	for (int i=0 ; i<tuples->decodedInts().size() ; i++)
	{
		cout << "AttributeID : " << tuples->decodedInts()[i]->attID << " - value : "<<tuples->decodedInts()[i]->ValueList()[0]<<endl;
	}

	for (int j=0 ; j<tuples->decodedStringAtts().size() ; j++)
	{
		cout << "AttributeID : " << tuples->decodedStringAtts()[j]->attID << " - value : "<<tuples->decodedStringAtts()[j]->ValueList()[0]<<endl;
	}
	
	//Testing the closing connection method.
// 	if (cCon.closeConnectionWithDB())
// 	{
// 		cout<<endl<<"Connection to the database is terminated successfully."<<endl;
// 	}
	
	return 0;
}