#pragma once
#include <vector>
#include "distinctvalue.h"
#include <xstring>
/************************************************************************
*    Class  :Attribute	  
*    Author :Amila De Silva
*    Subj   :
* Represents an attribute for the classifier C4.5. A collection of "DistinctValue"s
* are wrapped using this class. Each attribute can have a seperate weight.
* In future this can be extended to support a seperate weight for each 
* instance in the attribute.
*    Version: 1
************************************************************************/
class Attribute
{
public:
	/***
	* Constructor
	*/
	Attribute(void);

	/***
	* Destructor
	*/
	~Attribute(void);
	
	/***
	* Initializes the attribute wrapping distinct values given by _values
	* The weight of the attribute is given by _weight
	*/
	Attribute(double _weight, vector<DistinctValue *> & _values);

	/***
	* Returns the weight of this attribute.
	*/
	double Weight() const { return m_weight; }

	/***
	* Sets the weight of this attribute.
	*/
	void Weight(double val) { m_weight = val; }

	/***
	* Returns the number of unique values held by this attribute.
	*/
	int numValues() const { return m_uniqueValNo; }

	/***
	* Sets the number of unique values held by this attribute.
	*/
	void numValues(int val) { m_uniqueValNo = val; }

	/***
	* Retrieves a bitmap by the position.
	* The bitmap returns a distinct value in an attribute.
	*/
	BitStreamInfo * bitStreamAt(size_t position);

	/***
	* Retrieves a distinct values corresponding to a particular position.	
	*/
	DistinctValue * distinctValueAt(size_t post);

	/***
	* Retrieves the collection of unique values held by the Attribute.
	*/
	vector<DistinctValue *> UniqueValues() const { return m_uniqueValues; }

	/***
	* Sets the set of Unique Values.
	*/
	void UniqueValues(vector<DistinctValue *> & val) { m_uniqueValues = val; }

	/***
	* Prints the contents of the Attribute to the console.
	*/
	void Print();

	/***
	* Retrieves the name of the attribute.
	*/
	std::string name() const { return m_name; }

	/***
	* Sets the name of the attribute.
	*/
	void name(std::string val) { m_name = val; }

	/**
	* Returns a value of a nominal or string attribute.  Returns an
	* empty string if the attribute is neither a string nor a nominal
	* attribute.
	*/
	string value(int valIndex);

private :
	/* No of unique bitmaps */
	int m_uniqueValNo;
	
	/* Vector holding all the unique values */
	vector<DistinctValue *> m_uniqueValues;
	
	/*weight of each attribute */
	double m_weight;	

	/*Name of the attribute */
	string m_name;	
}; 
