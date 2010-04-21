#include "StdAfx.h"
#include "DataSource.h"



DataSource::DataSource(void)
{
}

DataSource::DataSource(WrapDataSource * _wrapped,int _class_index)
{
	buildDataSource(_wrapped);
	m_row_count = _wrapped->noOfRows();
	if (_class_index >= _wrapped->noOfAttributes() || _class_index < 0)
	{
		this->m_classIndex = _wrapped->noOfAttributes() -1;
	}
	m_no_of_attributes = m_attributes.size();
	this->m_numClasses = m_attributes[m_classIndex]->UniqueValues().size();
}

DataSource::~DataSource(void)
{
	for (size_t i = 0 ; i < m_attributes.size() ; i++)
	{
		delete m_attributes[i];
	}
}

Attribute * DataSource::classAttribute()
{
	return attribute(m_classIndex);
}

Attribute * DataSource::attribute(int _index)
{
	return m_attributes[_index];
}

void DataSource::buildDataSource(WrapDataSource *_source)
{
	//throw and exception if all the attributes are not nominal
	vector<EncodedAttributeInfo *> original_atts = _source->codedAttributes();
	vector<Attribute *> attributes(original_atts.size());
	initialiseWeights(attributes.size());
	
	//m_type = original_atts[0]->bitStreamAt(0)->Type();
	BitStreamInfo * _info = original_atts[0]->vBitStreams()[0];
	m_type = _info->Type();
	for (size_t i = 0 ; i < attributes.size(); i++)
	{
		assert(original_atts[i]->attributeType() == MULTICAT_VAL);
		attributes[i] = new Attribute(m_weights[i],getDistinctValues(original_atts[i]));
		attributes[i]->name(original_atts[i]->attributeName());
	}
	m_attributes = attributes;

}

void DataSource::Print()
{
	cout <<"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
	cout << "Printing Datasource" << endl;
	for (size_t i = 0 ; i < m_attributes.size() ; i++)
	{cout << "Attribute Index : " << i << endl;
		m_attributes[i]->Print();
	}
	cout <<"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
}

vector<DistinctValue *> DataSource::getDistinctValues( EncodedAttributeInfo * _attribute )
{
	//TODO : Replace the assertion with an exception
	assert(_attribute->attributeType() == MULTICAT_VAL);
	
	EncodedMultiCatAttribute * multi_cat = static_cast<EncodedMultiCatAttribute *>(_attribute);
	vector<BitStreamInfo *> bitstreams = AlgoUtils::FindDistinctValues(multi_cat);
	vector<DistinctValue *> distinct_vals(bitstreams.size());
	DistinctValue * val = NULL;
	for (size_t i = 0 ; i < bitstreams.size() ; i++)
	{
			distinct_vals[i] = new DistinctValue(bitstreams[i],multi_cat->uniqueValList()[i]);
	}

	return distinct_vals;
}

void DataSource::initialiseWeights(int _attr_no)
{
	for (size_t i = 0 ; i < _attr_no ; i++)
	{
		m_weights.push_back(1);
	}
}
