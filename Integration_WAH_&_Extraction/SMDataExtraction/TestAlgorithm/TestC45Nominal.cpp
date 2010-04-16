#include "TestC45Nominal.h"


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
	CsvConnection cConcsv("C:\\Data\\weather.nominal.csv",',','\n','""');	
	ExtractedCsvDTO *dat = cConcsv.extractData();
	WrapDataSource *ds = new WrapDataSource(*dat,"0");	
	cout <<"Finished Loading data " << endl;
	cout << "=======================================" << endl;
	ds->encodeAtrributes();
	cout << "Finished Encoding Data" << endl;
	cout << "=======================================" << endl;
	
	C45TreeNominal tree;
	tree.buildClassifier(ds);
	
}

// void TestC45Nominal::PrintDistribution(Distribution * _dist)
// {
// //_dist->
// }
