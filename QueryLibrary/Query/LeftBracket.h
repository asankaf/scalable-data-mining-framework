#ifndef LEFTBRACKET
#define LEFTBRACKET

#include <iostream>

#include "Symbol.h"

using namespace std;

class LeftBracket:public Symbol
{
public:
	LeftBracket();

	LeftBracket(string name);

	~LeftBracket();

	string getName();

	string toString();

};
#endif