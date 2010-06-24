#pragma once
#include "splitcriterion.h"
#include <math.h>
class EntropyBasedSplitCrit :
	public SplitCriterion
{
public:
	EntropyBasedSplitCrit(void);
	virtual ~EntropyBasedSplitCrit(void);

	/**
	* Help method for computing entropy.
	*/
	virtual double logFunc(double num);

	/**
	* Computes entropy of distribution before splitting.
	*/
	virtual double oldEnt(Distribution * bags);

	/**
	* Computes entropy of distribution after splitting.
	*/
	virtual double newEnt(Distribution * bags);

	/**
	* Computes entropy after splitting without considering the
	* class values.
	*/
	virtual double splitEnt(Distribution * bags);

	/** The log of 2. */
protected:
	 static double log2 ;
};

