#pragma once
#include <math.h>
class Utils
{
public:
	Utils(void){}
	
	~Utils(void);

	/** The natural logarithm of 2. */
	static double const log2;

	/** The small deviation allowed in double comparisons. */
	static double const SMALL;
	
	/**
	* Tests if a is greater than b.	
	*/
	static bool gr(double a,double b);

	/**
	* Tests if a is smaller than b.	
	*/
	static bool sm(double a,double b);

	/**
	* Tests if a is smaller or equal to b.
	*/
	static bool smOrEq(double a,double b);

	/**
	* Tests if a is equal to b.
	*/
	static bool eq(double a, double b);

	/**
	* Tests if a is greater or equal to b.	
	*/
	static bool grOrEq(double a,double b);
};
// const double Utils::log2 = log10((double)2);
// const double Utils::SMALL = 1e-6;
