#pragma once
#include "BitStreamInfo.h"
#include <xstring>

/************************************************************************
*    Class  :DistinctValue	  
*    Author :Amila De Silva
*    Subj   :
* Class for representing the occurances of a distinct value in a nominal attribute.
* This class simply wraps a BitStreamInfo objects and provide certain
* additional functionalities.
*    Version: 1
************************************************************************/
class DistinctValue
{
public:
	/***
	* Constructor
	*/
	DistinctValue(void);

	/***
	* Initializes the DistinctValue with the given Bitstream. 
	*/
	DistinctValue(BitStreamInfo * _value);

	/***
	* Initializes the DistinctValue with the given Bitstream and the 
	* attribute name.
	*/
	DistinctValue(BitStreamInfo * _value,string _distinc_val);
	
	/***
	* Destructor
	*/
	~DistinctValue(void);

	/***
	* Returns the value of the Bitstream
	*/
	BitStreamInfo * Value() const { return m_value; }

	/***
	* Sets the value of the Bitstream
	*/
	void Value(BitStreamInfo * val) { m_value = val; }

	/***
	* Returns the name of the DistinctValue
	*/
	std::string name() const { return m_stringVal; }
	
	/***
	* Sets the name of the DistinctValue
	*/
	void name(std::string val) { m_stringVal = val; }

	/***
	* Prints the contents of the DistinctValue . 
	*/
	void Print();
	
private:

	/***
	* Holds the contents of the DistinctValue
	*/
	BitStreamInfo * m_value;

	/***
	* Holds the string representation of the value.
	*/
	string m_stringVal;
};
