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

	std::string String() const { return m_string; }

	void String(std::string val) { m_string = val; }

private :
	/* No of unique bitmaps */
	int m_uniqueValNo;
	
	/* Vector holding all the unique values */
	vector<DistinctValue *> m_uniqueValues;
	
	/*weight of each attribute */
	double m_weight;	

	string m_string;	
}; 
