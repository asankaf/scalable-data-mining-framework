#pragma once
#include "distribution.h"

class SplitCriterion
{
public:
	SplitCriterion(void);
	virtual ~SplitCriterion(void);

	/**
	* Computes result of splitting criterion for given distribution.		
	*/
	_declspec(dllexport) virtual double splitCritValue(Distribution * bags){	return 0;}

	/**
	* Computes result of splitting criterion for given training and
	* test distributions.	
	*/
	_declspec(dllexport) virtual double splitCritValue(Distribution train, Distribution test){	return 0;}

	/**
	* Computes result of splitting criterion for given training and
	* test distributions and given number of classes.	
	*/
	_declspec(dllexport) virtual double splitCritValue(Distribution * _train, Distribution * test,int noClassesDefault){	return 0;}

	/**
	* Computes result of splitting criterion for given training and
	* test distributions and given default distribution.	
	*/
	_declspec(dllexport) virtual double splitCritValue(Distribution * train, Distribution * test,Distribution * defC){return 0;}
};
