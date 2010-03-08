#ifndef AND
#define AND

#include <iostream>

#include "Symbol.h"

using namespace std;

class And:public Symbol
{
public:
	And();

	And(string name);

	~And();

	string getName();

	string toString();

};
#endif