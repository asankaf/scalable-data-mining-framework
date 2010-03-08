#include "Or.h"

Or::Or()
{

}
Or::Or(string name)
{
	this->name=name;
	this->type=TYPEOR;
}
Or::~Or()
{

}
string Or::getName()
{
	return name;
}
string Or::toString()
{
	return name;
}