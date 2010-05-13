#include "TestNaiveBayes.h"
#include "BitStreamInfo.h"
#include "CompressionHandler.h"
#include "classifiertestsource.h"
#include "naivebayesmod.h"


TestNaiveBayes::TestNaiveBayes(void)
{
}

TestNaiveBayes::~TestNaiveBayes(void)
{
}

void TestNaiveBayes::TestSuite()
{
	//TestClassifier(CreateDataSource("C:\\Data\\test dataset-10000.csv"));
	//TestClassifier(CreateDataSource("C:\\Data\\iris.csv"));
	//TestClassifier(CreateDataSource("C:\\Data\\soyaTestsort.csv"));
	TestClassifier(CreateDataSource("C:\\Data\\weather.nominal.csv"));
	//test dataset-10000.csv
	//randInts
	//TestClassifier(CreateDataSource("randInts_data","randInts_metadata","randInts"));
	//TestClassifier(CreateDataSource("soyaTestlarge_300000_data","soyaTestlarge_300000_metadata","soyaTestlarge"));
	//TestClassifier(CreateDataSource("C:\\Data\\soybeanTest3.csv"));
	//soyaTestlarge_300000_metadata.xml
/*
	string line;
	vector<string> conf(4);
	size_t i = 0; 
	ifstream myfile ("config.txt");
	if (myfile.is_open())
	{
		cout << "File read successfully" << endl;
		while (! myfile.eof() )
		{
			getline (myfile,line);
			if (i < conf.size())
			{
				conf[i++] = line;
			}
			
		}
		myfile.close();
	}

	else cout << "Unable to open file"<< endl;; 
		
	if (conf[3] == "EWAH")
	{
		WrapDataSource * ds = CreateDataSource(conf[0],conf[1],conf[2]);

		CompressionHandler::ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
		TestClassifier(ds);
	}
	else{
		TestClassifier(CreateDataSource(conf[0],conf[1],conf[2]));
	}*/

}

WrapDataSource * TestNaiveBayes::CreateDataSource(string csv_path)
{
	//"C:\\Data\\weather.nominal.csv"
	CsvConnection cConcsv(csv_path.data(),',','\n','""');		
	ExtractedCsvDTO *dat = cConcsv.extractData();
	WrapDataSource *ds = new WrapDataSource(dat,"0");	
	ds->encodeAtrributes();
	return ds;
}

void TestNaiveBayes::TestClassifier(WrapDataSource * ds)
{
	CompressionHandler::ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
	//NaiveBayes bayes;
	NaiveBayesMod bayes;
	clock_t start,end;
	cout <<"Started to build classifier : " << endl;
	start = clock();
	bayes.buildClassifier(ds,ds->codedAttributes().size()-1);
	end = clock();
	cout << bayes.toString() << endl;
	cout << "Time Spent : " << end - start << endl;
	ClassifierTestSource * test_source = new ClassifierTestSource(ds,ds->codedAttributes().size()-1);
	bayes.ClassifyInstances(test_source);
	test_source->BuildConfusionMatrix();
	delete ds;
}

WrapDataSource * TestNaiveBayes::CreateDataSource(string datafile,string metadFile,string filename)
{
	LoadSavedDataSources *lsd = new LoadSavedDataSources(metadFile,datafile);
	//	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_0.5mil_metadata","soyabean_0.5mil_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	return (*dsLoaded)(filename);
}

