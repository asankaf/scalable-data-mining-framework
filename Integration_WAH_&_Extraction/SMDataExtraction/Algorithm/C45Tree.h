#pragma once
#include "classifier.h"
#include "modelselection.h"
#include "classifiertree.h"

class C45Tree :
	public Classifier
{
public:
	_declspec(dllexport) C45Tree(void);
	_declspec(dllexport) virtual ~C45Tree(void);
	/**
	* Generates a C45 classifier. Method overridden from Classifier. 
	*
	* @param _data set of instances serving as training data		
	*/
	_declspec(dllexport) virtual void buildClassifier(WrapDataSource * _data);

	
	
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

};
