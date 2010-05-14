#pragma once
#include "BitStreamInfo.h"
#include "bayesdistinct.h"
#include <vector>
#include <xstring>

using namespace std;
class NumericAttribute
{
public:
	NumericAttribute(void);
	~NumericAttribute(void);

	//AbstractAtt(double _weight, vector<BayesDistinct *> & _values) =0;

	virtual double Weight(){ return 0;}

	virtual void Weight(double val){}

	virtual int numValues(){return -1;}

	virtual void numValues(int val){}

	//virtual BitStreamInfo * bitStreamAt(size_t position){return NULL;}
	virtual BitStreamInfo * bitStreamAt(size_t position){return NULL;}

	virtual BayesDistinct * distinctValueAt(size_t post){return NULL;}

	virtual vector<BayesDistinct *> UniqueValues(){vector<BayesDistinct*> ret_val; return ret_val;}

	virtual void UniqueValues(vector<BayesDistinct *> & val){}

	virtual void Print(){}

	virtual std::string name(){return "";}

	virtual void name(std::string val){}

	/**
	* Returns a value of a nominal or string attribute.  Returns an
	* empty string if the attribute is neither a string nor a nominal
	* attribute.
	*/
	virtual string value(int valIndex){return "";}

private:
};
