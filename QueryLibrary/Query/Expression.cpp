#include "Expression.h"

Expression::Expression()
{

}

Expression::Expression(Symbol* symbol)
{
	expression.push_back(symbol);
}

Expression::~Expression()
{

}

void Expression::addSymbolToExpression(Symbol* symbol)
{
	expression.push_back(symbol);
}

Symbol* Expression::getSymbolAt(size_t position)
{
	return expression[position];
}

size_t Expression::count(){
	return expression.size();
}

void Expression::print()
{
	for (int i=0;i<expression.size();i++)
	{
		//cout<<expression[i]->toString()<<endl;
	}
}