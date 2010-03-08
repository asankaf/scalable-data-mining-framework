#ifndef RIGHTBRACKET
#define RIGHTBRACKET

#include <iostream>

#include "Symbol.h"

using namespace std;

class RightBracket:public Symbol
{
public:
	RightBracket();

	RightBracket(string name);

	~RightBracket();

	string getName();

	string toString();

};
#endif