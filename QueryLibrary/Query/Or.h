#ifndef OR
#define OR

#include <iostream>

#include "Symbol.h"

using namespace std;

class Or:public Symbol
{
public:
	Or();

	Or(string name);

	~Or();

	string getName();

	string toString();

};
#endif