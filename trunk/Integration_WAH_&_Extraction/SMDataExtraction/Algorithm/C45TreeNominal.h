#pragma once
#include "classifier.h"
#include "modelselection.h"
#include "classifiertree.h"
#include "ewah.h"
#include "boost/dynamic_bitset.hpp"
#include "WAHStructure.h"

using namespace CompressedStructure;
class C45TreeNominal :
	public Classifier
{
public:
	_declspec(dllexport) C45TreeNominal(void);
	_declspec(dllexport) virtual ~C45TreeNominal(void);
	/**
	* Generates a C45 classifier. Method overridden from Classifier. 
	*
	* @param _data set of instances serving as training data		
	*/
	_declspec(dllexport) virtual void buildClassifier(WrapDataSource * _data);

	_declspec(dllexport) string toString();

	/** Gets the graph of the tree*/
	_declspec(dllexport) string toGraph();

	/** Public Getters and Setters*/
	BitStreamInfo * Existence_map() const { return m_existence_map; }

	void Existence_map(BitStreamInfo * val) { m_existence_map = val; }	

	int ClassIndex() const { return m_classIndex; }

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

	/**Creates a dumy existence bitmap*/
	void CreateExistenceBitMap();
	
};
