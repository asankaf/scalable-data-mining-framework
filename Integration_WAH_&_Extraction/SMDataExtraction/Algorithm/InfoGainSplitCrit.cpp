#include "StdAfx.h"
#include "InfoGainSplitCrit.h"

InfoGainSplitCrit::InfoGainSplitCrit(void)
{
}

InfoGainSplitCrit::~InfoGainSplitCrit(void)
{
}


double InfoGainSplitCrit::splitCritValue(Distribution * bags)
{
	double numerator;

	numerator = oldEnt(bags)-newEnt(bags);

	// Splits with no gain are useless.
	if (Utils::eq(numerator,0))
		return DBL_MAX;

	// We take the reciprocal value because we want to minimize the
	// splitting criterion's value.
	return bags->Total()/numerator;
}

double InfoGainSplitCrit::splitCritValue(Distribution * bags, double totalNoInst) 
{
	double numerator = 0;
	double noUnknown = 0;
	double unknownRate = 0;
	int i = 0;

	noUnknown = totalNoInst-bags->Total();
	unknownRate = noUnknown/totalNoInst;
	numerator = (oldEnt(bags)-newEnt(bags));
	numerator = (1-unknownRate)*numerator;

	// Splits with no gain are useless.
	if (Utils::eq(numerator,0))
		return 0;

	return numerator/bags->Total();
}

double InfoGainSplitCrit::splitCritValue(Distribution * bags,double totalNoInst,double oldEnt)
{

	double numerator = 0;
	double noUnknown = 0;
	double unknownRate = 0;
	int i = 0;

	noUnknown = totalNoInst-bags->Total();
	unknownRate = noUnknown/totalNoInst;
	numerator = (oldEnt-newEnt(bags));
	numerator = (1-unknownRate)*numerator;

	// Splits with no gain are useless.
	if (Utils::eq(numerator,0))
		return 0;

	return numerator/bags->Total();
}

