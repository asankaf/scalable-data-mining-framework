/*
#include <iostream>
#include <string>
#include <exception>

#include "Expression.h"
#include "Symbol.h"
#include "And.h"
#include "Or.h"
#include "Scanner.h"

using namespace std;

void main()
{
	Scanner s;
	try
	{
		Expression* tokens = s.scan("(id[2,333])&!id[3,2]");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	
	s.print();

	system("pause");
}
*/
