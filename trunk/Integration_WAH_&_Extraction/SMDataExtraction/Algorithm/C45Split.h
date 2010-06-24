#pragma once
#include "classifiersplitmodel.h"
#include "infogainsplitcrit.h"
#include "gainratiosplitcrit.h"
#include "datasource.h"
#include "BitStreamInfo.h"
#include <xstring>

using namespace std;

/************************************************************************
*    Class  :C45Split	  
*    Author :Amila De Silva
*    Subj   :
*	Class implementing a C4.5-type split on an attribute.
*
*    Version: 1
************************************************************************/
class C45Split :
	public ClassifierSplitModel

{
public:
	/***
	* Constructor
	*/
	C45Split(void);

	/**
	* Initializes the split model.
	*/
	C45Split(int attIndex,int minNoObj, double sumOfWeights,bool useMDLcorrection);

	/***
	* Destructor
	*/
	~C45Split(void);

	/**
	* Creates a C4.5-type split on the given data. Assumes that none of
	* the class values is missing.	
	*/
	void buildClassifier(DataSource * _source, BitStreamInfo * _existence);

	/**
	* Prints left side of condition..
	*/
	string leftSide(DataSource * data);

	/** Public getters and setters*/

	/**
	* Returns (C4.5-type) information gain for the generated split.
	*/
	double infoGain() const { return m_infoGain; }

	/**
	* Sets (C4.5-type) information gain for the generated split.
	*/
	void infoGain(double val) { m_infoGain = val; }

	/**
	* Returns (C4.5-type) gain ratio for the generated split.
	*/
	double gainRatio() const { return m_gainRatio; }

	/**
	* Sets (C4.5-type) gain ratio for the generated split.
	*/
	void gainRatio(double val) { m_gainRatio = val; }

	/**
	* Prints left side of condition satisfied by instances in subset index.
	*/
	string rightSide(int index,DataSource * data);

	
private:
	/** Desired number of branches. */
	int m_complexityIndex;  

	/** Attribute to split on. */
	int m_attIndex;         

	/** Minimum number of objects in a split.   */
	int m_minNoObj;         

	/** Use MDL correction? */
	bool m_useMDLcorrection;         

	/** Value of split point. */
	double m_splitPoint;   

	/** InfoGain of split. */ 
	double m_infoGain; 
	
	/** GainRatio of split.  */
	double m_gainRatio;  
	
	/** The sum of the weights of the instances. */
	double m_sumOfWeights;  

	/** Number of split points. */
	int m_index; 	

	/** Static reference to splitting criterion. */
	static InfoGainSplitCrit * infoGainCrit; 

	/** Static reference to splitting criterion. */
	static GainRatioSplitCrit * gainRatioCrit;

	/** Create existence maps for a given attribute */
	void createExistenceMaps(DataSource * _source, BitStreamInfo * _existence_map);
	
	/**
	* Creates split on enumerated attribute.	
	*/
	void handleEnumeratedAttribute(DataSource * trainInstances, BitStreamInfo * _existence_map);
};