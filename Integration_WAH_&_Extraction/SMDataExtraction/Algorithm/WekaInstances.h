#pragma once
#include <map>
#include <vector>
#include <xstring>
#include "EncodedAttributeInfo.h"
#include "WrapDataSource.h"

class WekaInstances
{
public:
	_declspec(dllexport) WekaInstances(void);

	_declspec(dllexport) WekaInstances(WrapDataSource * _source,int _clas_index,bool _delete_atts = false);

	_declspec(dllexport) ~WekaInstances(void);

	double ** Data_source() const { return m_data_source; }

	void Data_source(double ** val) { m_data_source = val; }

	WrapDataSource * Headers() const { return m_headers; }

	void Headers(WrapDataSource * val) { m_headers = val; }

	int numClasses() const { return m_num_classes; }

	void numClasses(int val) { m_num_classes = val; }

	int numAttributes()const {return m_attributes;}

	bool isNumeric(int _att_index){return m_numeric_nominal[_att_index];}

	/** Gives the number of unique values of a nominal attribute*/
	int noOfVlaues(int _att_index){return m_distinct_vals[_att_index].size();}

	double classValueFor(int _row_no){return m_data_source[_row_no][m_class_index];}

	int Rows() const { return m_rows; }
	void Rows(int val) { m_rows = val; }

	// Returns the cell value indicated by _row_id and _att_id
	double instanceValue(int _row_id,int _att_id){	return m_data_source[_row_id][_att_id];}

	//Gives the string representation of the value for a class
	string classValueString(int _index){return m_class_values[_index];}

	//Gives the name of the instance
	string nameOfAttribute(int _att_index){return m_headers->codedAttributes()[_att_index]->attributeName();}
	
private:
	void init();

	/** Holds the decoded data. */
	double ** m_data_source;
	
	/** Keeps the attribute info. */
	WrapDataSource * m_headers;
	
	/** Creates the set of Dynamic Bitsets out of bitstreams */
	void CreateDynamicBitSets(EncodedAttributeInfo * _attribute,vector<dynamic_bitset<>> & _bit_sets);

	/** Test Method*/
	void PrintSource();	

	/** Convert the attribute and write it to the array*/
	void ConvertAttributeTo(EncodedAttributeInfo * _attribute,double ** _values,int _att_index);

	/** Delete Bitstreams in the current attribute*/
	void ClearBitStreams(EncodedAttributeInfo * _attribute);

	/** Gives the number of rows*/
	int m_rows ;
	
	/** Gives the number of attributes*/
	int m_attributes;

	int m_num_classes;
	
	int m_class_index;

	vector<string> m_class_values; 

	/**Store the numeric/nominal type of an attribute 1 for numeric 0 for nominal*/
	bool * m_numeric_nominal;

	/** Keeps the distinct values for each Nominal attribute*/
	map<int,vector<string>> m_distinct_vals;

	// Gives the numeric nominal type of an attribute
	bool AttributeTypeSelector(EncodedAttributeInfo * _attribute);


};
