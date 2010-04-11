#pragma once
#include "distribution.h"

class ClassifierSplitModel
{
public:
	_declspec(dllexport) ClassifierSplitModel(void);
	_declspec(dllexport) virtual ~ClassifierSplitModel(void);

private:
	/** Distribution of class values. */  
	Distribution m_distribution;  

	/** Number of created subsets. */
	int m_numSubsets;
};
