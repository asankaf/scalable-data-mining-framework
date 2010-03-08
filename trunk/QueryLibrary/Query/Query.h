#ifndef QUERY
#define QUERY

#include <iostream>
#include <string>
#include <exception>

#include "Expression.h"
#include "Symbol.h"
#include "InfixToPostfix.h"
#include "Scanner.h"

using namespace std;
class Query
{
private:
	Expression* postfixExpression;
	Expression* tokens;

	void scan(string query);

	void parse();

	void makePostfixExpression();

public:
	Query();

	Query(string query) throw(exception);

	~Query();

	Expression* getPostfixExpression();
};
#endif