#pragma once
#include <vector>
#include "atribute.h"
#include "WrapDataSource.h"
#include "distinctvalue.h"
#include "EncodedAttributeInfo.h"
#include "algoutils.h"
#include "AttributeType.h"


class DataSource
{
public:
	
	DataSource(void);

	DataSource(WrapDataSource * _wrapped,int _class_index);

	~DataSource(void);

	Attribute * attribute(int _index);

	/**
	* Returns the class attribute.	
	*/
	Attribute * classAttribute();

	/* Getters and setters for private members*/
	int numInstances() const { return m_row_count; }

	void numInstances(int val) { m_row_count = val; }

	int numAttributes() const { return m_no_of_attributes; }

	void numAttributes(int val) { m_no_of_attributes = val; }

	int numClasses() const { return m_numClasses; }

	void numClasses(int val) { m_numClasses = val; }

	int ClassIndex() const { return m_classIndex; }

	void ClassIndex(int val) { m_classIndex = val; }
	
	vector<Attribute *> Attributes() const { return m_attributes; }

	void Attributes(vector<Attribute *> val) { m_attributes = val; }

	vector<double> Weights() const { return m_weights; }

	void Weights(vector<double> val) { m_weights = val; }

	BitStreamInfo::vertical_bit_type Type() const { return m_type; }

	void Type(BitStreamInfo::vertical_bit_type val) { m_type = val; }

	void Print();

private:
	/*Holds no of attributes for this datasource*/
	int m_no_of_attributes;
	
	/* Holds no of rows in the dataset */
	int m_row_count;
	
	/*Holds the set of attributes for this data set */
	vector<Attribute *> m_attributes;
	
	/*Holds the of different number of classes */
	int m_numClasses;

	/* Index of the class attribute */
	int m_classIndex;

	/** Build Data source object from the instances*/
	void buildDataSource(WrapDataSource * _source) throw (invalid_parameter_exception); 

	/** Holds different weights assigned for each attribute*/
	vector<double> m_weights;

	/** Initializes weights for each attribute*/
	void initialiseWeights(int _attr_no);

	/** Obtains the set of distinct values*/
	vector<DistinctValue *> getDistinctValues(EncodedAttributeInfo * _attribute);

	/** Compression Type*/
	BitStreamInfo::vertical_bit_type m_type;
	
};
