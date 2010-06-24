#pragma once
#include "classifiersplitmodel.h"

/************************************************************************
*    Class  :ModelSelection	  
*    Author :Amila De Silva
*    Subj   :
* Class implementing a "no-split"-split.
*    Version: 1
************************************************************************/
class NoSplit :
	public ClassifierSplitModel
{
public:
	/***
	* Constructor
	*/
	NoSplit(void);
	
	/**
	* Creates "no-split"-split for given distribution.
	*/
	_declspec(dllexport) NoSplit(Distribution * _dist);

	/***
	* Destructor
	*/
	~NoSplit(void);

	/**
	* Creates a "no-split"-split for a given set of instances.	
	*/
	_declspec(dllexport) void buildClassifier(DataSource * _source, BitStreamInfo * _existence);

	/**
	* Does nothing because no condition has to be satisfied.
	*/
	string leftSide(DataSource * data);

	/**
	* Does nothing because no condition has to be satisfied.
	*/
	string rightSide(int index,DataSource * data);
};
