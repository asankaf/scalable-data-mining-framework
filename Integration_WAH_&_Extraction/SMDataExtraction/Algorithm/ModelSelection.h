#pragma once
#include "classifiersplitmodel.h"
#include "datasource.h"
#include "BitStreamInfo.h"

/************************************************************************
*    Class  :ModelSelection	  
*    Author :Amila De Silva
*    Subj   :
* Abstract class for model selection criteria.
*    Version: 1
************************************************************************/
class ModelSelection
{
public:
	/***
	* Constructor
	*/
	_declspec(dllexport) ModelSelection(void);

	/***
	* Destructor
	*/
	_declspec(dllexport) virtual ~ModelSelection(void);

	/**
	* Selects a model for the given dataset.
	*
	*/
	_declspec(dllexport) virtual ClassifierSplitModel * selectModel(DataSource * _data,BitStreamInfo * _existence_bitmap) = 0;

	/**
	* Selects a model for the given train data using the given test data
	*	
	*/
	//_declspec(dllexport) virtual ClassifierSplitModel selectModel(WrapDataSource * _train, WrapDataSource * _test)  = 0;
	
};
