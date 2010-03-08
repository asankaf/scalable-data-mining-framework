#include "DummyDataExtraction.h"

DataExtraction::DataExtraction()
{
	srand ( time(NULL) );
	size_t size=10;
	data=vector<bitstring>(1,bitstring(size));

	//cout<<"bitstring:";

	for (int i=0;i<size;i++)
	{
		data[0][i]=rand() % 2;
		//cout<<data[0][i];
	}
	//cout<<endl<<endl;
}

DataExtraction::~DataExtraction()
{

}

bitstring DataExtraction::getBitString(string dataSourceID,int column,int bit)
{
	return data[0];
}

bitstring DataExtraction::getBitString(Operand* op)
{
	return data[0];
}