#ifndef OPERAND
#define OPERAND

#include <iostream>
#include <sstream>

#include "Symbol.h"

using namespace std;

class Operand:public Symbol
{
private:
	size_t attributeIndex;
	size_t bitStringIndex;
public:
	Operand();

	Operand(string name,size_t attributeIndex,size_t bitStringIndex);

	~Operand();

	string getName();

	size_t getAttributeIndex();

	size_t getbitStringIndex();

	string toString();

};
#endif