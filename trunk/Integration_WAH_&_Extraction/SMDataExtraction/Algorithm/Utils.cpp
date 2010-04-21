#include "StdAfx.h"
#include "Utils.h"

Utils::~Utils(void)
{
}

bool Utils::gr(double a,double b)
{
	return (a-b > SMALL);
}

bool Utils::sm(double a,double b)
{
	return (b-a > SMALL);
}

bool Utils::smOrEq(double a,double b)
{
	return (a-b < SMALL);
}

bool Utils::eq(double a, double b)
{
	return (a - b < SMALL) && (b - a < SMALL); 
}

bool Utils::grOrEq(double a,double b)
{

	return (b-a < SMALL);
}

/**
* Rounds a double to the given number of decimal places.
*/
double Utils::roundDouble(double value,int afterDecimalPoint)
{
	double mask = pow(10.0, (double)afterDecimalPoint);
	unsigned long long rnd_val = (unsigned long long)value * mask;
	return (double)(rnd_val) / mask;
}

std::string Utils::toStringVal( int val )
{
	// This method is said to be inefficient
	return boost::lexical_cast<string>(val);
}

string Utils::toStringVal(double val)
{
	// This method is said to be inefficient
	return boost::lexical_cast<string>(val);
}

string Utils::toStringVal(double val, int precission)
{
	// This method is said to be inefficient
	return toStringVal(roundDouble(val,precission));
}