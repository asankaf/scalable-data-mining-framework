#pragma once
#ifndef SCANNEREXCEPTION
#define	SCANNEREXCEPTION

#include <iostream>
#include <stdexcept>

using namespace std;

class OperandSyntaxException: public std::exception
{
	virtual const char* what() const throw();
};

class SymbolException: public std::exception
{
	virtual const char* what() const throw();
}; 

#endif