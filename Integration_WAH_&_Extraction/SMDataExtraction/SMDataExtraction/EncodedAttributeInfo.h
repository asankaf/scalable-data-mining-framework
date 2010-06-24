#pragma once

#include "VBitStream.h"
#include "AttributeType.h"
#include "boost/dynamic_bitset.hpp"
#include "seedminerexceptions.h"

#include <vector>

using namespace std;
using namespace boost;

/************************************************************************/
/*    Class  :EncodedAttributeInfo.h
/*	  Started:25.02.2010 21:45:12
/*    Updated:21.04.2010 23:11:09
/*    Author :SEEDMiner
/*    Subj   :The abstract super class of all specific types of Encoded Attributes
/*			  This particular class holds whatever the common behaviours 
/*			  and attributes that are available in each specific Attribute
/*    Version: 
/************************************************************************/

class EncodedAttributeInfo{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) EncodedAttributeInfo();
	__declspec(dllexport) ~EncodedAttributeInfo();

	#pragma endregion Constructors & Destructor
	
	#pragma region Getters & Setters

	__declspec(dllexport) vector<BitStreamInfo*> vBitStreams();
	__declspec(dllexport) string attributeName();
	__declspec(dllexport) int attributeID();
	__declspec(dllexport) int NoOfVBitStreams();
	__declspec(dllexport) ATT_TYPE attributeType();
	__declspec(dllexport) void setVBitStreams(BitStreamInfo **vBitset);
	__declspec(dllexport) void setVBitStreams(vector<BitStreamInfo *> VBitStreams);
	__declspec(dllexport) void setNoOfVBitStreams(int novBitsets,int rows);
	__declspec(dllexport) void setAttType(ATT_TYPE type);
	__declspec(dllexport) void setAttName(string name);
	__declspec(dllexport) void setAttID(int attID);
	__declspec(dllexport) void setVBitStreamSize(int newSize);

	#pragma endregion Getters & Setters

	/** Method to provide a particular bitstream which is stored in Attribute when the bitstreamID is provided*/
	__declspec(dllexport) BitStreamInfo* bitStreamAt(int bitStreamID);

	/** Operator to provide a particular bitstream which is stored in Attribute when the bitstreamID is provided*/
	__declspec(dllexport) BitStreamInfo* operator()(const int bitStreamID) throw(error_vector_out_of_range);

	/** Method to provide the memory allocated for the specific attribute*/
	__declspec(dllexport) size_t SpaceUtilisation() throw(error_vector_out_of_range);

	/** Method to delete vertical bitstreams from a particular attribute*/
	__declspec(dllexport) void DeleteVBitStreams();
	
	/** A template method that needs to be implemented in subclasses to decode a tuple according to the type of data they have*/
	template<typename T>
	__declspec(dllexport) T decodeTheTuple(int tupleID) = 0;


private:
	vector<BitStreamInfo*> _vBitStreams;
	BitStreamInfo **_vBStreams;
	ATT_TYPE _attType;
	int _attID;
	string _attName;
	int _noOfVBitStreams;
};