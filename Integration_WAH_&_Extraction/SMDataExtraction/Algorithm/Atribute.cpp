#include "StdAfx.h"
#include "Atribute.h"
#include <iostream>

using namespace std;
Attribute::Attribute(void)
{

}

Attribute::Attribute(double _weight, std::vector<DistinctValue*> &_values)
{
	m_weight = _weight;
	m_uniqueValues = _values;
	m_uniqueValNo = _values.size();
}

void Attribute::Print()
{
	cout <<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
	cout << "Printing Attribute :"<< endl;
	cout <<"Attr Name : " << m_name << endl;
	cout << "Unique Value : " << m_uniqueValNo << endl;
	for (size_t i = 0 ; i < m_uniqueValues.size() ; i++)
	{
		cout << "Value Index : " << i << endl;
		m_uniqueValues[i]->Print();
	}
	cout <<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
}

string Attribute::value(int valIndex)
{	
	return m_uniqueValues[valIndex]->name();
}

Attribute::~Attribute(void)
{
	for (size_t i = 0 ; i < m_uniqueValues.size() ; i++)
	{
		delete m_uniqueValues[i];
	}
}
