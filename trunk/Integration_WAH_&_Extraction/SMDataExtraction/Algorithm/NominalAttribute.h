#pragma once
#include <vector>
#include "distinctvalue.h"
#include <xstring>
#include "bayesdistinct.h"
#include "abstractatt.h"

class NominalAttribute : public AbstractAtt
{
public:
	NominalAttribute(void);
	~NominalAttribute(void);

	NominalAttribute(double _weight, vector<BayesDistinct *> & _values);

	double Weight() const { return m_weight; }

	void Weight(double val) { m_weight = val; }

	int numValues() const { return m_uniqueValNo; }

	void numValues(int val) { m_uniqueValNo = val; }

	BitStreamInfo * bitStreamAt(size_t position);

	BayesDistinct * distinctValueAt(size_t post);

	vector<BayesDistinct *> UniqueValues() const { return m_uniqueValues; }

	void UniqueValues(vector<BayesDistinct *> & val) { m_uniqueValues = val; }

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
	vector<BayesDistinct *> m_uniqueValues;

	/*weight of each attribute */
	double m_weight;	

	/*Name of the attribute */
	string m_name;
};
