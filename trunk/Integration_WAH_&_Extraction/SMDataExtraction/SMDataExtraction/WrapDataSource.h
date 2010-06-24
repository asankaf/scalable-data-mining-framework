#pragma once
#include "DBQueryExecution.h"
#include "EncodedIntAttribute.h"
#include "EncodedMultiCatAttribute.h"
#include "DecodedTuple.h"
#include "EncodedAttributeInfo.h"
#include "extractedCsvDTO.h"
#include "boost/dynamic_bitset.hpp"
#include "encodeddoubleattribute.h"
#include "seedminerexceptions.h"

#include <vector>

using namespace DBQueryExecutionInfo;
using namespace CsvDataExtraction;

/************************************************************************/
/*    Class  :WrapDataSource.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:18.06.2010 23:40:01
/*    Author :SEEDMiner
/*    Subj   :WrapDataSource is the main core class of this SMDataExtraction
/*			  Sub-project. Under this class there exists various methods to
/*			  call the procedures in other utility classes to load, encode, save
/*			  , etc... and this class is responsible for most of those core component
/*			  integration.
/*    Version: 
/************************************************************************/

class WrapDataSource
{
public:
	/** Enum to identify the external data source*/
	__declspec(dllexport) enum DATASOURCE{DATABASE,CSVFILE,XMLFILE};

	#pragma region Constructors & Destructor

	__declspec(dllexport) WrapDataSource(DBQueryExecution *cExec,string dsName);
	__declspec(dllexport) WrapDataSource(ExtractedCsvDTO *csvExec,string dsName);
	__declspec(dllexport) WrapDataSource(void);
	__declspec(dllexport) ~WrapDataSource(void);

	#pragma endregion Constructors & Destructor

	#pragma region Getters & Setters

	__declspec(dllexport) int noOfRows();
	__declspec(dllexport) void noOfRows(int rows){this->_noOfRows = rows;}
	__declspec(dllexport) int noOfAttributes();
	__declspec(dllexport) void noOfAttributes(int noAtts){this->_noOfAttributes = noAtts;}
	__declspec(dllexport) vector<EncodedIntAttribute*> codedIntAtts();
	__declspec(dllexport) vector<EncodedMultiCatAttribute*> codedStringAtts();
	__declspec(dllexport) vector<EncodedAttributeInfo*> codedAttributes();
	__declspec(dllexport) void CodedAtts(vector<EncodedAttributeInfo *> _coded_atts );
	__declspec(dllexport) DBQueryExecution* queryExecPointer();
	__declspec(dllexport) void queryExecPointer(DBQueryExecution* cExec){this->_queryDataInfo = cExec;}
	__declspec(dllexport) ExtractedCsvDTO * CsvExtractedDatainfo() const { return _csvExtractedDatainfo; }
	__declspec(dllexport) void CsvExtractedDatainfo(ExtractedCsvDTO * val) { _csvExtractedDatainfo = val; }
	__declspec(dllexport) int DataSourceID() const { return _dataSourceID; }
	__declspec(dllexport) void DataSourceID(int val) { _dataSourceID = val; }
	__declspec(dllexport) DATASOURCE SourceType() const { return _sourceType; }
	__declspec(dllexport) void setSourceType(DATASOURCE _stype){this->_sourceType = _stype;}
	__declspec(dllexport) string DataSourceName() const {return this->_dsName;}
	__declspec(dllexport) void setDSName(string _sName){this->_dsName = _sName;}
	__declspec(dllexport) boost::dynamic_bitset<> ExistanceDatabitMap() const { return _existanceDatabitMap; }
	__declspec(dllexport) void ExistanceDatabitMap(boost::dynamic_bitset<> val) { _existanceDatabitMap = val; }

	#pragma endregion Getters & Setters
	
	/**	Method to return a complete instance in any kind of data source*/
	__declspec(dllexport) Tuple* DecodeTheTuple(int tupleID) throw(error_vector_out_of_range);

	/**	Operator to return the EncodedAttribute when the attributeID is provided*/
	__declspec(dllexport) EncodedAttributeInfo* operator()(const int attID) throw(error_vector_out_of_range);

	/**	Operator to return the Bitstream when the BitstreamID and the identity of that bitstream is given*/
	__declspec(dllexport) BitStreamInfo* operator()(const int attID,const int bitStreamID) throw(error_vector_out_of_range);
	
	/** Method to calculate the memory consumption by the whole wrappeddatasource*/
	__declspec(dllexport) size_t SpaceUtilsation() throw(error_vector_out_of_range);
	
	/**	Method to return the decoded instance as a comma separated string*/
	__declspec(dllexport) string decodeTheTupleAsString(int tupleID) throw(error_vector_out_of_range,error_object_null);

	/**	Method to generate whole data source as a commas separated string*/
	__declspec(dllexport) string generateCSVStringofDecodedData() throw(error_vector_out_of_range,error_object_null);
	
	/** Overloaded Method to to generate a part of the data source as a commas separated string*/
	__declspec(dllexport) string generateCSVStringofDecodedData(int _no_of_rows) throw(error_vector_out_of_range,error_object_null);

	/** Method to co-ordinate the encoding of all kinds of attributes in the data source*/
	__declspec(dllexport) void encodeAtrributes() throw(error_encoding_int_atts,error_encoding_string_atts,error_encoding_double_atts);
	
	/**	Method to encode the integer attributes available in the data source*/
	void encodeIntAttributes(vector<PureIntAttInfo*> intAtts) throw(error_encoding_int_atts);
	
	/**	Method to encode the String attributes available in the data source*/
	void encodeStringAttributes(vector<PureStringAttInfo*> stringAtts) throw(error_encoding_string_atts);
	
	/**	Method to encode the Double attributes available in the data source*/
	void encodeDoubleAttributes(vector<PureDoubleAttInfo*> doubleAtts) throw(error_encoding_double_atts);
	
	/**	Method to encode the String attributes available in the data source, when the data source is a CSV data source*/
	void encodeCSVStringAttributes(PureStringAttInfo** stringAtts,int arrLength) throw(error_encoding_string_atts);
	
	/** Method to co-ordinate all kinds of attributes in CSV data sources*/
	void encodeCSVAttributes();
	
	/**	Method to return the Precision value taken from configuration file*/
	long getPrecision(vector<PureDoubleAttInfo*> doubleVals);

	/**	Pre-processing method to discretizing all the continuous attributes in the data source*/
	void discretizeCtsAtts(vector<EncodedIntAttribute*> intAtts){
		this->_codedDoubleAtts.clear();
		for (int i = 0 ; i < intAtts.size() ; i++)
		{
			this->_codedIntAtts.push_back(intAtts[i]);
		}
	}

private:

	/**Initializes the vectors to NULL*/
	void WrapDataSource::Init();

	DBQueryExecution *_queryDataInfo;
	ExtractedCsvDTO *_csvExtractedDatainfo;
	vector<EncodedAttributeInfo*> _codedAtts;
	vector<EncodedIntAttribute*> _codedIntAtts;
	vector<EncodedDoubleAttribute*> _codedDoubleAtts;
	vector<EncodedMultiCatAttribute*> _codedStringAtts;
	int _noOfRows;
	int _noOfAttributes;
	int _dataSourceID;
	DATASOURCE _sourceType;
	string _dsName;
	boost::dynamic_bitset<> _existanceDatabitMap;
	
};
