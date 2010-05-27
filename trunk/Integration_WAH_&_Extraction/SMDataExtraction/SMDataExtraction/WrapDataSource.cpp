#include "StdAfx.h"
#include "WrapDataSource.h"
#include "PureIntAttInfo.h"
#include "EncodedIntAttribute.h"
#include "boost/dynamic_bitset.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <time.h>
#include "AttributeType.h"
#include <xutility>
#include "encodeddoubleattribute.h"
#include "commons.h"

using namespace std;
using namespace boost;

WrapDataSource::WrapDataSource(DBQueryExecution *cExec,string dsName)
{
	Init();
	this->_queryDataInfo = cExec;
	this->_noOfAttributes = cExec->RetievedIntData().size() + cExec->RetrievedDoubleData().size() + cExec->RetrievedStringData().size();
	this->_noOfRows = cExec->RowCount();
	this->_dsName = dsName;
	this->_sourceType = DATASOURCE::DATABASE;
	this->_existanceDatabitMap = PureAttInfo::existanceBitSet;
	PureAttInfo::existanceBitSet.clear();
	this->_csvExtractedDatainfo = NULL;
}

WrapDataSource::WrapDataSource(ExtractedCsvDTO *csvExec,string dsName){
	Init();
	this->_dsName = dsName;
	this->_noOfAttributes = csvExec->AttributeCount();
	this->_noOfRows = csvExec->RowCount();
	this->_csvExtractedDatainfo = csvExec;
	this->_sourceType = DATASOURCE::CSVFILE;
	this->_existanceDatabitMap = PureAttInfo::existanceBitSet;
	PureAttInfo::existanceBitSet.clear();
	this->_queryDataInfo = NULL;
}

WrapDataSource::WrapDataSource(void)
{	Init();
	this->_existanceDatabitMap = PureAttInfo::existanceBitSet;
	this->_csvExtractedDatainfo = NULL;
	this->_queryDataInfo = NULL;
	PureAttInfo::existanceBitSet.clear();
}
WrapDataSource::~WrapDataSource(void)
{	
	cout<<"WrapDataSource Destructor calls..."<<endl;
	vector<EncodedAttributeInfo *> zero_vect(0);
	vector<EncodedIntAttribute *> zero_vect_int(0);
	vector<EncodedDoubleAttribute *> zero_vect_double(0);
	vector<EncodedMultiCatAttribute *> zero_vect_multi(0);
	
	Commons::DeleteVector(_codedIntAtts.begin(),_codedIntAtts.end());
	Commons::DeleteVector(_codedDoubleAtts.begin(),_codedDoubleAtts.end());
	Commons::DeleteVector(_codedStringAtts.begin(),_codedStringAtts.end());	

	this->_codedIntAtts.clear();
	_codedIntAtts.swap(zero_vect_int);
	this->_codedDoubleAtts.clear();
	_codedDoubleAtts.swap(zero_vect_double);
	this->_codedStringAtts.clear();
	_codedStringAtts.swap(zero_vect_multi);
//	Commons::DeleteVector(_codedAtts.begin(),_codedAtts.end());
	this->_codedAtts.clear();
	_codedAtts.swap(zero_vect);
	if (this->_queryDataInfo != NULL)
	{
		delete this->_queryDataInfo;
	}
	if (this->_csvExtractedDatainfo != NULL)
	{
		delete this->_csvExtractedDatainfo;
	}
}

void WrapDataSource::Init()
{
	Commons::InitVector(_codedAtts.begin(),_codedAtts.end());
	Commons::InitVector(_codedIntAtts.begin(),_codedIntAtts.end());
	Commons::InitVector(_codedDoubleAtts.begin(),_codedDoubleAtts.end());
	Commons::InitVector(_codedStringAtts.begin(),_codedStringAtts.end());
}

size_t WrapDataSource::SpaceUtilsation()
{
	size_t space = sizeof(this);
	for (size_t index = 0; index < this->_codedAtts.size(); index++)
	{
		space += _codedAtts[index]->SpaceUtilisation();
	}
	return space;
}

void WrapDataSource::encodeAtrributes(){
	this->_codedAtts.resize(this->_noOfAttributes);
	if(this->_sourceType == DATASOURCE::DATABASE){
		encodeIntAttributes(this->_queryDataInfo->RetievedIntData());
		encodeStringAttributes(this->_queryDataInfo->RetrievedStringData());
		encodeDoubleAttributes(this->_queryDataInfo->RetrievedDoubleData());
	}
	else if (this->_sourceType == DATASOURCE::CSVFILE)
	{
		encodeIntAttributes(this->_csvExtractedDatainfo->IntData());
		encodeStringAttributes(this->_csvExtractedDatainfo->MultiCatData());
		encodeDoubleAttributes(this->_csvExtractedDatainfo->DoubleData());
		if (this->_csvExtractedDatainfo->StringData() != NULL)
		{
			encodeCSVStringAttributes(this->_csvExtractedDatainfo->StringData(),this->_csvExtractedDatainfo->AttributeCount());
		}
	}
}

int WrapDataSource::noOfAttributes(){
	return this->_noOfAttributes;
}

int WrapDataSource::noOfRows(){
	return this->_noOfRows;
}

void WrapDataSource::CodedAtts(vector<EncodedAttributeInfo *> _coded_atts )
{
	this->_codedAtts = _coded_atts;
}

void WrapDataSource::encodeIntAttributes(vector<PureIntAttInfo*> intAtts){
	int i = 0;
	try{
	for (; i< intAtts.size() ; i++)
	{
		PureIntAttInfo* pureIntAtt = intAtts[i];
		EncodedIntAttribute *encodedIntAtt = new EncodedIntAttribute();
		encodedIntAtt->setAttName(pureIntAtt->attName);
		encodedIntAtt->setAttType(ATT_TYPE::SIGNEDINT_VAL);
		encodedIntAtt->setAttID(pureIntAtt->attID);
		
		int upperNum = pureIntAtt->Upper();
		encodedIntAtt->setMaxVal(upperNum);
		encodedIntAtt->setMinVal(pureIntAtt->Lower());
		int no_v_bitStreams = 0;
		if (upperNum == 0)
		{
			no_v_bitStreams = 1;
		}
		else 
		{
			no_v_bitStreams = (int)ceil(log10((double)upperNum)/log10(2.0));
			if (pow(2.0,(double)no_v_bitStreams) == (double)upperNum)
			{
				no_v_bitStreams++;
			}
		}
		encodedIntAtt->setNoOfVBitStreams(no_v_bitStreams,this->_noOfRows);
		vector<long int> values = pureIntAtt->valList();
		encodedIntAtt->setTheSignBitMap(values,this->_noOfRows);
		int j = 0;
		
		//array to hold converted bit representations of the actual row data.
		dynamic_bitset<> *convertdBitArray = new dynamic_bitset<>[this->_noOfRows];

		time_t start_1,end_1;
		start_1 = clock();
		for (; j < this->_noOfRows ; j++)
		{
			dynamic_bitset<> bitSet(no_v_bitStreams,(unsigned long)abs(values[j]));
			convertdBitArray[j] = bitSet;
		}		
		
		end_1 = clock();
		cout<<"Time to convert each data item to its corresponding bit representation : " << (end_1 - start_1)<<endl;

		int temp = this->_noOfRows;

		time_t start,end;
		start = clock();

		BitStreamInfo **bitStreams = new BitStreamInfo*[no_v_bitStreams];
		for (int k = 0 ; k < no_v_bitStreams ; k++)
		{
			BitStreamInfo* temp_bitStream = new VBitStream();
			dynamic_bitset<> temp_bitSet(temp);
			for (int l = 0 ; l < temp ; l++)
			{
				bool val = convertdBitArray[l][k];
				temp_bitSet[l] = val;
			}
			temp_bitStream->convert(temp_bitSet);
			temp_bitStream->setBitStreamAllocAttID(pureIntAtt->attID);
			temp_bitStream->setBitStreamAllocAttName(pureIntAtt->attName);
			bitStreams[k] = temp_bitStream;
		}
		vector<BitStreamInfo*> vBitStreams(bitStreams,bitStreams + no_v_bitStreams);
		encodedIntAtt->setVBitStreams(vBitStreams);

// 		for (int j = 0 ; j < no_v_bitStreams ; j++)
// 		{
// 			delete bitStreams[j];
// 		}
// 		delete []bitStreams;

		end = clock();
		cout<<endl<<"Time to encode Int data : "<<(end - start) << endl;
		
		this->_codedIntAtts.push_back(encodedIntAtt);
		this->_codedAtts[encodedIntAtt->attributeID()] = encodedIntAtt;
// 		if (encodedIntAtt->attributeID() == 0)
// 		{
// 			this->_codedAtts.insert(this->_codedAtts.begin(),encodedIntAtt);
// 		}
// 		else this->_codedAtts.insert(this->_codedAtts.end(),encodedIntAtt->attributeID(),encodedIntAtt);
// 		
		
		start = clock();
		delete []convertdBitArray;
		end = clock();
		//cout<<"Time to delete temporary attributes : "<<(end - start)<<endl;
	}
}
	catch(std::exception &e){
		cerr<<"Error in encoding int attributes : " << e.what()<<endl;
		exit(9);
	}
}

void WrapDataSource::encodeStringAttributes(vector<PureStringAttInfo*> stringAtts){
	try
	{
		for (int i = 0 ; i < stringAtts.size() ; i++)
		{
			PureStringAttInfo* stringAtt = stringAtts[i];
			EncodedMultiCatAttribute* multiCatAtt = new EncodedMultiCatAttribute();
			multiCatAtt->setAttID(stringAtt->attID);
			multiCatAtt->setAttName(stringAtt->attName);
			multiCatAtt->setAttType(ATT_TYPE::MULTICAT_VAL);

			time_t start,end;
			start = clock();
			dynamic_bitset<> *convertedBitSet = multiCatAtt->mapStringDataToCategories(stringAtt->ValueList(),stringAtt->uniqueValueSet(),this->_noOfRows);
			end = clock();
			cout<<"Time for mapping data to ints : "<<(end - start)<<endl;

// 			start = clock();
// 			dynamic_bitset<> *convertedBitSet_1 = multiCatAtt->mapStringDataToCategories(stringAtt->ValObjects(),this->_noOfRows,stringAtt->uniqueValueSet().size());
// 			end = clock();
// 			cout<<"Time for mapping data to ints using new method : "<<(end - start)<<endl;

// 			for (int j = 0 ; j < this->_noOfRows ; j++)
// 			{
// 				cout<<"Using Old Method : "<<convertedBitSet[j]<<" : ";
// 				cout<<"Using New Method : "<<convertedBitSet_1[j]<<endl;
// 			}

			int temp = this->_noOfRows;
			start = clock();
			int no_v_streams = multiCatAtt->NoOfVBitStreams();

			BitStreamInfo **bitStreams = new BitStreamInfo*[no_v_streams];
			for (int k = 0 ; k < no_v_streams ; k++)
			{
				BitStreamInfo* temp_bitStream = new VBitStream();
				dynamic_bitset<> temp_bitSet(temp);
				for (int l = 0 ; l < temp ; l++)
				{
					bool val = convertedBitSet[l][k];
					temp_bitSet[l] = val;
				}
				temp_bitStream->convert(temp_bitSet);
				temp_bitStream->setBitStreamAllocAttID(stringAtt->attID);
				temp_bitStream->setBitStreamAllocAttName(stringAtt->attName);
				bitStreams[k] = temp_bitStream;
			}
			
			vector<BitStreamInfo*> vBitStreams(bitStreams,bitStreams + no_v_streams);
			multiCatAtt->setVBitStreams(vBitStreams);

// 			for (int j = 0 ; j < no_v_streams ; j++)
// 			{
// 				delete bitStreams[j];
// 			}
// 			delete []bitStreams;


			end = clock();
			cout<<"Time to encode converted data : "<<(end - start) << endl;
			this->_codedStringAtts.push_back(multiCatAtt);

			this->_codedAtts[multiCatAtt->attributeID()] = multiCatAtt;
// 			if (multiCatAtt->attributeID() == 0)
// 			{
// 				this->_codedAtts.insert(this->_codedAtts.begin(),multiCatAtt);
// 			}
// 			else this->_codedAtts.insert(this->_codedAtts.end(),multiCatAtt->attributeID(),multiCatAtt);
		}

	}
	catch(std::exception &e){
		cerr<<"Error in encoding multi category attribute values : "<<e.what()<<endl;
		exit(10);
	}
}

void WrapDataSource::encodeDoubleAttributes(vector<PureDoubleAttInfo*> doubleAtts){
	try{
		for (int i = 0 ; i < doubleAtts.size() ; i++)
		{
			PureDoubleAttInfo* pureDoubleAtt  = doubleAtts[i];
			EncodedDoubleAttribute* encodedDoubleAtt = new EncodedDoubleAttribute();
			encodedDoubleAtt->setAttID(pureDoubleAtt->attID);
			encodedDoubleAtt->setAttName(pureDoubleAtt->attName);
			encodedDoubleAtt->setAttType(ATT_TYPE::DOUBLE_VAL);
			
			double upperVal = pureDoubleAtt->Upper();
			encodedDoubleAtt->setMaxVal(upperVal);
			encodedDoubleAtt->setMinVal(pureDoubleAtt->Lower());
			long precision = getPrecision(doubleAtts);
			encodedDoubleAtt->Precision(precision);
			long maxLongValConverted = floor(upperVal * precision);
			int no_v_bitstreams = 0;
			if (maxLongValConverted == 0)
			{
				no_v_bitstreams = 1;
			}
			else 
			{
				no_v_bitstreams = (int)ceil(log10((double)maxLongValConverted)/log10(2.0));
				if (pow(2.0,(double)no_v_bitstreams) == (double)maxLongValConverted)
				{
					no_v_bitstreams++;
				}
			}
			encodedDoubleAtt->setNoOfVBitStreams(no_v_bitstreams,this->_noOfRows);

			vector<double> vals = pureDoubleAtt->valList();
			encodedDoubleAtt->setTheSignBitMap(vals,this->_noOfRows);
			dynamic_bitset<>* convertedBitArray = new dynamic_bitset<>[this->_noOfRows];

			time_t start_1,end_1;
			start_1 = clock();
			for (int j=0 ; j < this->_noOfRows ; j++)
			{
				double val = abs(vals[j]);
				dynamic_bitset<> bitSet(no_v_bitstreams,(unsigned long)floor(val * precision));
				convertedBitArray[j] = bitSet;
			}	
			end_1 = clock();
			cout<<"Time to convert to the corresponding bit representation : "<<(end_1 - start_1)<<endl;

			BitStreamInfo **bitStreams = new BitStreamInfo*[no_v_bitstreams];
			for (int k = 0 ; k < no_v_bitstreams ; k++)
			{
				BitStreamInfo* temp_bitStream = new VBitStream();
				dynamic_bitset<> temp_bitSet(this->_noOfRows);
				for (int l = 0 ; l < this->_noOfRows ; l++)
				{
					bool val = convertedBitArray[l][k];
					temp_bitSet[l] = val;
				}
				temp_bitStream->convert(temp_bitSet);
				temp_bitStream->setBitStreamAllocAttID(pureDoubleAtt->attID);
				temp_bitStream->setBitStreamAllocAttName(pureDoubleAtt->attName);
				bitStreams[k] = temp_bitStream;
			}
			
			vector<BitStreamInfo*> vBitStreams(bitStreams,bitStreams + no_v_bitstreams);
			encodedDoubleAtt->setVBitStreams(vBitStreams);

// 			for (int j = 0 ; j < no_v_bitstreams ; j++)
// 			{
// 				delete bitStreams[j];
// 			}
// 			delete []bitStreams;

			this->_codedDoubleAtts.push_back(encodedDoubleAtt);
			this->_codedAtts[encodedDoubleAtt->attributeID()] = encodedDoubleAtt;
// 			if (encodedDoubleAtt->attributeID() == 0)
// 			{
// 				this->_codedAtts.insert(this->_codedAtts.begin(),encodedDoubleAtt);
// 			}
// 			else this->_codedAtts.insert(this->_codedAtts.end(),encodedDoubleAtt->attributeID(),encodedDoubleAtt);

			delete []convertedBitArray;			

		}

	}catch(std::exception &ex){
		cerr<<"Error in encoding multi category attribute values : "<<ex.what()<<endl;
		exit(11);
	}
}


vector<EncodedIntAttribute*> WrapDataSource::codedIntAtts(){
	return this->_codedIntAtts;
}

vector<EncodedMultiCatAttribute*> WrapDataSource::codedStringAtts(){
	return this->_codedStringAtts;
}

Tuple* WrapDataSource::DecodeTheTuple(int tupleID){
	Tuple* decodedTuple = new Tuple();

	vector<PureIntAttInfo*> intTuples;
	vector<PureStringAttInfo*> stringTuples;
	vector<PureDoubleAttInfo*> doubleTuples;

	try
	{
		for (int i=0 ; i < this->_codedIntAtts.size() ; i++)
		{
			PureIntAttInfo* intAtt = new PureIntAttInfo();
			int val = this->_codedIntAtts[i]->decodeTheTuple(tupleID);
			long int *vals = new long int[1];
			vals[0] = val;
			intAtt->setValueList(vals);
			intAtt->setLower(val);
			intAtt->setUpper(val);
			intAtt->attID = this->_codedIntAtts[i]->attributeID();
			intAtt->attName = this->_codedIntAtts[i]->attributeName();
			intAtt->type =  this->_codedIntAtts[i]->attributeType();

			intTuples.push_back(intAtt);
		}
	}
	catch(std::exception &e){
		cerr<<"Error in decoding integer values : "<<e.what()<<endl;
		exit(11);
	}


	try{
		for (int j=0 ; j < this->_codedStringAtts.size() ; j++)
		{
			PureStringAttInfo* strAtt=new PureStringAttInfo();
			string val = this->_codedStringAtts[j]->decodeTheTuple(tupleID);
			string* vals = new string[1];
			vals[0] = val;
			strAtt->setValueList(vals,1);
			strAtt->attID =  this->_codedStringAtts[j]->attributeID();
			strAtt->attName =  this->_codedStringAtts[j]->attributeName();
			strAtt->type =  this->_codedStringAtts[j]->attributeType();

			stringTuples.push_back(strAtt);
		}
	}
	catch(std::exception &e){
		cerr<<"Error in decoding string values : "<<e.what()<<endl;
		exit(12);
	}

	try{
		for (int i = 0 ; i < this->_codedDoubleAtts.size(); i++)
		{
			PureDoubleAttInfo* doubleAtt = new PureDoubleAttInfo();
			double val = this->_codedDoubleAtts[i]->decodeTheTuple(tupleID);
			double* vals = new double[1];
			vals[0] = val;
			doubleAtt->setValueList(vals);
			doubleAtt->setUpper(val);
			doubleAtt->setLower(val);

			doubleAtt->attID = this->_codedDoubleAtts[i]->attributeID();
			doubleAtt->attName = this->_codedDoubleAtts[i]->attributeName();
			doubleAtt->type =  this->_codedDoubleAtts[i]->attributeType();

			doubleTuples.push_back(doubleAtt);
		}
	}
	catch(std::exception& ex)
	{
		cerr<<"Error in decoding double values : "<<ex.what()<<endl;
		exit(12);
	}
	decodedTuple->setDecodedInts(intTuples);
	decodedTuple->setDecodedStrings(stringTuples);
	decodedTuple->DecodedDoubleAtts(doubleTuples);

	return decodedTuple;
}



vector<EncodedAttributeInfo*> WrapDataSource::codedAttributes(){
	return this->_codedAtts;
}

EncodedAttributeInfo* WrapDataSource::operator ()(const int attID){
	try{
		return this->_codedAtts.at(attID);
	}
	catch(std::exception &e){
		cerr<<"Error in retrieving attribute info from a given attribute ID : "<<e.what()<<endl;
		exit(13);
	}
}

BitStreamInfo* WrapDataSource::operator ()(const int attID, const int BitStreamID){
	return (*(this->_codedAtts[attID]))(BitStreamID);
}

DBQueryExecution* WrapDataSource::queryExecPointer(){
	return (this->_queryDataInfo);
}

void WrapDataSource::encodeCSVStringAttributes(PureStringAttInfo** stringAtts,int arrLength){
	vector<PureStringAttInfo*> strData(stringAtts,stringAtts + arrLength);
	encodeStringAttributes(strData);
}

long WrapDataSource::getPrecision(vector<PureDoubleAttInfo*> doubleVals){
	//return the precision point max length.
	//TODO implement precision feature.
	return 100;
}

void WrapDataSource::encodeCSVAttributes(){
	
}

std::string WrapDataSource::decodeTheTupleAsString( int tupleID )
{
	string tuple;	
	for (int i=0 ; i< this->_codedAtts.size() ;i++)
	{
		int tupleType = this->_codedAtts[i]->attributeType();
		switch(tupleType)
		{
		case 0:
			{
				EncodedIntAttribute* intAtt = static_cast<EncodedIntAttribute*>(this->_codedAtts[i]);
				long val = intAtt->decodeTheTuple(tupleID);
				tuple += ltoa(val,new char[32],10);
				tuple += ",";
				break;
			}
		case 1:
			{
				EncodedDoubleAttribute* doubleAtt = static_cast<EncodedDoubleAttribute*>(this->_codedAtts[i]);
				std::ostringstream ss;
				double val = doubleAtt->decodeTheTuple(tupleID);
				ss << val;
				tuple += ss.str();
				tuple += ",";
				break;
			}
		case 3:
			{
				EncodedMultiCatAttribute* multicatAtt = static_cast<EncodedMultiCatAttribute*>(this->_codedAtts[i]);
				tuple += multicatAtt->decodeTheTuple(tupleID);
				tuple += ",";
				break;
			}
		}
	}
	string lineBreak = "\n";
	tuple.replace((tuple.size() -1),lineBreak.size(),lineBreak);
	return  tuple;
}

std::string WrapDataSource::generateCSVStringofDecodedData(int _no_of_rows)
{
	string attVals;
	for (int i = 0 ; i < this->_codedAtts.size() ; i++)
	{
		attVals += this->_codedAtts[i]->attributeName();
		attVals += ",";
	}
	string lineBreak = "\n";
	attVals.replace((attVals.size() -1),lineBreak.size(),lineBreak);

	size_t row_no = _no_of_rows <= this->_noOfRows ?_no_of_rows:this->_noOfRows;
	//for (int j = 1 ; j <= this->_noOfRows ; j++)
	for (int j = 1 ; j <= row_no ; j++)
	{
		attVals += decodeTheTupleAsString(j);
	}
	return attVals;
}

std::string WrapDataSource::generateCSVStringofDecodedData()
{

	return generateCSVStringofDecodedData(this->_noOfRows);
}