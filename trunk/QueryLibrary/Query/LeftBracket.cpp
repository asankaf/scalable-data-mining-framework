#include "LeftBracket.h"

LeftBracket::LeftBracket()
{

}
LeftBracket::LeftBracket(string name)
{
	this->name=name;
	this->type=TYPELEFTBRACKET;
}
LeftBracket::~LeftBracket()
{

}
string LeftBracket::getName()
{
	return name;
}
string LeftBracket::toString()
{
	return name;
}