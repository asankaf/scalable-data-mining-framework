#pragma once
#include "entropybasedsplitcrit.h"
#include <float.h>
#include "distribution.h"


/************************************************************************
*    Class  :InfoGainSplitCrit	  
*    Author :Amila De Silva
*    Subj   :
* Class for computing the information gain for a given distribution.
*
*    Version: 1
************************************************************************/

class InfoGainSplitCrit :
	public EntropyBasedSplitCrit
{
public:
	/***
	* Constructor
	*/
	InfoGainSplitCrit(void);

	/***
	* Destructor
	*/
	virtual ~InfoGainSplitCrit(void);

	/**
	* This method is a straightforward implementation of the information
	* gain criterion for the given distribution.
	*/
	double splitCritValue(Distribution * bags);

	/**
	* This method computes the information gain in the same way 
	* C4.5 does.
	*/
	double splitCritValue(Distribution * bags, double totalNoInst); 

	/**
	* This method computes the information gain in the same way 
	* C4.5 does.
	*/
	double splitCritValue(Distribution * bags,double totalNoInst,double oldEnt);
};
