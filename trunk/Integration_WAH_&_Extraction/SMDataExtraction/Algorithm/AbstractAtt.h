#pragma once
#include <vector>
#include "distinctvalue.h"
#include <xstring>
#include "bayesdistinct.h"
#include "BitStreamInfo.h"

class AbstractAtt
{
public:
	AbstractAtt(void);
	virtual ~AbstractAtt(void);

	//AbstractAtt(double _weight, vector<BayesDistinct *> & _values) =0;

	virtual double Weight() const =0; 

	virtual void Weight(double val) = 0;

	virtual int numValues() const = 0;

	virtual void numValues(int val) = 0;

	//virtual BitStreamInfo * bitStreamAt(size_t position){return NULL;}
	virtual BitStreamInfo * bitStreamAt(size_t position) = 0;

	virtual BayesDistinct * distinctValueAt(size_t post) = 0;

	virtual vector<BayesDistinct *> UniqueValues() const = 0;

	virtual void UniqueValues(vector<BayesDistinct *> & val) = 0;

	virtual void Print() = 0;

	virtual std::string name() const = 0;

	virtual void name(std::string val) = 0;

	/**
	* Returns a value of a nominal or string attribute.  Returns an
	* empty string if the attribute is neither a string nor a nominal
	* attribute.
	*/
	virtual string value(int valIndex)=0;
};
