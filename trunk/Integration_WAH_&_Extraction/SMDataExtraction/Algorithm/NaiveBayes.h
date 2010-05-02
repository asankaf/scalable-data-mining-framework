#pragma once
#include "datasource.h"
#include <iostream>

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
	DataSource * m_Instances;

private:
	/**Initializes the pointers to NULL*/
	void Init();
};
