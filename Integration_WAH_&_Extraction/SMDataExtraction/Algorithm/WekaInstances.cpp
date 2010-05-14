#include "StdAfx.h"
#include "WekaInstances.h"
#include "commons.h"

WekaInstances::WekaInstances(void)
{
}

WekaInstances::WekaInstances(WrapDataSource * _source,int _class_index,bool _delete_atts)
{
	m_rows = _source->noOfRows();
	m_class_index = _class_index;

	m_data_source = new double*[m_rows];
	m_headers = _source;
	m_attributes = _source->codedAttributes().size();

	EncodedMultiCatAttribute * multi_cat_info = static_cast<EncodedMultiCatAttribute *>(m_headers->codedAttributes()[m_class_index]);
	m_num_classes = multi_cat_info->noOfUniqueValues();
	m_class_values = multi_cat_info->uniqueValList();

	for (size_t i = 0 ; i < m_rows ; i++)
	{
		//Number of Input variables are always one less than the total variables.
		m_data_source[i] = new double[m_attributes];	
	}

	EncodedAttributeInfo * att_info = NULL;
	m_numeric_nominal = new bool[m_attributes];

	size_t att_index = 0;
	//Traversing through each attribute and populating the test source
	for (size_t i = 0; i < _source->codedAttributes().size() ; i++)
	{
		att_info = _source->codedAttributes()[i];
		ConvertAttributeTo(att_info,m_data_source,att_index);
		m_numeric_nominal[i] = AttributeTypeSelector(att_info);
		att_index++;
		if (_delete_atts)
		{
			ClearBitStreams(att_info);
		}
	}
	PrintSource();
}

bool WekaInstances::AttributeTypeSelector(EncodedAttributeInfo * _attribute)
{
	if (_attribute->attributeType() == MULTICAT_VAL)
	{
		return false;
	}else {
		return true;
	}
}

void WekaInstances::init()
{
	m_data_source = NULL;
	m_headers = NULL;
	m_numeric_nominal = NULL;
}

void WekaInstances::ClearBitStreams(EncodedAttributeInfo * _attribute)
{
	_attribute->DeleteVBitStreams();
}

void WekaInstances::ConvertAttributeTo(EncodedAttributeInfo * _attribute,double ** _values,int _att_index)
{

	vector<dynamic_bitset<>> bit_sets(_attribute->NoOfVBitStreams());
	//Create a set of dynamic bit sets from the attribute.
	CreateDynamicBitSets(_attribute,bit_sets);

	switch (_attribute->attributeType())
	{
	case DOUBLE_VAL:
		{
			EncodedDoubleAttribute * double_att = static_cast<EncodedDoubleAttribute *>(_attribute);
			double precission = double_att->Precision();	
			size_t no_of_streams = double_att->NoOfVBitStreams();
			dynamic_bitset<> temp_bitset(no_of_streams);
			for (size_t  i = 0 ; i < m_rows ; i++)
			{
				for (size_t j = 0 ; j < no_of_streams ; j++)
				{
					temp_bitset[j] = bit_sets[j][i];				
				}
				_values[i][_att_index] = temp_bitset.to_ulong() * precission;
			}

		}
		break;
	case MULTICAT_VAL:
		{
			dynamic_bitset<> temp_bitset(bit_sets.size());
			for (size_t  i = 0 ; i < m_rows ; i++)
			{
				for (size_t j = 0 ; j < temp_bitset.size() ; j++)
				{
					temp_bitset[j] = bit_sets[j][i];				
				}
				_values[i][_att_index] = temp_bitset.to_ulong();				
			}
			EncodedMultiCatAttribute * multi_att = static_cast<EncodedMultiCatAttribute *>(_attribute);
			m_distinct_vals[_att_index] = multi_att->uniqueValList();
		}
		break;
	case SIGNEDINT_VAL:
		{
			dynamic_bitset<> temp_bitset(bit_sets.size());
			for (size_t  i = 0 ; i < m_rows ; i++)
			{
				for (size_t j = 0 ; j < temp_bitset.size() ; j++)
				{
					temp_bitset[j] = bit_sets[j][i];				
				}
				_values[i][_att_index] = temp_bitset.to_ulong();
			}
		}
		break;

	case DATE_VAL:
		{
			dynamic_bitset<> temp_bitset(bit_sets.size());
			for (size_t  i = 0 ; i < m_rows ; i++)
			{
				for (size_t j = 0 ; j < temp_bitset.size() ; j++)
				{
					temp_bitset[j] = bit_sets[j][i];				
				}
				_values[i][_att_index] = temp_bitset.to_ulong();
			}
		}	
		break;
	}
}

void WekaInstances::CreateDynamicBitSets(EncodedAttributeInfo * _attribute,vector<dynamic_bitset<>> & _bit_sets)
{
	for (size_t i = 0 ; i < _attribute->NoOfVBitStreams() ; i++)
	{
		_bit_sets[i] = _attribute->bitStreamAt(i)->Decompress();
	}
}

void WekaInstances::PrintSource()
{
	for (size_t i = 0 ; i < m_rows ; i++)
	{
		cout << "Printing Tuple " << (i+1) << endl;
		for (size_t j = 0 ; j < m_attributes ; j++)
		{
			cout << m_data_source[i][j] << "\t";
		}
		cout << endl;
	}
}

WekaInstances::~WekaInstances(void)
{
}
