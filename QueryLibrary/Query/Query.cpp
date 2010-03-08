#include "Query.h"

Query::Query()
{

}

Query::Query(string query) throw(exception)
{
	scan(query);
	parse();
	makePostfixExpression();
}

Query::~Query()
{

}

void Query::scan(string query)
{
	Scanner s;
	tokens = s.scan(query);
}

void Query::parse()
{

}

void Query::makePostfixExpression()
{
	InfixToPostfix itp;
	postfixExpression = itp.makePostfixExpression(tokens);
}

Expression* Query::getPostfixExpression()
{
	return postfixExpression;
}