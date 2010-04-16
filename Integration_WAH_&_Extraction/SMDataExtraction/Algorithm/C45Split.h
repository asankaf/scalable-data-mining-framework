#pragma once
#include "classifiersplitmodel.h"
#include "infogainsplitcrit.h"
#include "gainratiosplitcrit.h"
#include "datasource.h"
#include "BitStreamInfo.h"

class C45Split :
	public ClassifierSplitModel
{
public:
	C45Split(void);

	C45Split(int attIndex,int minNoObj, double sumOfWeights,bool useMDLcorrection);

	~C45Split(void);

	void buildClassifier(DataSource * _source, BitStreamInfo * _existence);

	/** Public getters and setters*/
	

	double infoGain() const { return m_infoGain; }

	void infoGain(double val) { m_infoGain = val; }

	double gainRatio() const { return m_gainRatio; }

	void gainRatio(double val) { m_gainRatio = val; }

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
// InfoGainSplitCrit * C45Split::infoGainCrit = new InfoGainSplitCrit();
// GainRatioSplitCrit * C45Split::gainRatioCrit = new GainRatioSplitCrit();