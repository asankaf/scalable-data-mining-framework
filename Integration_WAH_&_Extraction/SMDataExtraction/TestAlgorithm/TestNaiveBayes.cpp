#include "TestNaiveBayes.h"
#include "BitStreamInfo.h"
#include "CompressionHandler.h"
#include "classifiertestsource.h"
#include "naivebayesmod.h"


TestNaiveBayes::TestNaiveBayes(void):m_execution_config("algorithm.cfg"),LOOP_COUNT("loops"),
COMPRESSION("compression"),
METAFILE_NAME("metafile"),
DATAFILE_NAME("datafile"),
FILE_NAME("filename"),
FOLDER_NAME("datafolder"),
LOAD_TYPE("loadtype"),
LOAD_TYPE_MULTI("multiple"),
LOAD_TYPE_SINGLE("single"),
LOAD_TYPE_CSV("csv")
{

	m_no_of_loops = boost::lexical_cast<int>(Commons::ReadConfiguration(LOOP_COUNT));
	if (Commons::ReadConfiguration(COMPRESSION) == "NO")
	{
		m_compression = BitStreamInfo::VERTICAL_STREAM_FORMAT;
	} else if (Commons::ReadConfiguration(COMPRESSION) == "EWAH")
	{
		m_compression = BitStreamInfo::EWAH_COMPRESSION;
	}
	else if (Commons::ReadConfiguration(COMPRESSION) == "WAH")
	{
		m_compression = BitStreamInfo::WAH_COMPRESSION;
	}
	
	m_meta_file_name = Commons::ReadConfiguration(METAFILE_NAME);
	m_data_file_name = Commons::ReadConfiguration(DATAFILE_NAME);
	m_file_name = Commons::ReadConfiguration(FILE_NAME);
	m_folder_name = Commons::ReadConfiguration(FOLDER_NAME);
}

TestNaiveBayes::~TestNaiveBayes(void)
{
}

void TestNaiveBayes::TestSuite()
{
	WrapDataSource * ds = NULL;
	if (Commons::ReadConfiguration(LOAD_TYPE) == LOAD_TYPE_SINGLE)
	{
		ds = Utils::CreateDataSource(m_data_file_name,m_meta_file_name,m_file_name,1000);
	} else if (Commons::ReadConfiguration(LOAD_TYPE) == LOAD_TYPE_MULTI)
	{
		ds = Utils::CreateDataSourceFromMultipleFiles(m_folder_name,m_meta_file_name,m_file_name);
	}
	else if (Commons::ReadConfiguration(LOAD_TYPE) == LOAD_TYPE_CSV)
	{
		ds = Utils::CreateDataSource(m_file_name);
	}
	
	//TestClassifier(ds);
	PerformMemoryTest();
}
void TestNaiveBayes::PerformMemoryTest()
{
	cout << "Algorithm : Naive Bayes" << endl;
	DWORD process_id = GetCurrentProcessId();
	int ii =0 ;
	cout << "Current Process ID : " << process_id<<endl;
	cin>>ii;	
	cout << "Start Time :" << time(NULL)<< endl;

	WrapDataSource * ds = Utils::CreateDataSource(m_data_file_name,m_meta_file_name,m_file_name);
	cout <<"Finished Creating Data Source : "  << time(NULL)<< endl;	
	CompressionHandler::ConvertTo(ds,m_compression);
	cout <<"Starting To Execute Algorithm : " << time(NULL)<< endl;		

	for (size_t i = 0 ; i < m_no_of_loops ; i++)
	{
		SingleExecution(ds);
	}	
	cout <<"Finished Executing Algorithm : " << time(NULL) << endl;
	//cin>>ii;
	delete ds;
	cout <<"Deleted Data Source: " << time(NULL) << endl;
	//cin>>ii;
}
void TestNaiveBayes::SingleExecution(WrapDataSource * _ds)
{
	NaiveBayes bayes;
	bayes.buildClassifier(_ds,_ds->noOfAttributes()-1);
	//cout <<bayes.toString() << endl;
}

void TestNaiveBayes::TestClassifier(WrapDataSource * ds)
{
	cout << endl;
	TestSMNaiveBayes(ds,ds->codedAttributes().size()-1,m_no_of_loops);	
	cout << endl;
	//TestWekaNaiveBayes(ds,ds->codedAttributes().size()-1,5);
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
		//cout << bayes.toString() << endl;

	}
	cout << endl;
}