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
	//This test failed need to test rigorously
	DataSourceGenerator dg;
	vector<dynamic_bitset<>> bit_info_source = GetBitSetsForAttribute();
	EncodedAttributeInfo * attr = dg.CreateAttribute(bit_info_source,BitStreamInfo::WAH_COMPRESSION);
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
	//map<int,dynamic_bitset<>> result_map;
	vector<unsigned long int> unique_vals;
	typedef vector<dynamic_bitset<>>::const_iterator bitset_iter;
	cout<<endl;
	cout << "Printing Original Bit streams for source : " << endl;
	for (bitset_iter iter = bit_info_source.begin(); iter != bit_info_source.end(); iter++)
	{
		cout << *(iter) << endl;
	}
	cout<<endl;
	cout <<"printing unique Patterns " << endl;

	for (bitset_iter iter = unique_patters.begin(); iter != unique_patters.end(); iter++)
	{
		cout << *(iter) << endl;
	}
	dynamic_bitset<> bits =  utils.FindPattern(unique_patters.at(0),bit_info_source);
	cout <<"Test Result : " << bits<<endl;
	utils.GetUniqueBitmaps(attr,holder,unique_patters,map_index_values,unique_vals);
	attr->setAttID(13);
	utils.GetUniqueBitmaps(attr,holder,unique_patters,map_index_values,unique_vals);
	cout<<endl;
	cout <<"Printing ulong vals for indices  "<<endl;
	typedef vector<unsigned long int>::const_iterator ulong_iter;
	for (ulong_iter iter = unique_vals.begin(); iter != unique_vals.end(); iter++)
	{
		cout << *(iter) << endl;
	}
	cout<<endl;
	PrintBitStreamHolderVector(holder);
	cout<<endl;
	cout <<"Printing attribute Index mapping :" << endl;
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
		cout << "Index : " << index << endl;
		PrintBitStreamHolder(_holder.at(index));
	}
}

void TestAlgoUtil::PrintBitStreamHolder(BitStreamHolder * _holder)
{
	cout << "Printing Attribute Indices : " << endl;
	vector<int> att_info = _holder->Attribute_no();
	for (size_t index = 0; index < att_info.size(); index++)
	{
		cout << att_info.at(index)<<" , ";
	}
	cout << endl;
	cout << "Printing BitStream Indices : " << endl;
	vector<int> bit_stream_info = _holder->Bit_stream_no();
	for (size_t index = 0; index < bit_stream_info.size(); index++)
	{
		cout << bit_stream_info.at(index)<<" , ";
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

void TestAlgoUtil::TestUniqueVlauesForDataSource()
{
	DataSourceGenerator dg;
	vector<dynamic_bitset<>> bit_info_source = GetBitSetsForAttribute();
	EncodedAttributeInfo * attr = dg.CreateAttribute(bit_info_source,BitStreamInfo::WAH_COMPRESSION);
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

	//Creating dynamic bisets for Datasource
	vector<vector<dynamic_bitset<>>> source_bits;
	source_bits.push_back(bit_info_source);
	source_bits.push_back(bit_info_source);

	//Creating Unique values for data source
	vector<vector<dynamic_bitset<>>> dyn_sets;
	dyn_sets.push_back(unique_patters);
	dyn_sets.push_back(unique_patters);
	
	//Creating Datasource
	WrapDataSource * wrapped = dg.CreateDataSource(source_bits,BitStreamInfo::WAH_COMPRESSION,dyn_sets);


	map<int,vector<int>> map_index_values;
	//map<int,dynamic_bitset<>> result_map;
	vector<unsigned long int> unique_vals;
	typedef vector<dynamic_bitset<>>::const_iterator bitset_iter;
	cout<<endl;
	cout << "Printing Original Bit streams for source : " << endl;
	for (bitset_iter iter = bit_info_source.begin(); iter != bit_info_source.end(); iter++)
	{
		cout << *(iter) << endl;
	}
	cout<<endl;
	cout <<"printing unique Patterns " << endl;

	for (bitset_iter iter = unique_patters.begin(); iter != unique_patters.end(); iter++)
	{
		cout << *(iter) << endl;
	}
	dynamic_bitset<> bits =  utils.FindPattern(unique_patters.at(0),bit_info_source);
	cout <<"Test Result : " << bits<<endl;
	//utils.GetUniqueBitmaps(wrapped,holder,map_index_values);	
	utils.GetUniqueBitmaps(wrapped,holder,map_index_values,unique_vals);
	cout<<endl;
	cout <<"Printing ulong vals for indices  "<<endl;
	typedef vector<unsigned long int>::const_iterator ulong_iter;
	for (ulong_iter iter = unique_vals.begin(); iter != unique_vals.end(); iter++)
	{
		cout << *(iter) << endl;
	}
	cout<<endl;
	PrintBitStreamHolderVector(holder);
	cout<<endl;
	cout <<"Printing attribute Index mapping :" << endl;
	PrintAttIdIndexMapping(map_index_values);
}

void TestAlgoUtil::ConvertVectorToMapConvertsion()
{
	AlgoUtils utils;
	map<int,vector<int>> map_vector;
	vector<int> vect;
	vect.push_back(1);
	vect.push_back(2);
	vect.push_back(14);
	vect.push_back(121);
	vect.push_back(15);
	map_vector[3] = vect;
	map<int,int> maps = utils.CreateIndexAttributeMap(map_vector);
	typedef map<int,int>::const_iterator map_iter;
	for (map_iter iter = maps.begin(); iter != maps.end();iter++)
	{
		cout << "Key : " << iter->first << ", Pair : " << iter->second << endl;
	}
}

WrapDataSource * TestAlgoUtil::GetTestWrappedDataSource()
{

	DataSourceGenerator dg;
	vector<dynamic_bitset<>> bit_info_source = GetBitSetsForAttribute();
	EncodedAttributeInfo * attr = dg.CreateAttribute(bit_info_source,BitStreamInfo::WAH_COMPRESSION);
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

	//Creating dynamic bisets for Datasource
	vector<vector<dynamic_bitset<>>> source_bits;
	source_bits.push_back(bit_info_source);
	source_bits.push_back(bit_info_source);
	source_bits.push_back(bit_info_source);

	//Creating Unique values for data source
	vector<vector<dynamic_bitset<>>> dyn_sets;
	dyn_sets.push_back(unique_patters);
	dyn_sets.push_back(unique_patters);
	dyn_sets.push_back(unique_patters);

	//Creating Datasource
	WrapDataSource * wrapped = dg.CreateDataSource(source_bits,BitStreamInfo::WAH_COMPRESSION,dyn_sets);

return wrapped;
}

void TestAlgoUtil::PrintBitSets(vector<dynamic_bitset<>> & _bit_sets)
{
	for (size_t i = 0; i < _bit_sets.size(); i++)
	{
		cout << "Index " << i << " : " << _bit_sets.at(i)<<endl;
	}
}


