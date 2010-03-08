#ifndef EXPRESSION
#define EXPRESSION

#include <iostream>
#include <vector>

#include "Symbol.h"

using namespace std;

class Expression
{
private:
	vector<Symbol*> expression;

public:
	Expression();

	Expression(Symbol* symbol);

	~Expression();

	void addSymbolToExpression(Symbol* symbol);

	Symbol* getSymbolAt(size_t position);

	size_t count();

	void print();
};
#endif