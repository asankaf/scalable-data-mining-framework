#include "TestRangeSplitter.h"
//#include "..\..\..\..\..\mydocuments\visual studio 2008\projects\smdataextraction\smpreprocessing\rangesplitter.h"
#include "..\SMPreprocessing\RangeSplitter.h"
#include "..\SMDataExtraction\LoadSavedDataSources.h"
#include "..\SMDataExtraction\DataSources.h"

TestRangeSplitter::TestRangeSplitter(void)
{
}

TestRangeSplitter::~TestRangeSplitter(void)
{
}

void TestRangeSplitter::TestSuite()
{
	UniqueBitmapCreation();

}

void TestRangeSplitter::UniqueBitmapCreation()
{
	TestUniqueBitmapCreation(CreateDataSource("splitter_test_data","splitter_test_metadata","small_splitter_test"));
}

void TestRangeSplitter::TestUniqueBitmapCreation(WrapDataSource * _source)
{
	EncodedAttributeInfo * inf = (*_source)(0);
	vector<double> vals(5);
	vals[0] = 0;
	vals[1] = 1;
	vals[2] = 2;
	vals[3] = 3;
	vals[4] = 4;
	RangeSplitter *splitter = new RangeSplitter(inf,_source->noOfRows());
	EncodedMultiCatAttribute* mulAtt = splitter->SplitIntoEqualRanges(3);
	cout<<"Unique Values : "<<endl;
	for (int i = 0 ; i < mulAtt->uniqueValList().size() ; i++)
	{
		cout << mulAtt->uniqueValList()[i]<<endl;
	}
	cout<<"Converted Values : "<<endl;
	for (int j = 0 ; j < _source->noOfRows() ; j++)
	{
		cout << mulAtt->decodeTheTuple(j + 1)<<endl;
	}

	////Ranges considered [0,1],(1,2],(2,3]
	//EncodedAttributeInfo * inf = (*_source)(0);
	//RangeSplitter splitter;
	//vector<BitStreamInfo * > unique_maps = splitter.Splitter(inf,_source->noOfRows());


	//vector<int> vect(_source->noOfRows());
	//CreateIntArray(vect,inf);
	//int first_range = 0,second_range = 0, third_range = 0;
	//for (size_t i = 0 ; i < vect.size() ; i++)
	//{
	//	if (vect[i] <= 1)
	//	{
	//		first_range++;
	//	}
	//	else if (vect[i] <= 2 && vect[i] > 1)
	//	{
	//		second_range++;
	//	}
	//	else {
	//		third_range++;
	//	}
	//}

	////Testing whether the counts obtain from both methods are equal
	//cout << "Unique Map 0 " << unique_maps[0]->count() << " " << first_range << endl;
	//cout << "Unique Map 1 " << unique_maps[1]->count() << " " << second_range << endl;
	//cout << "Unique Map 2 " << unique_maps[2]->count() << " " << third_range << endl;
}

void TestRangeSplitter::CreateIntArray(vector<int> & _vect,EncodedAttributeInfo * _attr)
{

	vector<BitStreamInfo *> streams1 = _attr->vBitStreams();
	vector<dynamic_bitset<>> streams(streams1.size());
	for (size_t i = 0 ; i < streams.size() ; i++)
	{
		streams[i] = streams1[i]->Decompress();
	}

	for (size_t i = 0 ; i < _vect.size() ; i++)
	{
		dynamic_bitset<> temp((int)_attr->NoOfVBitStreams());
		for (size_t j = 0 ; j < temp.size() ; j++)
		{
			temp[j] = streams[j][i];
		}
		_vect[i] = (int)temp.to_ulong();
	}

}

WrapDataSource * TestRangeSplitter::CreateDataSource(string datafile,string metadFile,string dsnName)
{
	LoadSavedDataSources * m_lsd = new LoadSavedDataSources(metadFile,datafile);
	DataSources * m_data_sources = m_lsd->loadSavedEncodedData();
	return (*m_data_sources)(dsnName);
}
