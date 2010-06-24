#pragma once
#include "distribution.h"
#include <iostream>
#include <xstring>

using namespace std;
/************************************************************************
*    Class  :Classifier	  
*    Author :Amila De Silva
*    Subj   :
* Abstract class for classification models that can be used 
* recursively to split the data.
*    Version: 1
************************************************************************/
class ClassifierSplitModel
{
public:
	/***
	* Constructor
	*/
	_declspec(dllexport) ClassifierSplitModel(void);

	/***
	* Destructor
	*/
	_declspec(dllexport) virtual ~ClassifierSplitModel(void);

	/**
	* Builds the classifier split model for the given set of instances.		
	*/
	_declspec(dllexport) virtual void buildClassifier(DataSource * _source, BitStreamInfo * _existence) = 0;

	/***
	* Returns the number of subsets created.
	*/
	_declspec(dllexport) int NumSubsets() const { return m_numSubsets; }

	/***
	* Sets the number of subsets to be created.
	*/
	_declspec(dllexport) void NumSubsets(int val) { m_numSubsets = val; }

	/**
	* Returns the distribution of class values induced by the model.
	*/
	_declspec(dllexport) Distribution * Dist() const { return m_distribution; }

	/**
	* Sets the distribution of class values induced by the model.
	*/
	_declspec(dllexport) void Dist(Distribution * val) { m_distribution = val; }
	
	/**
	* Prints left side of condition satisfied by instances in subset index.
	*/
	virtual string rightSide(int index,DataSource * data) = 0;

	/**
	* Checks if generated model is valid.
	*/
	bool checkModel();

	/**
	* Prints label for subset index of instances (eg class).	
	*/
	string dumpLabel(int index,DataSource * data);

	/**
	* Prints left side of condition satisfied by instances.
	*/
	virtual string leftSide(DataSource * data) = 0;

	_declspec(dllexport) void Print();

	/**
	* Returns the existence bitmaps generated for each split
	*/
	vector<BitStreamInfo *> Existence_maps() const { return m_existence_maps; }

	/**
	* Sets the existence bitmaps generated for each split
	*/
	void Existence_maps(vector<BitStreamInfo *>  & val) { m_existence_maps = val; }

	/**
	* Returns the distribution of class values induced by the model.
	*/
	Distribution * distribution() const { return m_distribution; }

	/**
	* Sets the distribution of class values induced by the model.
	*/
	void distribution(Distribution * val) { m_distribution = val; }

protected:
	/** Distribution of class values. */  
	Distribution * m_distribution;  	

	/** Number of created subsets. */
	int m_numSubsets;

	/** Holds the existence bitmaps corresponding to each value under current attribute*/
	vector<BitStreamInfo *> m_existence_maps;	
	
};
