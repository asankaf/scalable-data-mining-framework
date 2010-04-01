#include "StdAfx.h"
#include "WrapDataSource.h"
#include "PureIntAttInfo.h"
#include "EncodedIntAttribute.h"
#include "boost/dynamic_bitset.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include "AttributeType.h"
#include <xutility>

using namespace std;
using namespace boost;

WrapDataSource::WrapDataSource(DBQueryExecution cExec,string dsName)
{
	this->_queryDataInfo = cExec;
	this->_noOfAttributes = cExec.RetievedIntData().size() + cExec.RetrievedDoubleData().size() + cExec.RetrievedStringData().size();
	this->_noOfRows = cExec.RowCount();
	this->_dsName = dsName;
	this->_sourceType = DATASOURCE::DATABASE;
}

WrapDataSource::WrapDataSource(ExtractedCsvDTO csvExec,string dsName){
	this->_dsName = dsName;
	this->_noOfAttributes = csvExec.AttributeCount();
	this->_noOfRows = csvExec.RowCount();
	this->_csvExtractedDatainfo = csvExec;
	this->_sourceType = DATASOURCE::CSVFILE;
}

WrapDataSource::WrapDataSource(void)
{

}
WrapDataSource::~WrapDataSource(void)
{
}

size_t WrapDataSource::SpaceUtilsation()
{
	size_t space = 0;
	//typedef vector<EncodedAttributeInfo *> ::const_iterator vector_iter;
	for (size_t index = 0; index < this->_codedAtts.size(); index++)
	{
		space += _codedAtts[index]->SpaceUtilisation();
	}
	return space;
}

void WrapDataSource::encodeAtrributes(){
	if(this->_sourceType == DATASOURCE::DATABASE){
		encodeIntAttributes(this->_queryDataInfo.RetievedIntData());
		encodeStringAttributes(this->_queryDataInfo.RetrievedStringData());
		
	}
	else if (this->_sourceType == DATASOURCE::CSVFILE)
	{
		encodeCSVStringAttributes(this->_csvExtractedDatainfo.StringData(),this->_csvExtractedDatainfo.AttributeCount());
	}

	vector<EncodedAttributeInfo* >::iterator nonrepetitivePos;
	nonrepetitivePos = std::unique(this->_codedAtts.begin(),this->_codedAtts.end());
	this->_codedAtts.erase(nonrepetitivePos,this->_codedAtts.end());
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
		int no_v_bitStreams = (int)ceil(log10((double)upperNum)/log10(2.0));
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
		encodedIntAtt->setVBitStreams(bitStreams);

		end = clock();
		cout<<endl<<"Time to encode Int data : "<<(end - start) << endl;
		
		this->_codedIntAtts.push_back(encodedIntAtt);
		if (encodedIntAtt->attributeID() == 0)
		{
			this->_codedAtts.insert(this->_codedAtts.begin(),encodedIntAtt);
		}
		else this->_codedAtts.insert(this->_codedAtts.end(),encodedIntAtt->attributeID(),encodedIntAtt);
		
		
		start = clock();
		delete []convertdBitArray;
		end = clock();
		cout<<"Time to delete temporary attributes : "<<(end - start)<<endl;
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
			multiCatAtt->setVBitStreams(bitStreams);


			end = clock();
			cout<<"Time to encode converted data : "<<(end - start) << endl;
			this->_codedStringAtts.push_back(multiCatAtt);

			if (multiCatAtt->attributeID() == 0)
			{
				this->_codedAtts.insert(this->_codedAtts.begin(),multiCatAtt);
			}
			else this->_codedAtts.insert(this->_codedAtts.end(),multiCatAtt->attributeID(),multiCatAtt);
		}

	}
	catch(std::exception &e){
		cerr<<"Error in encoding multi category attribute values : "<<e.what()<<endl;
		exit(10);
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
	decodedTuple->setDecodedInts(intTuples);
	decodedTuple->setDecodedStrings(stringTuples);

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

DBQueryExecution WrapDataSource::queryExecPointer(){
	return (this->_queryDataInfo);
}

void WrapDataSource::encodeCSVStringAttributes(PureStringAttInfo** stringAtts,int arrLength){
	vector<PureStringAttInfo*> strData(stringAtts,stringAtts + arrLength);
	encodeStringAttributes(strData);
}