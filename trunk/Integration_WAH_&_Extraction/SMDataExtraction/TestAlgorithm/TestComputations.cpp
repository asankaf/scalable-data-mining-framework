#include "TestComputations.h"
#include "algoutils.h"
#include <fstream>
#include <time.h>
#include <vector>
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include "CompressionHandler.h"

using namespace std;
TestComputations::TestComputations(void)
{
}

TestComputations::~TestComputations(void)
{
}

void TestComputations::TestSuite()
{
//IntAttributeSum();
//IntAttributeSumSquare();
	IntAttributeRangeQuery();
}

WrapDataSource * TestComputations::CreateDataSource(string datafile,string metadFile,string filename)
{
	LoadSavedDataSources *lsd = new LoadSavedDataSources(metadFile,datafile);
	//	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_0.5mil_metadata","soyabean_0.5mil_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	return (*dsLoaded)(filename);
}

void TestComputations::TestAttributeSumSquare( WrapDataSource * source )
{
	CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	EncodedAttributeInfo * inf = (*source)(0);
	clock_t start , end;
	double dd;
	start = clock();
	dd = AlgoUtils::SumSquare(inf);
	end = clock();
	cout << "Value : "<< dd << endl;
	cout << "Time : " << end - start << endl;

	vector<int> vect(source->noOfRows());
	CreateIntArray(vect,inf);
	double sum = 0;
	start = clock();
	for (size_t i = 0 ; i < vect.size() ; i++)
	{
		sum += vect[i] * vect[i];
	}

	end = clock();
	cout << "Sumsquare of vector : " << sum<<endl;
	cout << "Time taken : " << end - start << endl;
}

void TestComputations::TestRangeQueryInt( WrapDataSource * source )
{

	EncodedAttributeInfo * inf = (*source)(0);
	clock_t start , end;
	double dd;
	
	/*
	start = clock();	
		dd = AlgoUtils::UGreaterThan(inf,1500,source->noOfRows())->Count();
		end = clock();
		cout << "Value : "<< (int)dd << endl;
		cout << "Time : " << end - start << endl;
	
		start = clock();
		dd = AlgoUtils::UGreaterThanOrEq(inf,5506,source->noOfRows())->Count();
		end = clock();
		cout << "Value : "<< (int)dd << endl;
		cout << "Time : " << end - start << endl;
	

	start = clock();
	dd = AlgoUtils::ULessThan(inf,5506,source->noOfRows())->Count();
	end = clock();
	cout << "Value : "<< (int)dd << endl;
	cout << "Time : " << end - start << endl;
*/
	start = clock();
	dd = AlgoUtils::ULessThanOrEq(inf,5506,source->noOfRows())->Count();
	end = clock();
	cout << "Value : "<< (int)dd << endl;
	cout << "Time : " << end - start << endl;
	

}

void TestComputations::TestAttributeSum( WrapDataSource * source )
{
	CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	EncodedAttributeInfo * inf = (*source)(0);
	clock_t start , end;
	double dd;
	start = clock();
	dd = AlgoUtils::USum(inf);
	end = clock();
	cout << "Value : "<< (int)dd << endl;
	cout << "Time : " << end - start << endl;

/*
vector<int> vect(source->noOfRows());
CreateIntArray(vect,inf);
int sum = 0;
start = clock();
for (size_t i = 0 ; i < vect.size() ; i++)
{
	sum += vect[i];
}

end = clock();
cout << "Sum of vector : " << sum<<endl;
cout << "Time taken : " << end - start << endl;
*/
	/*
	std::ofstream stream("MF.csv");
		stream << source->generateCSVStringofDecodedData();
		stream.close();*/
	
}

void TestComputations::CreateIntArray(vector<int> & vect,EncodedAttributeInfo * attr)
{
	
vector<BitStreamInfo *> streams1 = attr->vBitStreams();
vector<dynamic_bitset<>> streams(streams1.size());
for (size_t i = 0 ; i < streams.size() ; i++)
{
	streams[i] = streams1[i]->Decompress();
}

for (size_t i = 0 ; i < vect.size() ; i++)
{
	dynamic_bitset<> temp((int)attr->NoOfVBitStreams());
	for (size_t j = 0 ; j < temp.size() ; j++)
	{
		temp[j] = streams[j][i];
	}
	vect[i] = (int)temp.to_ulong();
}

}

void TestComputations::IntAttributeSum()
{
TestAttributeSum(CreateDataSource("intVal_1000000_data","intVal_1000000_metadata","intVal_1000000"));
}

void TestComputations::IntAttributeSumSquare()
{
	TestAttributeSumSquare(CreateDataSource("intVal_100000_data","intVal_100000_metadata","intVal_100000"));
}

void TestComputations::IntAttributeRangeQuery()
{
	TestRangeQueryInt(CreateDataSource("intVal_100000_data","intVal_100000_metadata","intVal_100000"));
}