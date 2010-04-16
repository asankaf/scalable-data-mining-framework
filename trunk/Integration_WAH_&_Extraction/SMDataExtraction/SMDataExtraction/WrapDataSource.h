#pragma once
#include "DBQueryExecution.h"
#include "EncodedIntAttribute.h"
#include "EncodedMultiCatAttribute.h"
#include "DecodedTuple.h"
#include "EncodedAttributeInfo.h"
#include "extractedCsvDTO.h"
#include <vector>

using namespace DBQueryExecutionInfo;
using namespace CsvDataExtraction;

class WrapDataSource
{
public:
	__declspec(dllexport) enum DATASOURCE{DATABASE,CSVFILE,XMLFILE};
	__declspec(dllexport) WrapDataSource(DBQueryExecution cExec,string dsName);
	__declspec(dllexport) WrapDataSource(ExtractedCsvDTO csvExec,string dsName);
	__declspec(dllexport) WrapDataSource(void);
	__declspec(dllexport) ~WrapDataSource(void);
	__declspec(dllexport) int noOfRows();
	__declspec(dllexport) void noOfRows(int rows){this->_noOfRows = rows;}
	__declspec(dllexport) int noOfAttributes();
	__declspec(dllexport) void noOfAttributes(int noAtts){this->_noOfAttributes = noAtts;}
	__declspec(dllexport) void encodeAtrributes();
	__declspec(dllexport) vector<EncodedIntAttribute*> codedIntAtts();
	__declspec(dllexport) vector<EncodedMultiCatAttribute*> codedStringAtts();
	__declspec(dllexport) vector<EncodedAttributeInfo*> codedAttributes();
	__declspec(dllexport) Tuple* DecodeTheTuple(int tupleID);
	__declspec(dllexport) EncodedAttributeInfo* operator()(const int attID);
	__declspec(dllexport) BitStreamInfo* operator()(const int attID,const int bitStreamID);
	void encodeIntAttributes(vector<PureIntAttInfo*> intAtts);
	void encodeStringAttributes(vector<PureStringAttInfo*> stringAtts);
	void encodeCSVStringAttributes(PureStringAttInfo** stringAtts,int arrLength);
	__declspec(dllexport) void CodedAtts(vector<EncodedAttributeInfo *> _coded_atts );
	__declspec(dllexport) size_t SpaceUtilsation();
	__declspec(dllexport) DBQueryExecution queryExecPointer();
	__declspec(dllexport) int DataSourceID() const { return _dataSourceID; }
	__declspec(dllexport) void DataSourceID(int val) { _dataSourceID = val; }
	__declspec(dllexport) int SourceType() const { return _sourceType; }
	__declspec(dllexport) void setSourceType(DATASOURCE _stype){this->_sourceType = _stype;}
	__declspec(dllexport) string DataSourceName() const {return this->_dsName;}
	__declspec(dllexport) void setDSName(string _sName){this->_dsName = _sName;}

private:
	DBQueryExecution _queryDataInfo;
	ExtractedCsvDTO _csvExtractedDatainfo;
	vector<EncodedAttributeInfo*> _codedAtts;
	vector<EncodedIntAttribute*> _codedIntAtts;
	vector<EncodedMultiCatAttribute*> _codedStringAtts;
	int _noOfRows;
	int _noOfAttributes;
	int _dataSourceID;
	DATASOURCE _sourceType;
	string _dsName;
};
