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
	* Normalizes the doubles in the array using the given value.	
	*/
	static void Normalize(double * doubles, double sum,size_t _length);

	/**
	* Returns index of maximum element in a given
	* array of doubles. First maximum is returned.	
	*/
	static int MaxIndex(double * doubles,size_t _length); 

	/**
	* Converts a double value to a string	
	*/
	static string toStringVal(double val);

	/**
	* Gives a string having a the given number of precissions	
	*/
	static string toStringVal(double val,int precission);

	/**
	* Normalizes the doubles in the array by their sum.	
	*/
	static void Utils::Normalize(double * doubles,size_t _length);

	/**
	* Rounds a double and converts it into a formatted decimal-justified String.
	* Trailing 0's are replaced with spaces.
	*/
	static string doubleToString(double value, int width,int afterDecimalPoint);

	/**
	* Rounds a double and converts it into String.
	*/
	static string doubleToString(double value, int afterDecimalPoint);

	/** Computes the sum of the elements of an array of doubles.*/
	static double sum(double* doubles,size_t length);

};
// const double Utils::log2 = log10((double)2);
// const double Utils::SMALL = 1e-6;
