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
	//
	//TestClassifier(CreateDataSource("C:\\Data\\test dataset-10000.csv"));
	//TestClassifier(CreateDataSource("C:\\Data\\iris.csv"));
	//TestClassifier(CreateDataSource("C:\\Data\\soyaTestsort.csv"));
	//TestClassifier(CreateDataSource("C:\\Data\\weather.nominal.csv"));
	//TestClassifier(CreateDataSource("C:\\Data\\weather.csv"));
	//test dataset-10000.csv
	//soyabean_10col_metadata.xml
	//TestClassifier(CreateDataSource("soyabean_10col_data","soyabean_10col_metadata","soyabean"));
	//TestClassifier(CreateDataSource("poker_5mill_data","poker_5mill_metadata","poker_5mill_"));
	//TestClassifier(CreateDataSource("soya_5col_data","soya_5col_metadata","soya_5col_"));
	TestClassifier(CreateDataSource("SoyabeanColumns5mil\\integrate\\soyabean_data",
		"SoyabeanColumns5mil\\integrate\\soyabean_metadata","soyabean_5mill"));
	//TestClassifier(CreateDataSource("soyabean_1mill_10col_data","soyabean_1mill_10col_metadata","soyabean_1mill_10col"));
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
	//CsvConnection cConcsv(csv_path.data(),',','\n');		
	//ExtractedCsvDTO *dat = cConcsv.extractData();
	/*
	vector<int> meta_vect(5);
		meta_vect[0] = 2;
		meta_vect[1] = 1;
		meta_vect[2] = 1;
		meta_vect[3] = 2;
		meta_vect[4] = 2;*/
	
	ExtractedCsvDTO *dat = cConcsv.extractData();
	WrapDataSource *ds = new WrapDataSource(dat,"0");	
	ds->encodeAtrributes();
	return ds;
}

void TestNaiveBayes::TestClassifier(WrapDataSource * ds)
{
	//CompressionHandler::ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
	cout << endl;
	TestSMNaiveBayes(ds,ds->codedAttributes().size()-1,5);
	/*
	CompressionHandler::ConvertTo(ds,BitStreamInfo::EWAH_COMPRESSION);
		cout << endl;
		TestSMNaiveBayes(ds,ds->codedAttributes().size()-1,5);*/
	
	cout << endl;
	TestWekaNaiveBayes(ds,ds->codedAttributes().size()-1,5);
	delete ds;
}

void TestNaiveBayes::TestWekaNaiveBayes(WrapDataSource * _data_source, int _class_index,int _loop)
{
	cout << "Executing Weka Implementation " << endl;
	WekaInstances * instances = new WekaInstances(_data_source,_class_index,true);

	cout <<"Starting to build classifier : " << endl;
	cout << "Time Spent : ";
	for (size_t i = 0 ; i < _loop ; i++)
	{
		NaiveBayesMod bayes;
		clock_t start,end;

		start = clock();
		bayes.buildClassifier(instances,_class_index);
		end = clock();
		cout <<(end -start) << "\t";

	}
	cout << endl;
	delete instances;	
}

void TestNaiveBayes::TestSMNaiveBayes(WrapDataSource * _data_source, int _class_index,int _loop)
{
	cout << "Executing SM Implementation " << endl;
	cout <<"Starting to build classifier : " << endl;
	cout << "Time Spent : ";
	for (size_t i = 0 ; i < _loop ; i++)
	{
		NaiveBayes bayes;
		clock_t start,end;

		start = clock();
		bayes.buildClassifier(_data_source,_class_index);
		end = clock();
		cout <<(end -start) << "\t";

	}
	cout << endl;
}


WrapDataSource * TestNaiveBayes::CreateDataSource(string datafile,string metadFile,string filename)
{
	LoadSavedDataSources *lsd = new LoadSavedDataSources(metadFile,datafile);
	//	LoadSavedDataSources *lsd = new LoadSavedDataSources("soyabean_0.5mil_metadata","soyabean_0.5mil_data");
	DataSources *dsLoaded = lsd->loadSavedEncodedData();
	return (*dsLoaded)(filename);
}

