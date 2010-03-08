/*
#include <iostream>
#include <string>

#include "Expression.h"
#include "Symbol.h"
#include "And.h"
#include "Or.h"
#include "Scanner.h"
#include "InfixToPostfix.h"

using namespace std;
void main()
{
	Expression* tokens;
	Scanner s;
	try
	{
		tokens = s.scan("(id[2,333]|id[2,333])&!id[2,333]");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	Expression* postfixExpression;
	InfixToPostfix itp;
	postfixExpression = itp.makePostfixExpression(tokens);
	postfixExpression->print();

	system("pause");
}*/
