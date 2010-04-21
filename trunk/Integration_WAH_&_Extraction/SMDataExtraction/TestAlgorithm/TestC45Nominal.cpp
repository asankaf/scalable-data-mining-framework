#include "TestC45Nominal.h"
#include "BitStreamInfo.h"
#include "CompressionHandler.h"
#include "LoadSavedDataSources.h"


TestC45Nominal::TestC45Nominal(void)
{
}

TestC45Nominal::~TestC45Nominal(void)
{
}

void TestC45Nominal::TestSuite()
{
	// Testing unique itemset generation

	// Creating a wrapped data source

	//weather.nominal.csv
	//CsvConnection cConcsv("C:\\Data\\weather.nominal.csv",',','\n','""');	
	//soyaTest.csv
// 	CsvConnection cConcsv("C:\\Data\\soyaTest.csv",',','\n','""');	
// 	ExtractedCsvDTO *dat = cConcsv.extractData();
// 	WrapDataSource *ds = new WrapDataSource(*dat,"0");	
//soyabean_0.25mil_metadata.xml
	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabeansmall_100000_metadata","soyabeansmall_100000_data");
//	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_0.5mil_metadata","soyabean_0.5mil_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	WrapDataSource * ds =  (*dsLoaded)("soyabeansmall_100000");
	//soyaTestlarge,soyabeansmall_100000
	//WrapDataSource * ds =  (*dsLoaded)("soyabean");
	cout <<"Finished Loading data " << endl;
	cout << "=======================================" << endl;
	//ds->encodeAtrributes();
	cout << "Finished Encoding Data" << endl;
	cout << "=======================================" << endl;
	
	C45TreeNominal tree;
	CompressionHandler comp;
//	comp.ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
	tree.buildClassifier(ds);
	
}

// void TestC45Nominal::PrintDistribution(Distribution * _dist)
// {
// //_dist->
// }
