#include "StdAfx.h"
#include "TestAlgoUtil.h"
#include "BitStreamHolder.h"



TestAlgoUtil::TestAlgoUtil(void)
{
}

TestAlgoUtil::~TestAlgoUtil(void)
{
}

void TestAlgoUtil::TestAttLevelUniquePatternFinding()
{
	DataSourceGenerator dg;
	EncodedAttributeInfo * attr = dg.CreateAttribute(GetBitSetsForAttribute(),BitStreamInfo::WAH_COMPRESSION);
	attr->setAttName("Name");
	attr->setAttID(12);
	AlgoUtils utils;
	vector<BitStreamHolder *> holder;
	vector<dynamic_bitset<> > unique_patters;
	dynamic_bitset<> pattern(3);
	pattern[2] = 1;
	pattern[1] = 0;
	pattern[0] = 0;
	unique_patters.push_back(pattern);

	pattern[2] = 1;
	pattern[1] = 0;
	pattern[0] = 1;
	unique_patters.push_back(pattern);

	pattern[2] = 0;
	pattern[1] = 1;
	pattern[0] = 1;
	unique_patters.push_back(pattern);

	pattern[2] = 0;
	pattern[1] = 1;
	pattern[0] = 0;
	unique_patters.push_back(pattern);

	map<int,vector<int>> map_index_values;
	map<int,dynamic_bitset<>> result_map;
	result_map = utils.GetUniqueBitmaps(attr,holder,unique_patters,map_index_values);

	PrintBitStreamHolderVector(holder);
	PrintAttIdIndexMapping(map_index_values);


}

void TestAlgoUtil::PrintAttIdIndexMapping(map<int ,vector<int>> & _index_att_value_map)
{
	typedef map<int,vector<int>>::const_iterator ci_t;
	typedef vector<int>::const_iterator vect_t;
	for (ci_t start = _index_att_value_map.begin(); start != _index_att_value_map.end(); start++)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
		cout << "Atribute Index : " << start->first<< endl;
		for (vect_t second = start->second.begin(); second != start->second.end() ; second ++)
		{
			cout << *(second);
		}
		cout << endl;
	}
}

void TestAlgoUtil::PrintBitStreamHolderVector(vector<BitStreamHolder *> _holder)
{
	for (size_t index = 0; index < _holder.size(); index++)
	{
		PrintBitStreamHolder(_holder.at(index));
	}
}

void TestAlgoUtil::PrintBitStreamHolder(BitStreamHolder * _holder)
{
	cout << "Printing Attribute Indices : " << endl;
	vector<int> att_info = _holder->Attribute_no();
	for (size_t index = 0; index < att_info.size(); index++)
	{
		cout << att_info.at(index);
	}
	cout << endl;
	cout << "Printing BitStream Indices : " << endl;
	vector<int> bit_stream_info = _holder->Bit_stream_no();
	for (size_t index = 0; index < bit_stream_info.size(); index++)
	{
		cout << bit_stream_info.at(index);
	}
	cout << endl;

	cout <<"Printing BitstreamInfo " << endl;
	PrintBitStreamInfo(_holder->Bit_stream());

}

void TestAlgoUtil::PrintBitStreamInfo(BitStreamInfo * _bit_stream)
{
	if (_bit_stream->Type() == BitStreamInfo::WAH_COMPRESSION)
	{
		WAHStructure * comp = dynamic_cast<WAHStructure *>(_bit_stream);
		comp->printCompressedStream();
	}
	dynamic_bitset<> decom = _bit_stream->Decompress();
	cout<<"Decompressed : " << decom << endl;
}

vector<dynamic_bitset<>> TestAlgoUtil::GetBitSetsForAttribute()
{
	vector<dynamic_bitset<>> result;
	BitsetGenerator bg;
	dynamic_bitset<>temp =  bg.getMiddleAlternatingBitset(100,50,49,true);
	result.push_back(temp);
	temp = bg.getAlternatingBitset(100);
	result.push_back(temp);
	result.push_back(temp.flip());
	return result;
}

void TestAlgoUtil::TestPatternFindBitInfoMethod()
{
	DataSourceGenerator dg;
	BitsetGenerator bg;
	dynamic_bitset<> bit_set_1 = bg.getAlternatingBitset(100);
	dynamic_bitset<> bit_set_2 = bg.getPureBitset(100,false);
	dynamic_bitset<> bit_set_3 = bg.getPureBitset(100,true);
	// 	bit_set_1[10] = 1;
	// 	bit_set_1[20] = 1;
	// 	bit_set_1[30] = 1;
	vector<dynamic_bitset<>> _container;
	_container.push_back(bit_set_2);
	_container.push_back(bit_set_1);
	_container.push_back(bit_set_3);
	dynamic_bitset<> pattern(3);
	vector<vector<dynamic_bitset<>>> _cont;
	_cont.push_back(_container);
	//pattern[2] = 1;
	WrapDataSource * wrpped = dg.CreateDataSource(_cont,BitStreamInfo::WAH_COMPRESSION);
	vector<EncodedAttributeInfo *> atts = wrpped->codedAttributes();
	vector<BitStreamInfo *> bit_streams = atts.at(0)->vBitStreams();
	AlgoUtils utils;
	PrintBitSets(bit_streams);
	BitStreamInfo * result = utils.FindPattern(pattern,bit_streams);
	cout << "Result : " << endl;
	result->Print();
	cout << "Count : " << result->Count() << endl;
}

void TestAlgoUtil::TestPatternFindBitsetMethods()
{
	BitsetGenerator bg;
	dynamic_bitset<> bit_set_1 = bg.getPureBitset(100,false);
	dynamic_bitset<> bit_set_2 = bit_set_1;
	// 	bit_set_1[10] = 1;
	// 	bit_set_1[20] = 1;
	// 	bit_set_1[30] = 1;
	bit_set_1 = bg.getAlternatingBitset(100);
	vector<dynamic_bitset<>> _container;
	_container.push_back(bit_set_2);
	_container.push_back(bit_set_1);
	dynamic_bitset<> pattern(2);
	PrintBitSets(_container);
	pattern[pattern.size()-1] = 1;	
	pattern[0] = 1;	
	AlgoUtils utils;
	dynamic_bitset<> result = utils.FindPattern(pattern,_container);
	cout << "Result : " << result<<endl;

}

void TestAlgoUtil::PrintBitSets(vector<BitStreamInfo *> & _bit_sets)
{
	for (size_t i = 0; i < _bit_sets.size() ; i++)
	{
		cout <<"Index " << i << " : " <<endl;
		BitStreamInfo * inf = _bit_sets.at(i);
		dynamic_bitset<> decompressed = inf->Decompress(); 
		if (inf->Type() == BitStreamInfo::WAH_COMPRESSION)
		{
			WAHStructure * compressed = dynamic_cast<WAHStructure *>(inf);
			cout <<"Compressed Stream : " << endl;
			compressed->printCompressedStream();
		}
		cout << decompressed << endl;	
	}
}

void TestAlgoUtil::PrintBitSets(vector<dynamic_bitset<>> & _bit_sets)
{
	for (size_t i = 0; i < _bit_sets.size(); i++)
	{
		cout << "Index " << i << " : " << _bit_sets.at(i)<<endl;
	}
}


