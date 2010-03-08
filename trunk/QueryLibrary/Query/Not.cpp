#include "Not.h"

Not::Not()
{

}
Not::Not(string name)
{
	this->name=name;
	this->type=TYPENOT;
}
Not::~Not()
{

}
string Not::getName()
{
	return name;
}
string Not::toString()
{
	return name;
}