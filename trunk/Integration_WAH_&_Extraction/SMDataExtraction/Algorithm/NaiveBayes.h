#pragma once
#include "datasource.h"
#include <iostream>
#include "classifiertestsource.h"
#include "instances.h"

using namespace std;

class NaiveBayes
{
public:
	_declspec(dllexport) NaiveBayes(void);

	_declspec(dllexport) ~NaiveBayes(void);

	/**Generates the classifier. */
	_declspec(dllexport) void buildClassifier(WrapDataSource * instances,int class_index);

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
	//DataSource * m_Instances;
	Instances * m_Instances;

	/** The class index used for current model. */
	int m_class_index;

private:
	/**Initializes the pointers to NULL*/
	void Init();

	/** Classifies a single instance*/
	int ClassifyInstance(double * _inputs,size_t _no_of_atts,WrapDataSource * _header);
};
