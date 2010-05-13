#include "TestClassifierTestSource.h"


TestClassifierTestSource::TestClassifierTestSource(void)
{
}

TestClassifierTestSource::~TestClassifierTestSource(void)
{
}

WrapDataSource * TestClassifierTestSource::CreateDataSource(string csv_path)
{
	//"C:\\Data\\weather.nominal.csv"
	CsvConnection cConcsv(csv_path.data(),',','\n','""');		
	ExtractedCsvDTO *dat = cConcsv.extractData();
	WrapDataSource *ds = new WrapDataSource(dat,"0");	
	ds->encodeAtrributes();
	return ds;
}

WrapDataSource * TestClassifierTestSource::CreateDataSource(string datafile,string metadFile,string filename)
{
	LoadSavedDataSources *lsd = new LoadSavedDataSources(metadFile,datafile);
	//	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_0.5mil_metadata","soyabean_0.5mil_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	return (*dsLoaded)(filename);
}

void TestClassifierTestSource::TestBuilding()
{
	WrapDataSource * ds = CreateDataSource("C:\\Data\\weather.nominal.csv");
	ClassifierTestSource * source = new ClassifierTestSource(ds,ds->codedAttributes().size()-1);
	
}

void TestClassifierTestSource::TestSuite()
{
	TestBuilding();
}
