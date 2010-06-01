#pragma once
#include "BitStreamInfo.h"
#include "bayesdistinct.h"
#include <vector>
#include <xstring>
#include "abstractatt.h"
#include "EncodedAttributeInfo.h"

using namespace std;
class NumericAttribute :public AbstractAtt
{
public:
	
	~NumericAttribute(void);

	//AbstractAtt(double _weight, vector<BayesDistinct *> & _values) =0;
	NumericAttribute(double _weight,AbstractAtt * _class_value, EncodedAttributeInfo * _attribute);

	virtual double Weight() const { return m_weight;}

	virtual void Weight(double val){m_weight = val;}

	virtual int numValues()const {return -1;}

	virtual void numValues(int val){}

	//virtual BitStreamInfo * bitStreamAt(size_t position){return NULL;}
	virtual BitStreamInfo * bitStreamAt(size_t position){return NULL;}

	virtual BayesDistinct * distinctValueAt(size_t post){return NULL;}

	virtual vector<BayesDistinct *> UniqueValues()const {vector<BayesDistinct*> ret_val; return ret_val;}

	virtual void UniqueValues(vector<BayesDistinct *> & val) {}

	virtual void Print(){}

	virtual std::string name() const {return m_name;}

	virtual void name(std::string val){ m_name = val; }

	/**
	* Returns a value of a nominal or string attribute.  Returns an
	* empty string if the attribute is neither a string nor a nominal
	* attribute.
	*/
	virtual string value(int valIndex){return "";}

	/** Returns the mean for a set of values occurring with a class attribute.*/
	double GetMeanValue(size_t _class_value);

	/** Returns the standard deviation for a set of values occurring with a class attribute.*/
	double GetStandardDeviation(size_t _class_value);

private:
	double * m_means;
	double * m_deviations;

	/*weight of each attribute */
	double m_weight;	

	/*Name of the attribute */
	string m_name;
};
