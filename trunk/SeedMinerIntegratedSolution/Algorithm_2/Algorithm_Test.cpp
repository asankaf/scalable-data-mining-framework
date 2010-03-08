#include <iostream>
#include "WrapDataSource.h"
#include "DBConnection.h"
#include "DBQueryExecution.h"

#include <string>
#include <exception>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;

#include "Apriori_Algo.h"
#include "CompressionHandler.h"
using namespace CompressedStructure;
void main()
{
	try{
	int count;
	Apriori_Algo *aa = new Apriori_Algo();

	DBConnection cCon("soyabean","","");
	cCon.initiateConnectionToDB();
	DBQueryExecution *cPtr = new DBQueryExecution("SELECT precip, `plant-growth`, stem	FROM `soybeanTest.csv`");
	cPtr->ExecuteQueryAndBindData(cCon.DBConnectionPtr());
	//const char* statement="SELECT precip FROM `soybeanTest.csv`";
	//DBQueryExecution cExec(statement);
	//cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr());
	WrapDataSource *ds = new WrapDataSource(*cPtr,0);
// 	CompressionHandler handler;
// 	handler.ConvertTo(ds,BitStreamInfo::WAH_COMPRESSION);
	ds->encodeAtrributes();
	//WrapDataSource *wds = new WrapDataSource(cPtr,0);
	//wds->encodeAtrributes();
	//cout<<wds->noOfAttributes();
	cout<<ds->noOfAttributes()<<endl;

	cout << "hello" << endl;
	/*system("pause");*/
/********************************************************************/
	Olap olap;
	string dataSourceId="id";
	vector<int> results;
	double numberOfAttributes = ds->noOfAttributes();
	//cout<<numberOfAttributes<<endl;
	double numberOfCombinations = pow(2,numberOfAttributes);
	//cout<<numberOfCombinations<<endl;

	for (int i=0;i<numberOfCombinations;i++)
	{
		string query="";
		unsigned long combination = (unsigned long)i;
		const boost::dynamic_bitset<> bitString(numberOfAttributes, combination);
		//cout<<bitString<<endl;
		for (int j=0;j<bitString.size();j++)
		{
			stringstream column;
			column << bitString[j];
			string columnPos = column.str();
			if(bitString[j]==0){
				if (query.size()!=0)
				{
					query="!"+dataSourceId+"["+columnPos+",0"+"]&"+query;//get OR val at 0
				}
				else
				{
					query="!"+dataSourceId+"["+columnPos+",0"+"]"+query;//get OR val at 0
				}
				cout<<(*ds)(0,0)->getProcessedBitStream()<<endl;
				//cout<<bitString[j]<<endl;
			}
			else
			{
				if (query.size()!=0)
				{
					query=dataSourceId+"["+columnPos+",0"+"]&"+query;//get OR val at 0
				}
				else
				{
					query=(dataSourceId+"["+columnPos+",0"+"]"+query);//get OR val at 0
				}
				//cout<<bitString[j]<<endl;
			}
		}
		try 
		{
			Query *r = new Query(query);
			size_t count_ = olap.count(r,ds);
			results.push_back(count_);
			cout<<count_<<endl;
			delete r;
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;
		}
		//int numberOfVerticalBitStrings = (*ds)(i)->NoOfVBitStreams();

	}
/********************************************************************/
	//BitStreamInfo* bit_1 = (*ds)(0,0);
	//bit_1->Print();
	delete cPtr;
	delete aa;
	}catch(std::exception &ex){

	}
	/*return 0;*/
}