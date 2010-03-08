#ifndef INFIXTOPOSTFIX
#define INFIXTOPOSTFIX

#include <iostream>
#include <stack>

#include "Expression.h"
#include "Symbol.h"
#include "And.h"

using namespace std;

class InfixToPostfix
{
private:
	Expression* postfixExpression;

	bool precedance(Symbol* stackTop,Symbol* inputSymbol);

public:
	Expression* makePostfixExpression(Expression* tokens);
};
#endif