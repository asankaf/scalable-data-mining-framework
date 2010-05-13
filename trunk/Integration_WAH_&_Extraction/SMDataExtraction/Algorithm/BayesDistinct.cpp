#include "StdAfx.h"
#include "BayesDistinct.h"
#include <iostream>

using namespace std;

BayesDistinct::BayesDistinct(void)
{
	m_value = NULL;
}

BayesDistinct::BayesDistinct(BitStreamInfo * _value)
{
	m_value = _value;

}

BayesDistinct::BayesDistinct(BitStreamInfo * _value,string _unique)
{
	m_value = _value;
	m_stringVal = _unique;
}

void BayesDistinct::Print()
{
	cout <<"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
	cout <<"Printing Distinct Value : " << endl;
	cout <<"Printing string : " << m_stringVal << endl;
	cout << "Printing value : "<< endl;
	m_value->Print();
	cout <<"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
}

BayesDistinct::~BayesDistinct(void)
{
	if (m_value != NULL)
	{
		delete m_value;
	}
}
