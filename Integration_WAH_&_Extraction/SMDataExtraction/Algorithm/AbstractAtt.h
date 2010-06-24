#pragma once
#include <vector>
#include "distinctvalue.h"
#include <xstring>
#include "bayesdistinct.h"
#include "BitStreamInfo.h"

/************************************************************************
*    Class  :AbstractAtt	  
*    Author :Amila De Silva
*    Subj   :
* Used for representing an attribute for the Naive Bayes Algorithm.
* This class can be extended to create Numeric and Nominal Attributes.
*    Version: 1
************************************************************************/
class AbstractAtt
{
public:
	/***
	* Constructor
	*/
	AbstractAtt(void);

	/***
	* virtual Destructor
	*/
	virtual ~AbstractAtt(void);

	/***
	* Gives the weight of a particular attribute.
	*/
	virtual double Weight() const =0; 

	/***
	* Sets the weight of a particular attribute.
	*/
	virtual void Weight(double val) = 0;

	/***
	* Gives the number of values held by this attribute.
	*/
	virtual int numValues() const = 0;

	/***
	* Sets the number of values held by this attribute.
	*/
	virtual void numValues(int val) = 0;

	/***
	* Retrieves a bitstream by position.
	*/
	virtual BitStreamInfo * bitStreamAt(size_t position) = 0;

	/***
	* Retrieves a DistinctValue by position.
	*/
	virtual BayesDistinct * distinctValueAt(size_t post) = 0;

	/***
	* Retrieves the collection of Uniquevalue held by this attribute.
	*/
	virtual vector<BayesDistinct *> UniqueValues() const = 0;

	/***
	* Sets the collection of Uniquevalue held by this attribute.
	*/
	virtual void UniqueValues(vector<BayesDistinct *> & val) = 0;

	/***
	* Prints the contents of this Attribute
	*/
	virtual void Print() = 0;

	/***
	* Retrieves the name of this attribute.
	*/
	virtual std::string name() const = 0;

	/***
	* Sets the name of this attribute.
	*/
	virtual void name(std::string val) = 0;

	/**
	* Returns a value of a nominal or string attribute.  Returns an
	* empty string if the attribute is neither a string nor a nominal
	* attribute.
	*/
	virtual string value(int valIndex)=0;
};
