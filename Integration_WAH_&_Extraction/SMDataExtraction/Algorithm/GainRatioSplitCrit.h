#pragma once
#include "entropybasedsplitcrit.h"
#include <float.h>

class GainRatioSplitCrit :
	public EntropyBasedSplitCrit
{
public:
	GainRatioSplitCrit(void);
	~GainRatioSplitCrit(void);

	/**
	* This method is a straightforward implementation of the gain
	* ratio criterion for the given distribution.
	*/
	double splitCritValue(Distribution * bags);

	/**
	* This method computes the gain ratio in the same way C4.5 does.
	*/
	double splitCritValue(Distribution * bags, double totalnoInst,double numerator);

	/**
	* Help method for computing the split entropy.
	*/
	double splitEnt(Distribution * bags,double totalnoInst);
};
