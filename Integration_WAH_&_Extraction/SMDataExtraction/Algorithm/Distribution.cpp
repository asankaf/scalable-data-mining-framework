#include "StdAfx.h"
#include "Distribution.h"

Distribution::Distribution(void)
{
	m_perClassPerBag = NULL;
	m_perBag = NULL;
	m_perClass = NULL;
	m_perClassLength = 0;
	m_perClassPerBag = 0;	
}

Distribution::Distribution(Distribution * toMerge)
{
	totaL = toMerge->totaL;
	m_perClassLength = toMerge->m_perClassLength;
	m_perClass = new double [m_perClassLength];
	memcpy(m_perClass,toMerge->m_perClass,m_perClassLength * sizeof(double));	
	m_perClassPerBag = new double*[1];
	m_perClassPerBag[0] = new double [m_perClassLength];
	memcpy(m_perClassPerBag[0],toMerge->m_perClassPerBag[0],m_perClassLength * sizeof(double));		
	m_perBag = new double [1];
	m_perBagLength = 1;
	m_perBag[0] = totaL;
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
	initialiseArrays();
	totaL = 0;
}

void Distribution::initialiseArrays()
{
	for (size_t i = 0 ; i < m_perBagLength ; i++)
	{
		m_perBag[i] = 0;
	}
	for (size_t i = 0 ; i < m_perClassLength ; i++)
	{
		m_perClass[i] = 0;
	}
	for (size_t i = 0 ; i < m_perBagLength ; i++)
	{
		for (size_t j = 0 ; j < m_perClassLength ; j++)
		{
			m_perClassPerBag[i][j] = 0;
		}
	}
}

Distribution::~Distribution(void)
{ 
	ClearClassPerBags();
	ClearBags();
	ClearClass();
}

void Distribution::Print()
{
	cout << "Printing distribution" << endl;
	if (m_perClass != NULL)
	{
		cout << "Perclass array :" ;
		for (size_t i = 0 ; i < m_perClassLength ; i++)
		{
			cout << m_perClass[i] << " , ";
		}
		cout << endl;
	} 
	else
	{
		cout << "Perclass is NULL" << endl;
	}
	if (m_perBag != NULL)
	{
		cout << "PerBag array :" ;

		for (size_t i = 0 ; i < m_perBagLength ; i++)
		{
			cout << m_perBag[i] << " , ";
		}
		cout << endl;
	} 
	else
	{
		cout << "Perclass is NULL" << endl;
	}

	if (m_perClassPerBag != NULL)
	{
		cout << "PerClassPerBag array :" ;

		for (size_t i = 0 ; i < m_perBagLength ; i++)
		{
			for (size_t j = 0 ; j < m_perClassLength ; j++)
			{
				cout << m_perClassPerBag[i][j] << " , ";
			}
			cout<< endl;

		}
	} 
	else
	{
		cout << "Perclassperbag is NULL" << endl;
	}
	cout << endl;
}

double Distribution::numCorrect()
{
	return m_perClass[maxClass()];
}

int Distribution::maxBag()
{
	double max;
	int maxIndex;
	int i;

	max = 0;
	maxIndex = -1;
	for (i=0;i<m_perBagLength;i++)
		if (Utils::grOrEq(m_perBag[i],max))
		{
			max = m_perBag[i];
			maxIndex = i;
		}
		return maxIndex;
}

double Distribution::numIncorrect()
{

	return totaL-numCorrect();
}

Distribution::Distribution(DataSource * source, BitStreamInfo * _existence_map)
{
	m_perClassPerBag = new double * [1];
	m_perBag = new double [1];
	m_perBagLength = 1;
	m_perBag[0] = 0;
	totaL = 0;
	m_perClassLength = source->NumClasses();
	m_perClass = new double[m_perClassLength];
// 	for (size_t i = 0 ; i < m_perClassLength ; i++)
// 	{
// 		m_perClass[i] = 0;
// 	}
	m_perClassPerBag[0] = new double[source->NumClasses()];
// 	for (size_t i = 0 ; i < m_perBagLength ; i++)
// 	{
// 		for (size_t j = 0 ; j < m_perClassLength ; j++)
// 		{
// 			m_perClassPerBag[i][j] = 0;
// 		}
// 	}
	initialiseArrays();
	add(0,source,_existence_map);
}

double Distribution::perBag(int bagIndex)
{
	return m_perBag[bagIndex];
}

double Distribution::perClass(int classIndex)
{
	return m_perClass[classIndex];
}

double Distribution::perClassPerBag(int bagIndex, int classIndex)
{
	return m_perClassPerBag[bagIndex][classIndex];
}

double Distribution::numIncorrect(int index)
{
	return m_perBag[index]- numCorrect(index);
}

int Distribution::maxClass(int index)
{
	double maxCount = 0;
	int maxIndex = 0;
	int i;

	if (Utils::gr(m_perBag[index],0))
	{
		for (i=0;i<m_perClassLength;i++)
			if (Utils::gr(m_perClassPerBag[index][i],maxCount))
			{
				maxCount = m_perClassPerBag[index][i];
				maxIndex = i;
			}
			return maxIndex;
	}else
		return maxClass();
}

double Distribution::numCorrect(int index)
{
	return m_perClassPerBag[index][maxClass(index)];
}

int Distribution::maxClass()
{
	double maxCount = 0;
	int maxIndex = 0;
	int i;

	for (i=0;i<m_perClassLength;i++)
		if (Utils::gr(m_perClass[i],maxCount)) {
			maxCount = m_perClass[i];
			maxIndex = i;
		}

		return maxIndex;
}

// double Distribution::perClass(int classIndex)
// {
// 	return m_perClass[classIndex];
// }

bool Distribution::check(double minNoObj)
{
	int counter = 0;
	int i;

	for (i=0;i<m_perBagLength;i++)
	{
		if (Utils::grOrEq(m_perBag[i],minNoObj))
			counter++;
	}
	if (counter > 1)
		return true;
	else
		return false;
}

void Distribution::add(int bagIndex,DataSource * instance, BitStreamInfo * _existence_map) 
{
	size_t classIndex;
	double weight;

	Attribute * attribute = instance->Attributes()[instance->ClassIndex()];
	vector<DistinctValue *> classes = attribute->UniqueValues();
	weight = attribute->Weight();
	BitStreamInfo * new_value = NULL;
	int count_val = 0;
	for (classIndex = 0 ; classIndex < classes.size() ; classIndex++)
	{
		new_value = *(_existence_map) & *(classes[classIndex]->Value());
		count_val = new_value->Count();
		m_perClassPerBag[bagIndex][classIndex] = m_perClassPerBag[bagIndex][classIndex] +count_val * weight;
		m_perBag[bagIndex] = m_perBag[bagIndex]+ count_val * weight;
		m_perClass[classIndex] = m_perClass[classIndex]+count_val * weight;
		delete new_value;
	}		

	totaL = totaL + _existence_map->Count() * weight;
}

void Distribution::ClearClassPerBags()
{
	if (m_perClassPerBag != NULL)
	{
		for (size_t i = 0 ; i < m_perBagLength ; i++)
		{
			delete m_perClassPerBag[i];
			m_perClassPerBag[i] = NULL;
		}		
	}
	delete m_perClassPerBag;
}

void Distribution::ClearBags()
{
	delete m_perBag;
}

void Distribution::ClearClass()
{
	delete m_perClass;
}