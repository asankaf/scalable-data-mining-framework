#include "StdAfx.h"
#include "GainRatioSplitCrit.h"

GainRatioSplitCrit::GainRatioSplitCrit(void)
{
}

GainRatioSplitCrit::~GainRatioSplitCrit(void)
{
}

double GainRatioSplitCrit::splitCritValue(Distribution * bags)
{
	double numerator = 0;
	double denumerator = 0;

	numerator = oldEnt(bags)-newEnt(bags);

	// Splits with no gain are useless.
	if (Utils::eq(numerator,0))
	{
		return DBL_MAX;
	}	
	denumerator = EntropyBasedSplitCrit::splitEnt(bags);

	// Test if split is trivial.
	if (Utils::eq(denumerator,0))
	{
		return DBL_MAX;
	}
	//  We take the reciprocal value because we want to minimize the
	// splitting criterion's value.
	return denumerator/numerator;
}

double GainRatioSplitCrit::splitCritValue(Distribution * bags, double totalnoInst,double numerator)
{
	double denumerator = 0;
	double noUnknown = 0;
	double unknownRate = 0;
	int i = 0;

	// Compute split info.
	denumerator = splitEnt(bags,totalnoInst);

	// Test if split is trivial.
	if (Utils::eq(denumerator,0))
	{
		return 0;  
	}
	denumerator = denumerator/totalnoInst;

	return numerator/denumerator;
}

double GainRatioSplitCrit::splitEnt(Distribution * bags,double totalnoInst)
{
	double returnValue = 0;
	double noUnknown = 0;
	int i = 0;

	noUnknown = totalnoInst-bags->Total();
	if (Utils::gr(bags->Total(),0))
	{
		for (i=0;i<bags->numBags();i++)
			returnValue = returnValue - logFunc(bags->perBag(i));
		returnValue = returnValue-logFunc(noUnknown);
		returnValue = returnValue+logFunc(totalnoInst);
	}
	return returnValue;
}