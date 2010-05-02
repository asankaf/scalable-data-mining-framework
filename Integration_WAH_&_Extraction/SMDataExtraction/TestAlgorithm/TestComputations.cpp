#include "TestComputations.h"
#include "AlgoUtils.h"
#include <fstream>
#include <time.h>
#include <vector>
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include "CompressionHandler.h"
#include "commons.h"


using namespace std;
TestComputations::TestComputations(void)
{
}

TestComputations::~TestComputations(void)
{
}

void TestComputations::TestSuite()
{
IntAttributeSum();
//IntAttributeSumSquare();
//IntAttributeRangeQuery();
//DoubleAttributeSum();
//	TestMemoryLeaks();
}

WrapDataSource * TestComputations::CreateDataSource(string datafile,string metadFile,string filename)
{
	m_lsd = new LoadSavedDataSources(metadFile,datafile);
	m_data_sources = m_lsd->loadSavedEncodedData();
	return (*m_data_sources)(filename);
}

void TestComputations::TestAttributeSumSquare( WrapDataSource * source )
{
//	CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	cout << "Uncompressed Performances : " << endl;
	EncodedAttributeInfo * inf = (*source)(0);
	clock_t start , end;
	double dd;
	for (size_t i = 0 ; i < 5 ; i++)
	{
		start = clock();
		dd = AlgoUtils::SumSquare(inf);
		end = clock();
		cout << "Value : "<< dd << endl;
		cout << "Time : " << end - start << endl;
	}
	cout << "\nNormal Algorithm :" << endl;
	vector<double> vect(source->noOfRows());
	CreateDoubleArray(vect,inf);

	for (size_t i = 0 ; i < 5 ; i++)
	{
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
	cout << "Compressed Performances" << endl;
	CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	inf = (*source)(0);	
	for (size_t i = 0 ; i < 5 ; i++)
	{
		start = clock();
		dd = AlgoUtils::SumSquare(inf);
		end = clock();
		cout << "Value : "<< dd << endl;
		cout << "Time : " << end - start << endl;
	}
	
	
}

void TestComputations::TestRangeQueryInt( WrapDataSource * source )
{

	EncodedAttributeInfo * inf = (*source)(0);
	clock_t start , end;
	double dd;
	
	for (size_t i = 0 ; i < 5 ;i++)
	{

		start = clock();	
		dd = AlgoUtils::UGreaterThan(inf,1500,source->noOfRows())->Count();
		end = clock();
		cout << "Value : "<< (int)dd << endl;
		cout << "Time : " << end - start << endl;

	}
	start = clock();
	vector<double> vect(source->noOfRows());
	CreateDoubleArray(vect,inf);

	
	cout << "\nNaive Algo : " << endl;
	vector<bool> vv(vect.size());
	for (size_t i = 0 ; i < 5 ; i++)
	{
		dd = 0 ;
		start = clock();
		for (size_t j = 0 ; j < vect.size() ; j++)
		{
			if (vect[j] > 1500)
			{
				vv[j] = 1;
				dd++;
			}
		}
		end = clock();
		cout << "Value : "<< (int)dd << endl;
		cout << "Time : " << end - start << endl;

	}
	cout << "Compressed : " << endl;
	CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	inf = (*source)(0);

	for (size_t i = 0 ; i < 5 ;i++)
	{

		start = clock();	
		dd = AlgoUtils::UGreaterThan(inf,1500,source->noOfRows())->Count();
		end = clock();
		cout << "Value : "<< (int)dd << endl;
		cout << "Time : " << end - start << endl;

	}
	/*
	end = clock();
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

	start = clock();
	dd = AlgoUtils::ULessThanOrEq(inf,5506,source->noOfRows())->Count();
	end = clock();
	cout << "Value : "<< (int)dd << endl;
	cout << "Time : " << end - start << endl;
	
*/
}

void TestComputations::TestAttributeSum( WrapDataSource * source )
{
	//CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	cout << "Attribtue SUM : " << endl;
	cout <<"\tNormal Structure : "<< endl;
	EncodedAttributeInfo * inf = (*source)(0);
	clock_t start , end;
	double dd;
	cout << "\tTime : " ;
	for (size_t i = 0 ; i < 5 ;i++)
	{
	start = clock();
	dd = AlgoUtils::USum(inf);
	end = clock();
	//cout << "\tValue : "<< (int)dd << endl;
	//cout << "\tValue : "<< dd << endl;
	cout << end - start << " ";
	}	
	cout << endl;
	cout <<"\tNaive Alogrithm : " << endl;
	vector<double> vect(source->noOfRows());
		CreateDoubleArray(vect,inf);
		cout << "\tTIme : " ;
		for (size_t i = 0 ; i < 5 ;i++)
		{
		start = clock();
		dd = VectorSum(vect);
		end = clock();
		cout << end - start << " ";
		}
		cout << endl;



		CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	
									BitStreamInfo * inf2 = new VBitStream();
												dynamic_bitset<> test(23,34ul);
												inf2->convert(test);		
												delete inf2;
						
		
inf = (*source)(0);
		cout << "\tTime :";
		for (size_t i = 0 ; i < 5 ;i++)
		{
			start = clock();
			dd = AlgoUtils::USum(inf);
			end = clock();
			cout << end - start << " ";
		}

		
		
cout << "Attribute SUM finished " << endl;
cout << "===============================" << endl;
CompressionHandler::ConvertTo(source,BitStreamInfo::VERTICAL_STREAM_FORMAT);
/*
CompressionHandler::ConvertTo(source,BitStreamInfo::VERTICAL_STREAM_FORMAT);
cout <<"Attribute SUM Square start" << endl;
cout << "\tUncompressed Structure" << endl;

inf = (*source)(0);
cout << "\tTime : ";
for (size_t i = 0 ; i < 5 ;i++)
{
	start = clock();
	dd = AlgoUtils::SumSquare(inf);
	end = clock();
	cout << end - start << " ";
}	
cout << endl;
cout <<"\n\t Naive Algorithm : " << endl;
cout << "\tTime : ";
for (size_t i = 0 ; i < 5 ; i++)
{
	double sum = 0;
	start = clock();
	for (size_t i = 0 ; i < vect.size() ; i++)
	{
		sum += vect[i] * vect[i];
	}

	end = clock();
	
	cout <<  end - start << " ";
}
cout << endl;
cout << "\t Comrpessed Structure " << endl;

CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
inf = (*source)(0);
cout << "\tTime : ";
for (size_t i = 0 ; i < 5 ;i++)
{
	start = clock();
	dd = AlgoUtils::SumSquare(inf);
	end = clock();
	cout << end - start << " ";
}

	/*
	std::ofstream stream("MF.csv");
		stream << source->generateCSVStringofDecodedData();
		stream.close();*/
	
	delete source;
}

WrapDataSource * TestComputations::CreateDataSource(string csv_path)
{
	//"C:\\Data\\weather.nominal.csv"
	CsvConnection cConcsv(csv_path.data(),',','\n','""');		
	ExtractedCsvDTO *dat = cConcsv.extractData();
	WrapDataSource *ds = new WrapDataSource(*dat,"0");	
	ds->encodeAtrributes();
	return ds;
}

WrapDataSource * TestComputations::CreateDataSource()
{
	WrapDataSource * ws = new WrapDataSource();	
	int rows = 100;
	dynamic_bitset<> test(rows,25ul);
/*

	BitStreamInfo * inf = new VBitStream();
	BitStreamInfo * inf2 = new VBitStream();
	
*/
	
	
		BitStreamInfo * inf = new EWAH();
	//	BitStreamInfo * inf2 = new EWAH();
		
	
	
	inf->CompressWords(test);
	//inf2->CompressWords(test);
	vector<BitStreamInfo *> atts(1);
	atts[0] = inf;
	//atts[1] = inf2;
inf->Print();

//	inf->convert(test);
	EncodedAttributeInfo * encodeds = new EncodedAttributeInfo();	
	encodeds->setNoOfVBitStreams(atts.size(),rows);
	encodeds->setVBitStreams(atts);
	vector<EncodedAttributeInfo * > attribs(1);
	attribs[0] = encodeds;
	ws->CodedAtts(attribs);
	
	return ws;
}

void TestComputations::TestMemoryLeaks()
{
	//
	WrapDataSource * source = CreateDataSource();
// 	vector<BitStreamInfo *> info22 =  source->codedAttributes()[0]->vBitStreams();
// 	BitStreamInfo * result  = *(info22[0]) & *(info22[1]);
// 	delete result;
	//CompressionHandler::ConvertTo(source,BitStreamInfo::EWAH_COMPRESSION);
	//Commons::DeleteVector(info22.begin(),info22.end());
	cout << "\n\tWithout compression : "<< endl;

	/*
	delete m_data_sources;
		delete m_lsd;*/
	
	delete source;
	cout << endl;

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

double TestComputations::VectorSum( vector<int> & vect )
{
	double result = 0 ; 
	for (size_t i = 0 ; i < vect.size() ; i++)
	{
		result += vect[i];
	}
	return result;
}

double TestComputations::VectorSum(vector<double> & vect)
{
	double result = 0 ; 
	for (size_t i = 0 ; i < vect.size() ; i++)
	{
		result += vect[i];
	}
	return result;
}

void TestComputations::CreateDoubleArray(vector<double> & vect,EncodedAttributeInfo * attr)
{

	long precisiion = static_cast<EncodedDoubleAttribute *>(attr)->Precision();
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
		//vect[i] = (double)temp.to_ulong()/(double)precisiion;
		vect[i] = (double)temp.to_ulong();
	}

}

//knn_cup_DoubleCol
void TestComputations::DoubleAttributeSum()
{
	//randInts
TestAttributeSum(CreateDataSource("10mill_randvalues_data","10mill_randvalues_data","randvalues"));
//TestAttributeSum(CreateDataSource("knn_cup_DoubleCol1_1000000_data","knn_cup_DoubleCol1_1000000_metadata","knn_cup_DoubleCol"));
}

void TestComputations::IntAttributeSum()
{//intVal_100000
//TestAttributeSum(CreateDataSource("4mill_randvalues_data","4mill_randvalues_metadata","randvalues"));
	TestAttributeSum(CreateDataSource("intVal_single_data","intVal_single_metadata","intVal_100000"));
}

void TestComputations::IntAttributeSumSquare()
{
	//CreateDataSource("randInts_data","randInts_metadata","randInts")
	TestAttributeSumSquare(CreateDataSource("randInts_data","randInts_metadata","randInts"));
}

void TestComputations::IntAttributeRangeQuery()
{	//randvalues
	TestRangeQueryInt(CreateDataSource("9mill_randvalues_data","9mill_randvalues_metadata","randvalues"));
}