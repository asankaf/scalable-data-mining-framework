#include "StdAfx.h"
#include "Distribution.h"

Distribution::Distribution(void)
{
}

Distribution::Distribution(int _num_bags,int _num_classes) 
{
	int i;
	m_perClassPerBag = new double * [_num_bags];
	m_perBagLength = _num_bags;
	m_perClassLength = _num_classes;
	m_perBag = new double [_num_bags];
	m_perClass = new double [_num_classes];
	for (i=0;i<_num_bags;i++)
	{
		m_perClassPerBag[i] = new double [_num_classes];
	}
	totaL = 0;
} 
Distribution::~Distribution(void)
{ 
}
