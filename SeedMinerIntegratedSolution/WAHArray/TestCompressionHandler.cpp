#include "StdAfx.h"
#include "TestCompressionHandler.h"



TestCompressionHandler::TestCompressionHandler(void)
{
}

TestCompressionHandler::~TestCompressionHandler(void)
{
}

vector<dynamic_bitset<>> TestCompressionHandler::CreateTestBitsets()
{
	BitsetGenerator bg;
	vector<dynamic_bitset<>> result;
	dynamic_bitset<> temp = bg.getAlternatingBitset(100);

	result.push_back(temp);
	temp = bg.getPureBitset(100,true);
	result.push_back(temp);
	temp = bg.getPureBitset(100,false);
	temp = bg.getMiddleCompressedBitset(100,31,31*2,true);
	result.push_back(temp);
	
	return result;
	
}

void TestCompressionHandler::TestCompression()
{
	CompressionHandler com_hand;
	DataSourceGenerator data_source;
	vector<dynamic_bitset<>> temp =  CreateTestBitsets();
	vector<vector<dynamic_bitset<>>> input;
	input.push_back(temp);
	WrapDataSource * wrapper = data_source.CreateDataSource(input,BitStreamInfo::VERTICAL_STREAM_FORMAT);
	cout << "Converting to Compressed Stream" << endl;
	com_hand.ConvertTo(wrapper,BitStreamInfo::WAH_COMPRESSION);
	PrintWrapDataSource(wrapper);

}

void TestCompressionHandler::PrintWrapDataSource(WrapDataSource * _wrapper)
{
vector<EncodedAttributeInfo *> temp = _wrapper->codedAttributes();
for (int i  = 0; i < temp.size(); i++)
{
	cout << "Attribute : " << i << endl;
	PrintAttributes(temp.at(i));
}
}

void TestCompressionHandler::PrintAttributes(EncodedAttributeInfo * _attribute)
{
	vector<BitStreamInfo *> vertical = _attribute->vBitStreams();
	for (int i = 0; i < vertical.size(); i++)
	{
		BitStreamInfo * inf = vertical.at(i);
		if (inf->Type() == BitStreamInfo::WAH_COMPRESSION)
		{
			WAHStructure * wah = dynamic_cast<WAHStructure *>(inf);
			cout << "Compressed Stream :" << endl;
			wah->printCompressedStream();
		}
		
		cout << "Attribute Stream  "<<i <<" : "<<inf->Decompress()<< endl;
	}
}