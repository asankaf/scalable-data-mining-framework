#include "StdAfx.h"
#include "DistinctValue.h"
#include <iostream>
using namespace std;
DistinctValue::DistinctValue(void)
{
	m_value = NULL;
}

DistinctValue::DistinctValue(BitStreamInfo * _value)
{
	m_value = _value;

}

DistinctValue::DistinctValue(BitStreamInfo * _value,string _unique)
{
	m_value = _value;
	m_string = _unique;
}

void DistinctValue::Print()
{
	cout <<"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
	cout <<"Printing Distinct Value : " << endl;
	cout <<"Printing string : " << m_string << endl;
	cout << "Printing value : "<< endl;
	m_value->Print();
	cout <<"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
}

DistinctValue::~DistinctValue(void)
{
	if (m_value != NULL)
	{
		delete m_value;
	}
}
