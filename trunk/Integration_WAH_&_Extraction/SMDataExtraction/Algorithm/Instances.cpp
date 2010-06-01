#include "StdAfx.h"
#include "Instances.h"
#include "numericattribute.h"


Instances::Instances(void)
{
}


Instances::Instances(WrapDataSource * _wrapped,int _class_index):NOMINAL_CLASS("class NominalAttribute"),NUMERIC_CLASS("class NumericAttribute")
{
	if (_wrapped->codedAttributes()[_class_index]->attributeType() != MULTICAT_VAL)
	{
		assert(false);
	}
	buildNominalAtts(_wrapped);
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
	buildNumericAtts(_wrapped);
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

void Instances::buildNominalAtts(WrapDataSource *_source)
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
		if(original_atts[i]->attributeType() == MULTICAT_VAL)
		{
		attributes[i] = new NominalAttribute(m_weights[i],getDistinctValues(original_atts[i]));
		attributes[i]->name(original_atts[i]->attributeName());
		}
	}
	m_attributes = attributes;

}

void Instances::buildNumericAtts(WrapDataSource *_source)
{
	vector<EncodedAttributeInfo *> original_atts = _source->codedAttributes();
	
	//m_type = original_atts[0]->bitStreamAt(0)->Type();
	BitStreamInfo * _info = original_atts[0]->vBitStreams()[0];
	m_type = _info->Type();
	for (size_t i = 0 ; i < m_attributes.size(); i++)
	{
		if(original_atts[i]->attributeType() == DOUBLE_VAL ||original_atts[i]->attributeType() == SIGNEDINT_VAL)
		{
			m_attributes[i] = new NumericAttribute(m_weights[i],m_attributes[m_classIndex],original_atts[i]);
			m_attributes[i]->name(original_atts[i]->attributeName());
		}
	}
	
}

bool Instances::isNumeric(size_t _att_index)
{
	if (typeid(*(m_attributes.at(_att_index))).name() ==NUMERIC_CLASS)
	{
		return true;
	}
	return false;
}

bool Instances::isNominal(size_t _att_index)
{
	if (typeid(*(m_attributes.at(_att_index))).name() ==NOMINAL_CLASS)
	{
		return true;
	}
	return false;
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

string Instances::nameOfAttribute(size_t _att_index)
{
	return m_attributes[_att_index]->name();
}

size_t Instances::noOfVlaues(size_t _att_index)
{
	return m_attributes[_att_index]->UniqueValues().size();
}

string Instances::valueOfNominalAtt(size_t _att_index,size_t _value_index)
{
return m_attributes[_att_index]->distinctValueAt(_value_index)->name();
}

string Instances::classValueString(size_t _class_value)
{
	return m_attributes[m_classIndex]->value(_class_value);
}

void Instances::initialiseWeights(int _attr_no)
{
	m_weights = vector<double>(_attr_no);
	for (size_t i = 0 ; i < _attr_no ; i++)
	{
		m_weights[i] = 1;
	}
}