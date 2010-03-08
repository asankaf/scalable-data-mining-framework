#pragma once
#include "DBQueryExecution.h"
#include "EncodedIntAttribute.h"
#include "EncodedMultiCatAttribute.h"
#include "DecodedTuple.h"
#include "EncodedAttributeInfo.h"
#include <vector>


using namespace DBQueryExecutionInfo;

class WrapDataSource
{
public:
	__declspec(dllexport) WrapDataSource(DBQueryExecution cExec,int dataSourceID);
	__declspec(dllexport) WrapDataSource(void);
	__declspec(dllexport) ~WrapDataSource(void);
	__declspec(dllexport) int noOfRows();
	__declspec(dllexport) int noOfAttributes();
	__declspec(dllexport) void encodeAtrributes();
	__declspec(dllexport) vector<EncodedIntAttribute*> codedIntAtts();
	__declspec(dllexport) vector<EncodedMultiCatAttribute*> codedStringAtts();
	__declspec(dllexport) vector<EncodedAttributeInfo*> codedAttributes();
	__declspec(dllexport) Tuple* DecodeTheTuple(int tupleID);
	__declspec(dllexport) EncodedAttributeInfo* operator()(const int attID);
	__declspec(dllexport) BitStreamInfo* operator()(const int attID,const int bitStreamID);
	void encodeIntAttributes(vector<PureIntAttInfo*> intAtts);
	void encodeStringAttributes(vector<PureStringAttInfo*> stringAtts);
	__declspec(dllexport) void CodedAtts(vector<EncodedAttributeInfo *> _coded_atts );
	__declspec(dllexport) BitStreamInfo* getBitString(const int attID,const int bitStreamID);

private:
	DBQueryExecution _queryDataInfo;
	vector<EncodedAttributeInfo*> _codedAtts;
	vector<EncodedIntAttribute*> _codedIntAtts;
	vector<EncodedMultiCatAttribute*> _codedStringAtts;
	int _noOfRows;
	int _noOfAttributes;
	int _dataSourceID;
};
