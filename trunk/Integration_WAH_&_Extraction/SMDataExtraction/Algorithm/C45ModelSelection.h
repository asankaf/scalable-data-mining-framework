#pragma once
#include "datasource.h"
#include "ModelSelection.h"
#include "BitStreamInfo.h"
#include "classifiersplitmodel.h"
#include "Utils.h"

/************************************************************************
*    Class  :C45ModelSelection	  
*    Author :Amila De Silva
*    Subj   :
* Class for selecting a C4.5-type split for a given dataset.
*
*    Version: 1
************************************************************************/
class C45ModelSelection : public ModelSelection
{
public:
	/***
	* Constructor
	*/
	C45ModelSelection(void);
	
	/***
	* Destructor
	*/
	~C45ModelSelection(void);
	
	/**
	* Initializes the split selection method with the given parameters.
	*
	* minNoObj minimum number of instances that have to occur in at least two
	* subsets induced by split
	* allData FULL training dataset (necessary for
	* selection of split points).
	* useMDLcorrection whether to use MDL adjustement when
	* finding splits on numeric attributes
	*/
	_declspec(dllexport) C45ModelSelection(int minNoObj, 
		DataSource * allData,
		BitStreamInfo * _existence_bitmap,
		bool useMDLcorrection) ;

	/**
	* Selects C4.5-type split for the given dataset.
	*/
	_declspec(dllexport) ClassifierSplitModel * selectModel(DataSource * _data,BitStreamInfo * _existence_bitmap);

private:
	/** Minimum number of objects in interval. */
	int m_minNoObj;               

	/** Use MDL correction? */
	bool m_useMDLcorrection;         

	/** All the training data */
	DataSource * m_allData;

	/** Existence Bitmap */
	BitStreamInfo * m_existence_bitmap;
};
