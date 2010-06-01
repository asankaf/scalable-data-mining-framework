#include "StdAfx.h"
#include "NumericAttribute.h"
#include "algoutils.h"

NumericAttribute::NumericAttribute(double _weight,AbstractAtt * _class_value, EncodedAttributeInfo * _attribute)
{
	m_weight = _weight;
	if (_attribute->attributeType() == MULTICAT_VAL ||_attribute->attributeType() == DATE_VAL)
	{
		assert(false);
	}
	double count = 0;
	m_means = new double[_class_value->numValues()];
	m_deviations = new double[_class_value->numValues()];
	for (size_t i = 0 ; i < _class_value->numValues() ; i++)
	{
		count = _class_value->bitStreamAt(i)->Count();
		if (!count)
		{
			assert(false);
		}
		m_means[i] = AlgoUtils::USum(_attribute,_class_value->bitStreamAt(i))/count;
		m_deviations[i] = AlgoUtils::SumSquare(_attribute,_class_value->bitStreamAt(i)) - count * m_means[i]* m_means[i];
		m_deviations[i] /= count;
		m_deviations[i] = sqrt(m_deviations[i]);
	}
}

double NumericAttribute::GetMeanValue(size_t _class_value)
{
	return m_means[_class_value];
}


double NumericAttribute::GetStandardDeviation(size_t _class_value)
{
	return m_deviations[_class_value];
}

NumericAttribute::~NumericAttribute(void)
{
	delete [] m_means;
	delete [] m_deviations;
}
