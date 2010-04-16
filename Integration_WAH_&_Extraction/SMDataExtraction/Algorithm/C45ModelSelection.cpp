#include "StdAfx.h"
#include "C45ModelSelection.h"
#include "c45split.h"
#include "nosplit.h"

C45ModelSelection::C45ModelSelection(void)
{
	m_minNoObj = 2;
	m_allData = NULL;
	m_useMDLcorrection = false;
	m_existence_bitmap = NULL;
}

C45ModelSelection::~C45ModelSelection(void)
{
	m_allData = NULL;
	m_existence_bitmap = NULL;
}

C45ModelSelection::C45ModelSelection(int minNoObj, DataSource * allData,BitStreamInfo * _existence_bitmap, bool useMDLcorrection) 
{
	m_minNoObj = minNoObj;
	m_allData = allData;
	m_useMDLcorrection = useMDLcorrection;
	m_existence_bitmap = _existence_bitmap;
}

ClassifierSplitModel * C45ModelSelection::selectModel(DataSource * _data,BitStreamInfo * _existence_bitmap)
{
	double minResult;
	double currentResult;
	C45Split ** currentModel;
	C45Split * bestModel = NULL;
	NoSplit * noSplitModel = NULL;
	double averageInfoGain = 0;
	int validModels = 0;
	boolean multiVal = true;
	Distribution * checkDistribution;
	//Attribute attribute;
	double sumOfWeights;
	int i;

		// Check if all Instances belong to one class or if not
		// enough Instances to split.
		checkDistribution = new Distribution(_data,_existence_bitmap);
		//checkDistribution->Print();
		noSplitModel = new NoSplit(checkDistribution);
		if (Utils::sm(checkDistribution->Total(),2*m_minNoObj) ||
			Utils::eq(checkDistribution->Total(),
			checkDistribution->perClass(checkDistribution->maxClass())))
			return noSplitModel;

		// Check if all attributes are nominal and have a 
		// lot of values.
		Attribute * attribute = NULL;
		if (m_allData != NULL) {			
			for (size_t i = 0 ; i < m_allData->Attributes().size() ; i++)			
			 {
				attribute = m_allData->Attributes()[i];
				if ((Utils::sm((double)attribute->numValues(),(0.3*(double)m_allData->numInstances()))))
				{
					multiVal = false;
						break;
				}
			}
		} 

		currentModel = new C45Split * [_data->numAttributes()];
		//replace this by the actual counting method performed by bitmap ORing.
		sumOfWeights = _existence_bitmap->Count() * _data->Weights()[0];

		// For each attribute.
		for (i = 0; i < _data->numAttributes(); i++){

			// Apart from class attribute.
			if (i != _data->ClassIndex()){

				// Get models for current attribute.
				currentModel[i] = new C45Split(i,m_minNoObj,sumOfWeights,m_useMDLcorrection);
				currentModel[i]->buildClassifier(_data,_existence_bitmap);

				// Check if useful split for current attribute
				// exists and check for enumerated attributes with 
				// a lot of values.
				if (currentModel[i]->checkModel())
					if (m_allData != NULL) {
						//Check if this means attributes in the current dataset or 
						//attributes in the whole dataset
						if ((multiVal || Utils::sm((double)_data->attribute(i)->numValues(),
							(0.3*(double)m_allData->numInstances()))))
						{
								averageInfoGain = averageInfoGain+currentModel[i]->infoGain();
								validModels++;
						} 
					} else {
						averageInfoGain = averageInfoGain+currentModel[i]->infoGain();
						validModels++;
					}
			}else
				currentModel[i] = NULL;
		}

		// Check if any useful split was found.
		if (validModels == 0)
		{
			return noSplitModel;
		}
		averageInfoGain = averageInfoGain/(double)validModels;

		// Find "best" attribute to split on.
		minResult = 0;
		for (i=0;i<_data->numAttributes();i++){
			if ((i != (_data)->ClassIndex()) && (currentModel[i]->checkModel()))

				// Use 1E-3 here to get a closer approximation to the original
				// implementation.
				if ((currentModel[i]->infoGain() >= (averageInfoGain - 1E-3)) &&
					Utils::gr(currentModel[i]->gainRatio(),minResult))
				{ 
						bestModel = currentModel[i];
						minResult = currentModel[i]->gainRatio();
				} 
		}

		// Check if useful split was found.
		if (Utils::eq(minResult,0))
			return noSplitModel;

		return bestModel;
	
	return NULL;
}

