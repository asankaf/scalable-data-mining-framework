#pragma once
#include <math.h>
#include "boost/lexical_cast.hpp"
#include <xstring>

using namespace std;
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

	/**
	* Rounds a double to the given number of decimal places.	
	*/
	static double roundDouble(double value,int afterDecimalPoint);

	/**
	* Converts an integer to a string	
	*/
	static string toStringVal(int val);

	/**
	* Converts a double value to a string	
	*/
	static string toStringVal(double val);

	/**
	* Gives a string having a the given number of precissions	
	*/
	static string toStringVal(double val,int precission);
};
// const double Utils::log2 = log10((double)2);
// const double Utils::SMALL = 1e-6;
