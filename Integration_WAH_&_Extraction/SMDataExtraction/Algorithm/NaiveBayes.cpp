#include "StdAfx.h"
#include "NaiveBayes.h"
#include "algoutils.h"
#include "utils.h"
#include <time.h>
#include <iostream>
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
					delete m_Counts[attIndex][j];	
				}		 
				delete m_Counts[attIndex];
			}

			delete m_Counts;
		}
	
	if (m_Devs != NULL)
	{
		for (size_t i = 0 ; i < m_Instances->numClasses() ; i++)
		{
			delete m_Devs[i];
		}
		delete m_Devs;
	}
	if (m_Means != NULL)
	{
		for (size_t i  = 0 ; i < m_Instances->numClasses() ; i++)
		{
			delete m_Means[i];
		}
		delete m_Means;
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

	m_Instances = new DataSource(instances,class_index);
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
				m_Counts[j][attIndex] = new double[m_Instances->attribute(attIndex)->numValues()];
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
for(size_t class_index = 0 ; class_index < m_Instances->numClasses() ; class_index++ )
{
		class_value = m_Instances->attribute(m_Instances->ClassIndex())->bitStreamAt(class_index);
			for(size_t attIndex = 0 ; attIndex < m_Instances->numAttributes() -1;attIndex++)
			{
				Attribute * attribute =  m_Instances->attribute(attIndex);
				for (size_t att_vals = 0 ; att_vals < attribute->numValues() ; att_vals++) 
				{					
					m_Counts[class_index][attIndex][att_vals] = AlgoUtils::ANDCount(class_value,attribute->bitStreamAt(att_vals));		
					
				}
				
			}
			m_Priors[class_index] = class_value->Count();
		
	}


	
	/*// Compute means
	Enumeration enumAtts = instances.enumerateAttributes();
	attIndex = 0;
	while (enumAtts.hasMoreElements()) {
		Attribute attribute = (Attribute) enumAtts.nextElement();
		if (attribute.isNumeric()) {
			for (int j = 0; j < instances.numClasses(); j++) {
				if (m_Counts[j][attIndex][0] < 2) {
					throw new Exception("attribute " + attribute.name() +
						": less than two values for class " +
						instances.classAttribute().value(j));
				}
				m_Means[j][attIndex] /= m_Counts[j][attIndex][0];
			}
		}
		attIndex++;
	}    
*/
/*	// Compute standard deviations
	enumInsts = instances.enumerateInstances();
	while (enumInsts.hasMoreElements()) {
		Instance instance = 
			(Instance) enumInsts.nextElement();
		if (!instance.classIsMissing()) {
			enumAtts = instances.enumerateAttributes();
			attIndex = 0;
			while (enumAtts.hasMoreElements()) {
				Attribute attribute = (Attribute) enumAtts.nextElement();
				if (!instance.isMissing(attribute)) {
					if (attribute.isNumeric()) {
						m_Devs[(int)instance.classValue()][attIndex] +=
							(m_Means[(int)instance.classValue()][attIndex]-
							instance.value(attribute))*
							(m_Means[(int)instance.classValue()][attIndex]-
							instance.value(attribute));
					}
				}
				attIndex++;
			}
		}
	}
	*/
/*
	enumAtts = instances.enumerateAttributes();
	attIndex = 0;
	while (enumAtts.hasMoreElements()) {
		Attribute attribute = (Attribute) enumAtts.nextElement();
		if (attribute.isNumeric()) {
			for (int j = 0; j < instances.numClasses(); j++) {
				if (m_Devs[j][attIndex] <= 0) {
					throw new Exception("attribute " + attribute.name() +
						": standard deviation is 0 for class " +
						instances.classAttribute().value(j));
				}
				else {
					m_Devs[j][attIndex] /= m_Counts[j][attIndex][0] - 1;
					m_Devs[j][attIndex] = Math.sqrt(m_Devs[j][attIndex]);
				}
			}
		}
		attIndex++;
	} 
*/
	// Normalize counts
	

	for(size_t attIndex = 0; attIndex < m_Instances->numAttributes()-1 ; attIndex++)
	{
		Attribute  * attribute = m_Instances->attribute(attIndex);		
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

		for (int i = 0; i < m_Instances->numClasses(); i++)
		{
			text.append("\n\nClass " + m_Instances->classAttribute()->value(i) 
				+ ": P(C) = " 
				+ Utils::doubleToString(m_Priors[i], 10, 8)
				+ "\n\n");
					
			for(size_t attIndex = 0; attIndex < m_Instances->numAttributes()-1 ; attIndex++)
			{
				Attribute * attribute = m_Instances->attribute(attIndex);
				text.append("Attribute " + attribute->name() + "\n");
				
					for (int j = 0; j < attribute->numValues(); j++)
					{
						text.append(attribute->value(j) + "\t");
					}

					text.append("\n");

					for (int j = 0; j < attribute->numValues(); j++)
						text.append(Utils::doubleToString(m_Counts[i][attIndex][j], 10, 8)
						+ "\t");
				 
				text.append("\n\n");
				attIndex++;
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