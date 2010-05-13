#include "StdAfx.h"
#include "Instances.h"


Instances::Instances(void)
{
}


Instances::Instances(WrapDataSource * _wrapped,int _class_index)
{
	buildDataSource(_wrapped);
	m_row_count = _wrapped->noOfRows();
	if (_class_index >= _wrapped->noOfAttributes() || _class_index < 0)
	{
		this->m_classIndex = _wrapped->noOfAttributes() -1;
	}else
	{
		m_classIndex = _class_index;
	}
	m_no_of_attributes = m_attributes.size();
	this->m_numClasses = m_attributes[m_classIndex]->UniqueValues().size();
}

Instances::~Instances(void)
{
	for (size_t i = 0 ; i < m_attributes.size() ; i++)
	{
		delete m_attributes[i];
	}
}

AbstractAtt * Instances::classAttribute()
{
	return attribute(m_classIndex);
}

AbstractAtt * Instances::attribute(int _index)
{
	return m_attributes[_index];
}

void Instances::buildDataSource(WrapDataSource *_source)
{
	//throw and exception if all the attributes are not nominal
	vector<EncodedAttributeInfo *> original_atts = _source->codedAttributes();
	vector<AbstractAtt *> attributes(original_atts.size());
	initialiseWeights(attributes.size());

	//m_type = original_atts[0]->bitStreamAt(0)->Type();
	BitStreamInfo * _info = original_atts[0]->vBitStreams()[0];
	m_type = _info->Type();
	for (size_t i = 0 ; i < attributes.size(); i++)
	{
		assert(original_atts[i]->attributeType() == MULTICAT_VAL);
		attributes[i] = new NominalAttribute(m_weights[i],getDistinctValues(original_atts[i]));
		attributes[i]->name(original_atts[i]->attributeName());
	}
	m_attributes = attributes;

}

void Instances::Print()
{
	cout <<"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
	cout << "Printing Datasource" << endl;
	for (size_t i = 0 ; i < m_attributes.size() ; i++)
	{cout << "Attribute Index : " << i << endl;
	m_attributes[i]->Print();
	}
	cout <<"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
}

vector<BayesDistinct *> Instances::getDistinctValues( EncodedAttributeInfo * _attribute )
{
	//TODO : Replace the assertion with an exception
	assert(_attribute->attributeType() == MULTICAT_VAL);

	EncodedMultiCatAttribute * multi_cat = static_cast<EncodedMultiCatAttribute *>(_attribute);
	vector<BitStreamInfo *> bitstreams = AlgoUtils::FindDistinctValues(multi_cat);
	vector<BayesDistinct *> distinct_vals(bitstreams.size());
	BayesDistinct * val = NULL;
	for (size_t i = 0 ; i < bitstreams.size() ; i++)
	{
		distinct_vals[i] = new BayesDistinct(bitstreams[i],multi_cat->uniqueValList()[i]);
	}

	return distinct_vals;
}

void Instances::initialiseWeights(int _attr_no)
{
	for (size_t i = 0 ; i < _attr_no ; i++)
	{
		m_weights.push_back(1);
	}
}

