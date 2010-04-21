#pragma once
#include "BitStreamInfo.h"
#include <xstring>

class DistinctValue
{
public:
	DistinctValue(void);
	DistinctValue(BitStreamInfo * _value);
	DistinctValue(BitStreamInfo * _value,string _distinc_val);
	~DistinctValue(void);
	BitStreamInfo * Value() const { return m_value; }
	void Value(BitStreamInfo * val) { m_value = val; }
	std::string name() const { return m_stringVal; }
	void name(std::string val) { m_stringVal = val; }
	void Print();
	
private:
	BitStreamInfo * m_value;
	string m_stringVal;
};
