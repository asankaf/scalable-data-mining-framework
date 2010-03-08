#include "Operand.h"

Operand::Operand()
{

}
Operand::Operand(string name,size_t attributeIndex,size_t bitStringIndex)
{
	this->name=name;
	this->attributeIndex=attributeIndex;
	this->bitStringIndex=bitStringIndex;
	this->type=TYPEOPERAND;
}
Operand::~Operand()
{

}
string Operand::getName()
{
	return name;
}
size_t Operand::getAttributeIndex()
{
	return attributeIndex;
}
size_t Operand::getbitStringIndex()
{
	return bitStringIndex;
}
string Operand::toString()
{
	stringstream atind;
	atind << attributeIndex;
	stringstream bits;
	bits << bitStringIndex;
	return name+" "+atind.str()+" "+bits.str();
}