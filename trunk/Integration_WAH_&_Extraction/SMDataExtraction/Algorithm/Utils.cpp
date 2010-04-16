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