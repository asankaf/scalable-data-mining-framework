#include "StdAfx.h"
#include "ClassifierTestSource.h"
#include "utils.h"


ClassifierTestSource::ClassifierTestSource(void)
{
	init();
}

ClassifierTestSource::ClassifierTestSource(WrapDataSource *_source, int _class_index)
{

	//CompressionHandler::ConvertTo(_source,BitStreamInfo::VERTICAL_STREAM_FORMAT);

	m_class_index = _class_index;
	m_rows = _source->noOfRows();
	m_data_source = new double*[m_rows];
	m_headers = _source;
	m_predicted_classes = new double[m_rows];
	m_original_classes = new double[m_rows];
	EncodedMultiCatAttribute * multi_cat_info = static_cast<EncodedMultiCatAttribute *>(m_headers->codedAttributes()[m_class_index]);
	m_number_of_classes = multi_cat_info->noOfUniqueValues();
	m_class_values = multi_cat_info->uniqueValList();

	for (size_t i = 0 ; i < m_rows ; i++)
	{
		//Number of Input variables are always one less than the total variables.
		m_data_source[i] = new double[_source->codedAttributes().size()-1];	
	}

	EncodedAttributeInfo * att_info = NULL;
	size_t att_index = 0;
	//Traversing through each attribute and populating the test source
	for (size_t i = 0; i < _source->codedAttributes().size() ; i++)
	{
		att_info = _source->codedAttributes()[i];
		/* Class attribute is stored in the original class value array.
		m_data_source the other inputs. */
		if (i != m_class_index)
		{
			ConvertAttributeTo(att_info,m_data_source,att_index);
			att_index++;
		}
		else
		{
			ConvertAttributeTo(att_info,m_original_classes);
		}


	}

	/*
	PrintSource();
		cout << endl;
		cout << "Printing Original Values " << endl;
		Print(m_original_classes,m_rows);*/
	

}

void ClassifierTestSource::PrintSource()
{
for (size_t i = 0 ; i < m_rows ; i++)
{
	cout << "Printing Tuple " << (i+1) << endl;
	for (size_t j = 0 ; j < m_headers->codedAttributes().size() -1 ; j++)
	{
		cout << m_data_source[i][j] << "\t";
	}
	cout << endl;
}
}

void ClassifierTestSource::ConvertAttributeTo(EncodedAttributeInfo * _attribute,double * _values)
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
				_values[i] = temp_bitset.to_ulong() * precission;
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
				_values[i] = temp_bitset.to_ulong();
			}
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
				_values[i] = temp_bitset.to_ulong();
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
				_values[i] = temp_bitset.to_ulong();
			}
		}	
		break;
	}
}

void ClassifierTestSource::ConvertAttributeTo(EncodedAttributeInfo * _attribute,double ** _values,int _att_index)
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

void ClassifierTestSource::CreateDynamicBitSets(EncodedAttributeInfo * _attribute,vector<dynamic_bitset<>> & _bit_sets)
{
	for (size_t i = 0 ; i < _attribute->NoOfVBitStreams() ; i++)
	{
		_bit_sets[i] = _attribute->bitStreamAt(i)->Decompress();
	}
}

void ClassifierTestSource::Print( double * _array,int _length )
{
	for (size_t i = 0 ; i < _length ; i++)
	{
	//	cout << "Class for Tuple "<< (i+1) << " : "<< _array[i] << endl;
			cout << "Class for Tuple (original,predicted)"<< (i+1) << " : "<< m_original_classes[i]<<" "<<m_predicted_classes[i] << endl;
	}
}

ClassifierTestSource::~ClassifierTestSource(void)
{

}

void ClassifierTestSource::init()
{
	m_data_source = NULL;
	m_headers = NULL;
	m_original_classes = NULL;
	m_predicted_classes = NULL;
	m_confusion_matrix = NULL;
}

void ClassifierTestSource::PrintConfusion(double ** _array,int _length)
{
	cout << "Print Confusion " << endl;
	for (size_t i = 0 ; i <_length ; i++)
	{
		for (size_t j = 0 ; j < _length ; j++)
		{
			cout << m_confusion_matrix[i][j] << "\t";
		}
		cout << endl;
	}
}

string ClassifierTestSource::GetConfusionString()
{
	string output = "\n";
	for (size_t i = 0 ; i < m_number_of_classes ; i++)
	{
		if (i < 10)
		{
		output += " "+Utils::toStringVal((int)i)+" ";
		}else{
		output += Utils::toStringVal((int)i)+" ";
		}
	}
	output.erase(output.end()-1);
	output += " <-- classified as\n";

	for (size_t i = 0 ; i < m_number_of_classes ; i++)
	{
		for (size_t j = 0 ; j < m_number_of_classes ; j++)
		{
			if ((int)m_confusion_matrix[i][j] < 10)
			{
				output += " "+Utils::toStringVal((int)m_confusion_matrix[i][j])+" ";
			}else{
			output += Utils::toStringVal((int)m_confusion_matrix[i][j])+" ";
			}
		}
		output.erase(output.end()-1);
		if (i < 10)
		{
			output += " |  "+Utils::toStringVal((int)i) + " = " +m_class_values[i]+"\n";
		}else
		{
output += " | "+Utils::toStringVal((int)i) + " = " +m_class_values[i]+"\n";
		}
		
	}

	return output;
}

void ClassifierTestSource::BuildConfusionMatrix()
{
	//Replace this with an exception
	assert(m_data_source != NULL && m_original_classes != NULL && m_predicted_classes != NULL);
	m_confusion_matrix = new double*[m_number_of_classes];
	for (size_t i = 0 ; i < m_number_of_classes ; i++)
	{
		m_confusion_matrix[i] = new double[m_number_of_classes];
	}

	for (size_t i = 0 ; i < m_number_of_classes ; i++)
	{
		for (size_t j = 0 ; j < m_number_of_classes ; j++)
		{
			m_confusion_matrix[i][j] = 0;
		}
	}
	
	for (size_t i = 0 ; i < m_rows ; i++)
	{
		m_confusion_matrix[(int)m_original_classes[i]][(int)m_predicted_classes[i]]++;
	}
	cout << "Comparison : " << endl;
	//Print(NULL,m_rows);
	cout << "Confusion Matrix : " << endl;
	//PrintConfusion(m_confusion_matrix,m_number_of_classes);
	cout <<GetConfusionString()<< endl;
}

