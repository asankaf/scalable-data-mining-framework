#ifndef SCANNEREXCEPTION
#define	SCANNEREXCEPTION

#include <iostream>
#include <exception>

using namespace std;

class OperandSyntaxException: public exception
{
	virtual const char* what() const throw();
};

class SymbolException: public exception
{
	virtual const char* what() const throw();
}; 

#endif