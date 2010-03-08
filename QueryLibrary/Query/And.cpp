#include "And.h"

And::And()
{

}
And::And(string name)
{
	this->name=name;
	this->type=TYPEAND;
}
And::~And()
{

}
string And::getName()
{
	return name;
}
string And::toString()
{
	return name;
}