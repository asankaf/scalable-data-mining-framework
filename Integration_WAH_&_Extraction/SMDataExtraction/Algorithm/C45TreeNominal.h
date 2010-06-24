#pragma once
#include "classifier.h"
#include "modelselection.h"
#include "classifiertree.h"
#include "ewah.h"
#include "boost/dynamic_bitset.hpp"
#include "WAHStructure.h"
#include "smalgorithmexceptions.h"

using namespace CompressedStructure;

/************************************************************************
*    Class  :C45TreeNominal	  
*    Author :Amila De Silva
*    Subj   :
*	Class for generating a pruned or unpruned C4.5 decision tree.
*
*    Version: 1
************************************************************************/
class C45TreeNominal :
	public Classifier
{
public:
	/***
	* Constructor
	*/
	_declspec(dllexport) C45TreeNominal(void);

	/***
	* Destructor
	*/
	_declspec(dllexport) virtual ~C45TreeNominal(void);

	/**
	* Generates a C45 classifier. Method overridden from Classifier. 
	*
	* _data set of instances serving as training data		
	*/
	_declspec(dllexport) virtual void buildClassifier(WrapDataSource * _data) throw (invalid_parameter_exception);

	/***
	* Gets the string representation of the tree*
	*/
	_declspec(dllexport) string toString();

	/***
	* Gets the graph of the tree*
	*/
	_declspec(dllexport) string toGraph();

	/** Public Getters and Setters*/
	/***
	* Returns the existence bitmap for the current tree.
	*/
	BitStreamInfo * Existence_map() const { return m_existence_map; }

	/***
	* Sets the existence bitmap for the current tree.
	*/
	void Existence_map(BitStreamInfo * val) { m_existence_map = val; }	

	/***
	* Returns the class index to be used.
	*/
	int ClassIndex() const { return m_classIndex; }

	/***
	* Sets the class index to be used.
	*/
	void ClassIndex(int val) { m_classIndex = val; }
	
	private:
	/** The decision tree */
	ClassifierTree * m_root;

	/** Unpruned tree */
	boolean m_unpruned;

	/** Collapse tree */
	boolean m_collapseTree;

	/** Confidence level */
	float m_CF;

	/** Minimum number of instances */
	int m_minNumObj;

	/** Use MDL correction? */
	bool m_useMDLcorrection;         

	/** Binary splits on nominal attributes? */
	bool m_binarySplits;

	/** Subtree raising to be performed? */
	bool m_subtreeRaising;

	/** Cleanup after the tree has been built. */
	bool m_noCleanup;

	/** Stores the generated data source*/
	DataSource * m_source;	

	/** Existence bitmap for the current source*/
	BitStreamInfo * m_existence_map;

	/** Class index of the Data source*/
	int m_classIndex;	

	/***
	* Creates a dummy existence bitmap
	*/
	void CreateExistenceBitMap() throw(empty_data_source_exception);
	
};
