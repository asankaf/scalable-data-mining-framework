#include "StdAfx.h"
#include "NominalAttribute.h"

NominalAttribute::NominalAttribute(void)
{
}

NominalAttribute::NominalAttribute(double _weight, std::vector<BayesDistinct *> &_values)
{
	m_weight = _weight;
	m_uniqueValues = _values;
	m_uniqueValNo = _values.size();
}


BayesDistinct * NominalAttribute::distinctValueAt(size_t post)
{
	return m_uniqueValues[post];
}

BitStreamInfo * NominalAttribute::bitStreamAt(size_t position)
{
	return this->m_uniqueValues[position]->Value();
}

void NominalAttribute::Print()
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

string NominalAttribute::value(int valIndex)
{	
	return m_uniqueValues[valIndex]->name();
}

NominalAttribute::~NominalAttribute(void)
{
	for (size_t i = 0 ; i < m_uniqueValues.size() ; i++)
	{
		delete m_uniqueValues[i];
	}
}