#include "RightBracket.h"

RightBracket::RightBracket()
{

}
RightBracket::RightBracket(string name)
{
	this->name=name;
	this->type=TYPERIGHTBRACKET;
}
RightBracket::~RightBracket()
{

}
string RightBracket::getName()
{
	return name;
}
string RightBracket::toString()
{
	return name;
}