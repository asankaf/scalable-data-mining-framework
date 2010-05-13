#pragma once
#include "BitStreamInfo.h"
#include <xstring>

class BayesDistinct
	{
	public:
		BayesDistinct(void);
		BayesDistinct(BitStreamInfo * _value);
		BayesDistinct(BitStreamInfo * _value,string _distinc_val);
		~BayesDistinct(void);
		BitStreamInfo * Value() const { return m_value; }
		void Value(BitStreamInfo * val) { m_value = val; }
		std::string name() const { return m_stringVal; }
		void name(std::string val) { m_stringVal = val; }
		void Print();

	private:
		BitStreamInfo * m_value;
		string m_stringVal;

};
