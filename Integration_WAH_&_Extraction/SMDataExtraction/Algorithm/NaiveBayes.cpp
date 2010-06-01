#include "StdAfx.h"
#include "NaiveBayes.h"
#include "algoutils.h"
#include "utils.h"
#include <time.h>
#include <iostream>
#include "abstractatt.h"
#include "numericattribute.h"

using namespace std;

NaiveBayes::NaiveBayes(void)
{
	Init();
}

NaiveBayes::~NaiveBayes(void)
{
	
	if (m_Counts != NULL)
		{
		/*	for (size_t i = 0 ; i <  m_Instances->numClasses() ; i++)
			{
				for (size_t j = 0 ; j <m_Instances->numAttributes() - 1 ; j++)
				{
					delete m_Counts[i][j];
				}
				delete m_Counts[i];
			}
			
			*/
			for (size_t attIndex = 0 ; attIndex < m_Instances->numClasses()  ; attIndex++)	
			{			
				for (int j = 0; j < m_Instances->numAttributes()-1; j++)
				{
					if (m_Counts[attIndex][j] != NULL)
				{
					delete[] m_Counts[attIndex][j];	
				}
					
				}		 
				delete[] m_Counts[attIndex];
			}

			delete[] m_Counts;
		}
	
	if (m_Devs != NULL)
	{
		for (size_t i = 0 ; i < m_Instances->numClasses() ; i++)
		{
			delete[] m_Devs[i];
		}
		delete[] m_Devs;
	}
	if (m_Means != NULL)
	{
		for (size_t i  = 0 ; i < m_Instances->numClasses() ; i++)
		{
			delete[] m_Means[i];
		}
		delete[] m_Means;
	}

	if (m_Priors != NULL)
	{
		delete m_Priors;
	}
	if (m_Instances != NULL)
	{
		delete m_Instances;
	}

}

void NaiveBayes::Init()
{
m_Instances = NULL;
m_Counts = NULL;
m_Devs = NULL;
m_Means = NULL;
m_Priors = NULL;
}

void NaiveBayes::buildClassifier(WrapDataSource * instances,int class_index)
{
	int attIndex = 0;
	double sum = 0;
	m_class_index = class_index;
	//m_Instances = new DataSource(instances,class_index);
	m_Instances = new Instances(instances,class_index);
	// Reserve space
	m_Counts = new double**[m_Instances->numClasses()];
	for (size_t i = 0 ; i < m_Instances->numClasses(); i++)
	{
		m_Counts[i] = new double*[m_Instances->numAttributes() - 1];
		
	}
	
	m_Means = new double*[m_Instances->numClasses()];
	for (size_t i = 0 ; i < m_Instances->numClasses() ; i++)
	{
		m_Means[i] = new double[m_Instances->numAttributes() - 1];
	}
	
	m_Devs = new double *[m_Instances->numClasses()];
	for (size_t i = 0 ; i < m_Instances->numClasses() ; i++)
	{
	m_Devs[i] = new double[m_Instances->numAttributes() - 1];
	}
	
	m_Priors = new double[m_Instances->numClasses()];
	
	for (size_t attIndex = 0 ; attIndex < m_Instances->numAttributes()-1 ; attIndex++)	
	 {			
			for (int j = 0; j < m_Instances->numClasses(); j++)
			{
				if (m_Instances->isNominal(attIndex))
				{
					m_Counts[j][attIndex] = new double[m_Instances->attribute(attIndex)->numValues()];
				}else 
				{
					m_Counts[j][attIndex] = NULL;
				}
				/*
				for (size_t k = 0 ; k < m_Instances->attribute(attIndex)->numValues();k++)
								{
									m_Counts[j][attIndex][k] = 0;
								}*/
				
			}		 
		
	}
BitStreamInfo * class_value = NULL;
	// Compute counts and sums
//PrintCountArr(m_Counts);
for(size_t c_index = 0 ; c_index < m_Instances->numClasses() ; c_index++ )
{
		class_value = m_Instances->attribute(m_Instances->ClassIndex())->bitStreamAt(c_index);

			for(size_t a_Index = 0,attIndex = 0 ; a_Index < m_Instances->numAttributes();a_Index++)
			{
				if (class_index != a_Index)
				{
					//Attribute * attribute =  m_Instances->attribute(a_Index);
					AbstractAtt * attribute =  m_Instances->attribute(a_Index);
					if (m_Instances->isNominal(a_Index))
{
	for (size_t att_vals = 0 ; att_vals < attribute->numValues() ; att_vals++) 
						{	
							if (m_Instances->isNominal(attIndex))
							{
								m_Counts[c_index][attIndex][att_vals] = AlgoUtils::ANDCount(class_value,attribute->bitStreamAt(att_vals));		
							}
							
	
						}
}
					attIndex++;

				}
			}
			m_Priors[c_index] = class_value->Count();
		
	}


//Compute Means	and Stadard deviations
attIndex = 0;
NumericAttribute * temp_numeric = NULL;
for (size_t a_index = 0 ; a_index < m_Instances->numAttributes() ;a_index++ )
{
	if (attIndex != m_class_index)
	{
		if (m_Instances->isNumeric(a_index))
		{
			for (int j = 0; j < m_Instances->numClasses(); j++)
			{
					temp_numeric = dynamic_cast<NumericAttribute *>(m_Instances->attribute(attIndex));
					m_Means[j][attIndex] = temp_numeric->GetMeanValue(j);	
					m_Devs[j][attIndex] = temp_numeric->GetStandardDeviation(j);				
			}
		}
		attIndex++;
	}

}   

	// Normalize counts
	for(size_t attIndex = 0; attIndex < m_Instances->numAttributes()-1 ; attIndex++)
	{
		//Attribute  * attribute = m_Instances->attribute(attIndex);		
		AbstractAtt * attribute = m_Instances->attribute(attIndex);		
			if (m_Instances->isNominal(attIndex))
{
	for (int j = 0; j < m_Instances->numClasses(); j++) {
					sum = Utils::sum(m_Counts[j][attIndex],attribute->numValues());
					for (int i = 0; i < attribute->numValues(); i++)
					{
						m_Counts[j][attIndex][i] =
							(m_Counts[j][attIndex][i] + 1) 
							/ (sum + (double)attribute->numValues());
					}
				}
}
				
	}

	// Normalize priors
	sum = Utils::sum(m_Priors,m_Instances->numClasses());
	for (int j = 0; j < m_Instances->numClasses(); j++)
	{
		m_Priors[j] = (m_Priors[j] + 1) 
		/ (sum + (double)m_Instances->numClasses());
	}

	//PrintCountArr(m_Counts);
}


string NaiveBayes::toString()
{

	if (m_Instances == NULL) {
		return "Naive Bayes (simple): No model built yet.";
	}

	string text = "";
	int attIndex = 0;

	for (int class_index = 0; class_index < m_Instances->numClasses(); class_index++)
	{
		text.append("\n\nClass " + m_Instances->classValueString(class_index) 
			+ ": P(C) = " 
			+ Utils::doubleToString(m_Priors[class_index], 10, 8)
			+ "\n\n");

		for(size_t a_Index = 0,attIndex = 0; a_Index < m_Instances->numAttributes();a_Index++)
		{
			if (m_class_index != a_Index)
			{
				text.append("Attribute " + m_Instances->nameOfAttribute(a_Index) + "\n");

				if (m_Instances->isNumeric(a_Index))
				{
					text.append("Mean: " + Utils::doubleToString(m_Means[class_index][attIndex], 10, 3) + "\t");
					text.append("Standard Deviation: " + Utils::doubleToString(m_Devs[class_index][attIndex], 10, 3));
				}else
				{

					for (int j = 0; j < m_Instances->noOfVlaues(a_Index); j++)
					{
						text.append(m_Instances->valueOfNominalAtt(a_Index,j) + "\t");
					}

					text.append("\n");

					for (int j = 0; j < m_Instances->noOfVlaues(a_Index); j++)
						text.append(Utils::doubleToString(m_Counts[class_index][attIndex][j], 10, 8)
						+ "\t");
				}
				text.append("\n\n");
				attIndex++;
			}
		}
	}

	return text;
}

void NaiveBayes::PrintCountArr(double *** arr)
{
	for (int j = 0; j < m_Instances->numClasses(); j++)
	
	{			
		cout << "Class : " << m_Instances->attribute(m_Instances->ClassIndex())->distinctValueAt(j)->name() << endl;
	for (size_t attIndex = 0 ; attIndex < m_Instances->numAttributes()-1 ; attIndex++)		
		{
			cout << "\t Attribute : " << m_Instances->attribute(attIndex)->name()<<endl;
			//m_Counts[j][attIndex] = new double[m_Instances->attribute(attIndex)->numValues()];
			for (size_t k = 0 ; k < m_Instances->attribute(attIndex)->numValues();k++)
			{
				
				cout << "\t\t"<<k<<" : "<<arr[j][attIndex][k] <<endl;
			}
		}		 

	}
}

void NaiveBayes::ClassifyInstances( ClassifierTestSource * _source )
{
	double * predict_vals = _source->Predicted_classes();
	for (size_t i = 0 ; i < _source->Rows(); i++)
	{
		predict_vals[i] = ClassifyInstance(_source->Data_source()[i],_source->Headers()->codedAttributes().size() -1,_source->Headers());
	}

	//_source->Print(predict_vals,_source->Rows());

}

int NaiveBayes::ClassifyInstance(double * _inputs,size_t _no_of_atts,WrapDataSource * _header)
{
	int class_values = m_Instances->classAttribute()->numValues();
	double * classes = new double[class_values];

	for (size_t  i = 0; i < class_values ; i++)
	{
		classes[i] = m_Priors[i];
	}

	for (size_t j = 0 ; j < class_values ; j++)
	{
		for (size_t i = 0 ; i < _no_of_atts ; i++)
		{
			classes[j] *= m_Counts[j][i][(int)_inputs[i]];
		}
	}
	Utils::Normalize(classes,class_values);
	return Utils::MaxIndex(classes,class_values);

}