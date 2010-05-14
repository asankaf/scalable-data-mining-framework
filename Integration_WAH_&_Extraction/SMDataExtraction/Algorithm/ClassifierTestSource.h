#pragma once
#include "WrapDataSource.h"
#include "EncodedAttributeInfo.h"
#include "AttributeType.h"
#include <vector>
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <xstring>

using namespace std;
using namespace boost;
class ClassifierTestSource
{
public:
	_declspec(dllexport) ClassifierTestSource(void);

	_declspec(dllexport) ClassifierTestSource(WrapDataSource * _source,int _class_index,bool _delete_atts = false);	

	_declspec(dllexport) ~ClassifierTestSource(void);

	/**Public Getters and setters**/
	_declspec(dllexport) double ** Data_source() const { return m_data_source; }

	_declspec(dllexport) void Data_source(double ** val) { m_data_source = val; }

	_declspec(dllexport) WrapDataSource * Headers() const { return m_headers; }

	_declspec(dllexport) void Headers(WrapDataSource * val) { m_headers = val; }

	_declspec(dllexport) int Class_index() const { return m_class_index; }

	_declspec(dllexport) void Class_index(int val) { m_class_index = val; }

	_declspec(dllexport) double * Original_classes() const { return m_original_classes; }

	_declspec(dllexport) void Original_classes(double * val) { m_original_classes = val; }

	_declspec(dllexport) double * Predicted_classes() const { return m_predicted_classes; }

	_declspec(dllexport) void Predicted_classes(double * val) { m_predicted_classes = val; }

	_declspec(dllexport) int Rows() const { return m_rows; }

	_declspec(dllexport) void Rows(int val) { m_rows = val; }

	_declspec(dllexport) size_t Number_of_classes() const { return m_number_of_classes; }

	_declspec(dllexport) void Number_of_classes(size_t val) { m_number_of_classes = val; }

	_declspec(dllexport) string GetConfusionString();

	/**Constructs the confusion matrix*/
	_declspec(dllexport) void BuildConfusionMatrix();

	/** Common mehtod to print a double array*/
	_declspec(dllexport) void Print(double * _array,int _length);

	/**Prints the confusion matrix*/
	_declspec(dllexport) void PrintConfusion(double ** _array,int _length);
private:
	
	/**Initialises variables to NULL values */
	void init();

	/** Creates the set of Dynamic Bitsets out of bitstreams */
	void CreateDynamicBitSets(EncodedAttributeInfo * _attribute,vector<dynamic_bitset<>> & _bit_sets);

	/** Test Method*/
	void PrintSource();	

	/** Convert the attribute and write it to the array*/
	void ConvertAttributeTo(EncodedAttributeInfo * _attribute,double * _values);

	void ConvertAttributeTo(EncodedAttributeInfo * _attribute,double ** _values,int _att_index);

	/** Delete Bitstreams in the current attribute*/
	void ClearBitStreams(EncodedAttributeInfo * _attribute);

	/** Holds the decoded data. */
	double ** m_data_source;
	
	/** Keeps the attribute info. */
	WrapDataSource * m_headers;
	
	/** Index of the class used for classification. */
	int m_class_index;
	
	/** Holds the original class values for the particular data source. */
	double * m_original_classes;
	
	/** Predicted Classes. */
	double * m_predicted_classes;
	
	/** Number of rows. */
	int m_rows;

	/** Confusion Matrix*/
	double ** m_confusion_matrix;

	/** Number of Classes*/
	size_t m_number_of_classes;

	/** Number of attributes in the training set*/
	size_t m_no_of_attributes;

	/** Vector containing strings mapped for particular numbers*/
	vector<string> m_class_values;
};
