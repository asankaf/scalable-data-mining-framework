#ifndef NOT
#define NOT

#include <iostream>

#include "Symbol.h"

using namespace std;

class Not:public Symbol
{
public:
	Not();

	Not(string name);

	~Not();

	string getName();

	string toString();

};
#endif