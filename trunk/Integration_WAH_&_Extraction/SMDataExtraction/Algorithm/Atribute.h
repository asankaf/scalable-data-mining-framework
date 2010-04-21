#pragma once
#include <vector>
#include "distinctvalue.h"
#include <xstring>

class Attribute
{
public:
	Attribute(void);
	~Attribute(void);

	Attribute(double _weight, vector<DistinctValue *> & _values);

	double Weight() const { return m_weight; }

	void Weight(double val) { m_weight = val; }

	int numValues() const { return m_uniqueValNo; }

	void numValues(int val) { m_uniqueValNo = val; }

	vector<DistinctValue *> UniqueValues() const { return m_uniqueValues; }

	void UniqueValues(vector<DistinctValue *> & val) { m_uniqueValues = val; }

	void Print();

	std::string name() const { return m_name; }

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
