#include "StdAfx.h"
#include "C45Split.h"

C45Split::C45Split(void)
{
	m_minNoObj = 2;
}

C45Split::~C45Split(void)
{
	//Deleting existence maps
	for (size_t i = 0 ; i < m_existence_maps.size() ; i++)
	{
		if (m_existence_maps[i] != NULL)
		{
			delete m_existence_maps[i];
		}		
	}
	m_existence_maps.clear();
}

string C45Split::leftSide(DataSource * data)
{
	return data->attribute(m_attIndex)->name();
}

C45Split::C45Split(int attIndex,int minNoObj, double sumOfWeights,bool useMDLcorrection)
{

	// Get index of attribute to split on.
	m_attIndex = attIndex;

	// Set minimum number of objects.
	m_minNoObj = minNoObj;

	// Set the sum of the weights
	m_sumOfWeights = sumOfWeights;

	// Whether to use the MDL correction for numeric attributes
	m_useMDLcorrection = useMDLcorrection;
}

void C45Split::buildClassifier(DataSource * source,BitStreamInfo * _existence_map) 
{
	// Initialize the remaining instance variables.
	m_numSubsets = 0;
	m_splitPoint = DBL_MAX;
	m_infoGain = 0;
	m_gainRatio = 0;

	// Different treatment for enumerated and numeric
	// attributes.
		m_complexityIndex = source->attribute(m_attIndex)->numValues();
		m_index = m_complexityIndex;
	handleEnumeratedAttribute(source,_existence_map);
	
}    
void C45Split::handleEnumeratedAttribute(DataSource * trainInstances, BitStreamInfo * _existence_map)
{
	//Instance instance;

	// Check if number of classes means all the classes in the original source or
	// the number of classes which have a count greater than 0;
	m_distribution = new Distribution(m_complexityIndex,trainInstances->NumClasses());

	createExistenceMaps(trainInstances,_existence_map);
	// Only Instances with known values are relevant.
	for (size_t i = 0 ; i < m_existence_maps.size() ; i++)
	{
		m_distribution->add(i,trainInstances,m_existence_maps[i]);
	}

	// Check if minimum number of Instances in at least two
	// subsets.
	if (m_distribution->check(m_minNoObj)) {
		m_numSubsets = m_complexityIndex;
		m_infoGain = infoGainCrit->splitCritValue(m_distribution,m_sumOfWeights);
		m_gainRatio = gainRatioCrit->splitCritValue(m_distribution,m_sumOfWeights,m_infoGain);
	}
}

void C45Split::createExistenceMaps(DataSource * _source, BitStreamInfo * _existence_map)
{
	vector<DistinctValue *> distincts = _source->attribute(m_attIndex)->UniqueValues();
	vector<BitStreamInfo *> existence_maps(distincts.size());
	
	for (size_t i = 0 ; i < existence_maps.size(); i++)
	{
		existence_maps[i] = *(_existence_map) & *(distincts[i]->Value());
	}
	m_existence_maps = existence_maps;
}

string C45Split::rightSide(int index,DataSource * data)
{
	string text;
	
		text.append(" = "+
		data->attribute(m_attIndex)->value(index));		
	return text;
}
