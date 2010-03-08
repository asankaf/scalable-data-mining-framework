#include "ScannerException.h"

const char* OperandSyntaxException::what() const throw()
{
	return "Error in operand syntax: use operands such as id[1,2]";
}

const char* SymbolException::what() const throw()
{
	return "Symbol is not valid";
}
