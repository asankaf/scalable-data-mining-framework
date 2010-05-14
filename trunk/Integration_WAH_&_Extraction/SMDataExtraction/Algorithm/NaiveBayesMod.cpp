#include "StdAfx.h"
#include "NaiveBayesMod.h"

NaiveBayesMod::NaiveBayesMod(void)
{
	Init();
}

NaiveBayesMod::~NaiveBayesMod(void)
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

void NaiveBayesMod::Init()
{
	m_Instances = NULL;
	m_Counts = NULL;
	m_Devs = NULL;
	m_Means = NULL;
	m_Priors = NULL;
}

void NaiveBayesMod::buildClassifier(WrapDataSource * instances,int class_index)
{

	int attIndex = 0;
	double sum = 0;
	m_class_index = class_index;
	//m_Instances = new DataSource(instances,class_index);
	m_Instances = new WekaInstances(instances,class_index,true);
	//m_Instances = new WekaInstances(instances,class_index);
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
		if (m_Instances->isNumeric(attIndex))
		{
			for (int j = 0; j < m_Instances->numClasses(); j++)
			{

				m_Counts[j][attIndex] = new double[1];				
				m_Counts[j][attIndex][0] = 0;
			}
		}else
		{

			for (int j = 0; j < m_Instances->numClasses(); j++)
			{

				m_Counts[j][attIndex] = new double[m_Instances->noOfVlaues(attIndex)];
				for (size_t k =0 ; k < m_Instances->noOfVlaues(attIndex) ; k++)
				{
					m_Counts[j][attIndex][k] = 0;
				}
			}		 
		}

	}
	BitStreamInfo * class_value = NULL;
	// Compute counts and sums
	//PrintCountArr(m_Counts);
	for(size_t row_id = 0 ; row_id < m_Instances->Rows() ; row_id++ )
	{
		
		for(size_t a_Index = 0,attIndex = 0 ; a_Index < m_Instances->numAttributes();a_Index++)
		{
			if (a_Index != m_class_index)
{
	if (m_Instances->isNumeric(a_Index)) {
	
					m_Means[(int)m_Instances->classValueFor(row_id)][attIndex] +=
						m_Instances->instanceValue(row_id,a_Index);
					m_Counts[(int)m_Instances->classValueFor(row_id)][attIndex][0]++;	
					
				} else {
					m_Counts[(int)m_Instances->classValueFor(row_id)][attIndex]
					[(int)m_Instances->instanceValue(row_id,a_Index)]++;
				}
	attIndex++;
}
		}
		m_Priors[(int)m_Instances->classValueFor(row_id)]++;

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


	/*for(size_t attIndex = 0; attIndex < m_Instances->numAttributes()-1 ; attIndex++)
	{
		//Attribute  * attribute = m_Instances->attribute(attIndex);		
		AbstractAtt * attribute = m_Instances->attribute(attIndex);		
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
	}*/

	PrintCountArr(m_Counts);
}


string NaiveBayesMod::toString()
{
	/*
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
	
			for(size_t a_Index = 0,attIndex = 0; a_Index < m_Instances->numAttributes();a_Index++)
			{
				if (m_class_index != a_Index)
				{
					//Attribute * attribute = m_Instances->attribute(a_Index);
					AbstractAtt * attribute = m_Instances->attribute(a_Index);
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
		}
	
		return text;*/
	return "";

}

void NaiveBayesMod::PrintCountArr(double *** arr)
{
	for (int j = 0; j < m_Instances->numClasses(); j++)

	{			
		cout << "Class : " << m_Instances->classValueString(j) << endl;
		for (size_t attIndex = 0,a_index = 0 ; a_index < m_Instances->numAttributes() ; a_index++)		
		{
			if (a_index != m_class_index)
			{
			
			cout << "\t Attribute : " << m_Instances->nameOfAttribute(a_index)<<endl;
			//m_Counts[j][attIndex] = new double[m_Instances->attribute(attIndex)->numValues()];
			if (m_Instances->isNumeric(a_index))
			{
				cout << "\t\t"<<"Mean : "<<arr[j][attIndex][0] <<endl;
			
			}else
			{
				for (size_t k = 0 ; k < m_Instances->noOfVlaues(a_index);k++)
				{

					cout << "\t\t"<<k<<" : "<<arr[j][attIndex][k] <<endl;
				}
			}
			attIndex++;
			}
		}		 

	}
}

void NaiveBayesMod::ClassifyInstances( ClassifierTestSource * _source )
{
	/*
double * predict_vals = _source->Predicted_classes();
	for (size_t i = 0 ; i < _source->Rows(); i++)
	{
		predict_vals[i] = ClassifyInstance(_source->Data_source()[i],_source->Headers()->codedAttributes().size() -1,_source->Headers());
	}
*/
}

int NaiveBayesMod::ClassifyInstance(double * _inputs,size_t _no_of_atts,WrapDataSource * _header)
{/*
	
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
		return Utils::MaxIndex(classes,class_values);*/
	return 0;

}
