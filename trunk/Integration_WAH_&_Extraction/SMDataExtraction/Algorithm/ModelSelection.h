#pragma once
#include "classifiersplitmodel.h"
#include "datasource.h"
#include "BitStreamInfo.h"

class ModelSelection
{
public:
	_declspec(dllexport) ModelSelection(void);
	_declspec(dllexport) virtual ~ModelSelection(void);

	/**
	* Selects a model for the given dataset.
	*
	* @exception Exception if model can't be selected
	*/
	_declspec(dllexport) virtual ClassifierSplitModel * selectModel(DataSource * _data,BitStreamInfo * _existence_bitmap) = 0;

	/**
	* Selects a model for the given train data using the given test data
	*
	* @exception Exception if model can't be selected
	*/
	//_declspec(dllexport) virtual ClassifierSplitModel selectModel(WrapDataSource * _train, WrapDataSource * _test)  = 0;
	
};
