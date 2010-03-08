#ifndef SCANNER
#define SCANNER

#include <iostream>
#include <boost/regex.hpp>

#include "Expression.h"
#include "Symbol.h"
#include "And.h"
#include "Or.h"
#include "Not.h"
#include "Operand.h"
#include "LeftBracket.h"
#include "RightBracket.h"
#include "ScannerException.h"

using namespace std;
//using namespace boost;

class Scanner
{
private:
	Expression* tokens;
	string operand;

	void cleanOperand()
	{
		operand.clear();
	}

	void addOperand(string operand)
	{
		if (!operand.empty())
		{
			validateOperand(operand);
			size_t index1=operand.find_first_of("[");
			string dataSourceID = operand.substr (0,index1);
			size_t index2=operand.find_first_of(",");
			size_t attributeIndex = atoi((operand.substr (index1+1,index2-index1-1)).c_str());
			size_t index3=operand.find_first_of("]");
			size_t bitStringIndex = atoi((operand.substr (index2+1,index3-index2-1)).c_str());
			//cout<<dataSourceID<<endl; 
			//cout<<attributeIndex<<endl; 
			//cout<<bitStringIndex<<endl; 
			tokens->addSymbolToExpression(new Operand(dataSourceID,attributeIndex,bitStringIndex));
			//cout<<operand<<endl;
			cleanOperand();
		}
	}

	void validateOperand(string operand)
	{
		boost::regex regExp("([a-z]|[A-Z])+[\[][0-9]+[,][0-9]+[\]]");
		if(regex_match(operand,regExp))
		{
			//cout<<"match:"<<operand<<endl;
			return;
		}
		else
		{
			OperandSyntaxException oe;
			throw oe;
		}
	}
public:
	~Scanner()
	{

	}
	Expression* scan(string str)
	{
		tokens = new Expression();
		for (int i=0;i<str.size();i++)
		{
			char character = str[i];
			if(isspace(character))
			{
				continue;
			}
			else if(isalpha(character)|character == '['|character == ']'|character == ','|isdigit(character))
			{
				//cout<<character<<endl;
				operand.push_back(character);
			}
			else if(character == '&')
			{
				//cout<<character<<endl;
				addOperand(operand);
				tokens->addSymbolToExpression(new And("&_token"));
			}
			else if (character == '|')
			{
				//cout<<character<<endl;
				addOperand(operand);
				tokens->addSymbolToExpression(new Or("|_token"));
			}
			else if (character == '!')
			{
				//cout<<character<<endl;
				addOperand(operand);
				tokens->addSymbolToExpression(new Not("!_token"));
			}
			else if (character == '(')
			{
				//cout<<character<<endl;
				addOperand(operand);
				tokens->addSymbolToExpression(new LeftBracket("(_token"));
			}
			else if (character == ')')
			{
				//cout<<character<<endl;
				addOperand(operand);
				tokens->addSymbolToExpression(new RightBracket(")_token"));
			}
			else
			{
				SymbolException se;
				throw se;
			}
		}
		addOperand(operand);
		return tokens;
	}

	void print()
	{
		tokens->print();
	}
};
#endif