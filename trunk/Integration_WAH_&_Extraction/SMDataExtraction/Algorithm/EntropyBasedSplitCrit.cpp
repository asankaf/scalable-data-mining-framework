#include "StdAfx.h"
#include "EntropyBasedSplitCrit.h"

EntropyBasedSplitCrit::EntropyBasedSplitCrit(void)
{
}

EntropyBasedSplitCrit::~EntropyBasedSplitCrit(void)
{
}


double EntropyBasedSplitCrit::logFunc(double num)
{
	// Constant hard coded for efficiency reasons
	if (num < 1e-6)
		return 0;
	else
		return num*log10(num)/log2;
}

double EntropyBasedSplitCrit::oldEnt(Distribution * bags)
{
	double returnValue = 0;
	int j;

	for (j=0;j<bags->numClasses();j++)
		returnValue = returnValue+logFunc(bags->perClass(j));
	return logFunc(bags->Total())-returnValue; 
}

double EntropyBasedSplitCrit::newEnt(Distribution * bags)
{

	double returnValue = 0;
	int i,j;

	for (i=0;i<bags->numBags();i++)
	{
		for (j=0;j<bags->numClasses();j++)
		{
			returnValue = returnValue+logFunc(bags->perClassPerBag(i,j));
		}
		returnValue = returnValue-logFunc(bags->perBag(i));
	}
	return -returnValue;
}

/**
* Computes entropy after splitting without considering the
* class values.
*/
double EntropyBasedSplitCrit::splitEnt(Distribution * bags)
{

	double returnValue = 0;
	int i;

	for (i=0;i<bags->numBags();i++)
		returnValue = returnValue+logFunc(bags->perBag(i));
	return logFunc(bags->Total())-returnValue;
}