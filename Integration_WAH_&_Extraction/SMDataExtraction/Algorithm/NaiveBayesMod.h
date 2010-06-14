#pragma once
#include "datasource.h"
#include <iostream>
#include "classifiertestsource.h"
#include "instances.h"
#include "Utils.h"
#include "wekainstances.h"
#include "Commons.h"
#include "smalgorithmexceptions.h"
using namespace std;
class NaiveBayesMod
{
public:
	_declspec(dllexport) NaiveBayesMod(void);

	_declspec(dllexport) ~NaiveBayesMod(void);

	/**Generates the classifier. */
	_declspec(dllexport) void buildClassifier(WekaInstances * instances,int class_index) throw (algorithm_exception);

	/**Prints the value of m_count*/
	_declspec(dllexport) void PrintCountArr(double *** array);

	/** Returns a description of the classifier.*/
	_declspec(dllexport) string toString();

	/**	Classifies the supplied test set using the model created*/
	_declspec(dllexport) void ClassifyInstances(ClassifierTestSource * _source);

protected:
	/** All the counts for nominal attributes. */
	double *** m_Counts;

	/** The means for numeric attributes. */
	double ** m_Means;

	/** The standard deviations for numeric attributes. */
	double ** m_Devs;

	/** The prior probabilities of the classes. */
	double * m_Priors;

	/** The instances used for training. */
	WekaInstances * m_Instances;
	//DataSource * m_Instances;
	//Instances * m_Instances;

	/** The class index used for current model. */
	int m_class_index;

private:
	/**Initializes the pointers to NULL*/
	void Init();

	/** Classifies a single instance*/
	int ClassifyInstance(double * _inputs,size_t _no_of_atts,WrapDataSource * _header);
	
	/** Print prior probabilities*/
	void PrintPriors();

	/** Print means corresponding to each class*/
	void PrintMeans();

	/** Print standard deviations corresponding to each class*/
	void PrintDevs();
};