#include "InfixToPostfix.h"

bool InfixToPostfix::precedance(Symbol* stackTop,Symbol* inputSymbol)
{
	if(stackTop->getType()==TYPELEFTBRACKET&&(inputSymbol->getType()==TYPEAND||inputSymbol->getType()==TYPEOR))
	{
		return false;
	}
	if ((stackTop->getType()==TYPEAND||stackTop->getType()==TYPEOR)&&inputSymbol->getType()==TYPELEFTBRACKET)
	{
		return false;
	}
	if ((stackTop->getType()==TYPEAND||stackTop->getType()==TYPEOR)&&inputSymbol->getType()==TYPERIGHTBRACKET)
	{
		return true;
	}
	if (stackTop->getType()==TYPELEFTBRACKET&&inputSymbol->getType()==TYPERIGHTBRACKET)
	{
		return false;
	}
	return false;
}

Expression* InfixToPostfix::makePostfixExpression(Expression* tokens)
{
	postfixExpression = new Expression();
	stack<Symbol*> opstack;

	for (int i=0;i<tokens->count();i++)
	{

		Symbol* inputSymbol = tokens->getSymbolAt(i);

		if(inputSymbol->getType()==TYPEOPERAND||inputSymbol->getType()==TYPENOT)
		{
			postfixExpression->addSymbolToExpression(inputSymbol);
		}
		else
		{
			while (!opstack.empty()&&precedance(opstack.top(),inputSymbol))
			{
				//cout<<"not empty stack"<<endl;
				Symbol* topSymbol = opstack.top();
				opstack.pop();
				postfixExpression->addSymbolToExpression(topSymbol);
			}

			//if (inputSymbol->getType()==TYPEAND||inputSymbol->getType()==TYPEOR||inputSymbol->getType()==TYPELEFTBRACKET)
			//{
			if (opstack.empty()||inputSymbol->getType()!=TYPERIGHTBRACKET)
			{
				opstack.push(inputSymbol);
			}
			else
			{
				opstack.pop();
			}
			//opstack.push(inputSymbol);
			//}
			//else if(inputSymbol->getType()==TYPERIGHTBRACKET)
			//{
			//	opstack.pop();
			//}
		}
	}

	while(!opstack.empty())
	{
		Symbol* topSymbol = opstack.top();
		opstack.pop();
		postfixExpression->addSymbolToExpression(topSymbol);
	}

	delete tokens;
	return postfixExpression;
}