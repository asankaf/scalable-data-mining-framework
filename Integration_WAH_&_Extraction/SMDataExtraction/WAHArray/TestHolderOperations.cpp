#include "StdAfx.h"
#include "TestHolderOperations.h"

TestHolderOperations::TestHolderOperations(void)
{
}

TestHolderOperations::~TestHolderOperations(void)
{
}

BitStreamHolder * TestHolderOperations::CreateHolder(vector<int> & _attribute_array,vector<int> & _index_array)
{
	BitStreamHolder * holder = new BitStreamHolder();
	holder->Attribute_no(_attribute_array);
	holder->Bit_stream_no(_index_array);
	return holder;
}

void TestHolderOperations::TestHolderSuite()
{
	TestDifferenceOperator();
}

void TestHolderOperations::TestDifferenceOperator()
{
// 	vector<int> first_vector;
// 	first_vector.push_back(12);
	TestAlgoUtil test_util;
	AlgoUtils utils;
WrapDataSource * wrapped = test_util.GetTestWrappedDataSource();
	//BitStreamHolder * holder_1 = CreateHolder()
vector<BitStreamHolder *> unique_vals;
map<int,vector<int>> index_attno_map;
vector<unsigned long int> pattern_index_map;
utils.GetUniqueBitmaps(wrapped,unique_vals,index_attno_map,pattern_index_map);
map<int,int> int_map_index_attno;
int_map_index_attno = utils.CreateIndexAttributeMap(index_attno_map);
BitStreamHolder * holder_1 = unique_vals.at(0);
BitStreamHolder * holder_2 = unique_vals.at(1);
holder_2->AddStreamNo(2);
holder_2->AddStreamNo(3);
cout << "Holder 1: "<< endl;
holder_1->Print();

cout << "Holder 2: "<< endl;
holder_2->Print();
vector<BitStreamHolder * > new_holder_vect = holder_2->Difference(holder_1,int_map_index_attno);
cout << "Starting to print BitStreamInfo Array" << endl;
PrintBitInfoVector(new_holder_vect);

}

void TestHolderOperations::PrintBitInfoVector(vector<BitStreamHolder * > & _holder)
{
	typedef vector<BitStreamHolder *>::const_iterator holder_iter;
	size_t index = 0;
	for (holder_iter start = _holder.begin(); start != _holder.end(); start++)
	{
		cout << "Printing Holder : " << index++ << endl;
		(*(start))->Print();
	}
}

void TestHolderOperations::TestHash()
{
	vector<int> attr;
	attr.push_back(1);
	vector<int> index;
	index.push_back(1);
	index.push_back(2);
	index.push_back(3);
	index.push_back(4);
	BitStreamHolder * holder  = CreateHolder(attr,index);
	cout << "Hash : "<<holder->Hash() << endl;
}
