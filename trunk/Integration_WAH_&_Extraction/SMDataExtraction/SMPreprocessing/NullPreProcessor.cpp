#include "StdAfx.h"
#include "NullPreProcessor.h"
#include "boost/dynamic_bitset.hpp"
#include <algorithm>
#include "EncodedDoubleAttribute.h"


void NullPreProcessor::elimiateNullValues(){
	vector<EncodedAttributeInfo*> atts = m_original_datasource->codedAttributes();
	boost::dynamic_bitset<> existanceBitMap = m_original_datasource->ExistanceDatabitMap();
	vector<int> nullbitIDs = getInactiveBitIDs(existanceBitMap);
	if (existanceBitMap.count() == m_original_datasource->noOfRows())
	{
		this->ds = m_original_datasource;
		return;
	}
	WrapDataSource* newDS = new WrapDataSource();
	newDS->setDSName(m_original_datasource->DataSourceName());
	newDS->DataSourceID(m_original_datasource->DataSourceID());
	newDS->setSourceType(m_original_datasource->SourceType());
	newDS->noOfRows((m_original_datasource->noOfRows()) - nullbitIDs.size());
	newDS->noOfAttributes(m_original_datasource->noOfAttributes());
	newDS->ExistanceDatabitMap(m_original_datasource->ExistanceDatabitMap());
	vector<EncodedAttributeInfo*> newAtts;
	newAtts.resize(atts.size());
	for (int i = 0 ; i < atts.size() ; i++)
	{
		EncodedAttributeInfo *att = atts[i];
		int attType = (int)att->attributeType();
		vector<BitStreamInfo*> currBitStreams = att->vBitStreams();
		switch(attType){
			case 0:
				{
					EncodedIntAttribute* intAtt = static_cast<EncodedIntAttribute*>(att);
					EncodedIntAttribute* modifiedAtt = new EncodedIntAttribute();
					modifiedAtt->setAttName(intAtt->attributeName());
					modifiedAtt->setAttID(intAtt->attributeID());
					modifiedAtt->setAttType(intAtt->attributeType());
					modifiedAtt->setMaxVal(intAtt->maxAttVal());
					modifiedAtt->setMinVal(intAtt->minAttVal());
					//modifiedAtt->setSignBitMap(intAtt->SignBitMap());
					modifiedAtt->setSignBitSet(intAtt->SignBitSet());
					modifiedAtt->setVBitStreams(nullEliminatedBitstreams(currBitStreams,nullbitIDs));
					modifiedAtt->setNoOfVBitStreams(intAtt->NoOfVBitStreams(),0);
					newAtts[i] = modifiedAtt;
					break;
				}
			case 1:
				{
					EncodedDoubleAttribute* doubleAtt = static_cast<EncodedDoubleAttribute*> (att);
					EncodedDoubleAttribute* modifiedAtt = new EncodedDoubleAttribute();
					modifiedAtt->setAttName(doubleAtt->attributeName());
					modifiedAtt->setAttID(doubleAtt->attributeID());
					modifiedAtt->setAttType(doubleAtt->attributeType());
					modifiedAtt->setMaxVal(doubleAtt->maxAttVal());
					modifiedAtt->setMinVal(doubleAtt->minAttVal());
					modifiedAtt->setSignBitSet(doubleAtt->signBitSet());
					//modifiedAtt->SignBitMap(doubleAtt->SignBitMap());
					modifiedAtt->Precision(doubleAtt->Precision());
					modifiedAtt->setVBitStreams(nullEliminatedBitstreams(currBitStreams,nullbitIDs));
					modifiedAtt->setNoOfVBitStreams(doubleAtt->NoOfVBitStreams(),0);
					newAtts[i] = modifiedAtt;
					break;
				}
			case 3:
				{
					EncodedMultiCatAttribute* mulAtt = static_cast<EncodedMultiCatAttribute*>(att);
					EncodedMultiCatAttribute* modifiedAtt = new EncodedMultiCatAttribute();
					modifiedAtt->setAttID(mulAtt->attributeID());
					modifiedAtt->setAttName(mulAtt->attributeName());
					modifiedAtt->setAttType(mulAtt->attributeType());
					modifiedAtt->setNoOfVBitStreams(mulAtt->NoOfVBitStreams(),0);
					vector<string> _uniqueValList = mulAtt->uniqueValList();
					//int nullPOS = std::find(_uniqueValList.begin(),_uniqueValList.end(),"?") - _uniqueValList.begin();
					//_uniqueValList.erase(_uniqueValList.begin() + nullPOS);
					modifiedAtt->setUniqueValList(_uniqueValList);
					vector<BitStreamInfo*> newBitStreams = nullEliminatedBitstreams(mulAtt->vBitStreams(),nullbitIDs);
					//newBitStreams.erase(newBitStreams.begin() + nullPOS);
					modifiedAtt->setVBitStreams(newBitStreams);
					newAtts[i] = modifiedAtt;
					break;
				}
		}
		
	}
	newDS->CodedAtts(newAtts);
	this->ds = newDS;
}

vector<int> NullPreProcessor::getInactiveBitIDs(boost::dynamic_bitset<> bitSet){
	vector<int> activeIDs;
	for (int i = 0 ; i < bitSet.size() ; i++)
	{
		if ((int)bitSet[i] == 0) activeIDs.push_back(i);
	}
	return activeIDs;
}

vector<BitStreamInfo*> NullPreProcessor::nullEliminatedBitstreams(vector<BitStreamInfo*> bitStreams,vector<int> nullBits){
	
	vector<BitStreamInfo*> nullEliminatedBitStreams;
	nullEliminatedBitStreams.resize(bitStreams.size());
	for (int i = 0 ; i < bitStreams.size() ; i++)
	{
		int NullbitsCountDown = 0;
		VBitStream* currentBitStream = static_cast<VBitStream*>(bitStreams[i]);
		dynamic_bitset<> currentBitSet = currentBitStream->Decompress();
		int bitCount = currentBitSet.size() - nullBits.size();
		BitStreamInfo* vBitStream = new VBitStream(bitCount);
		vBitStream->setBitStreamAllocAttID(currentBitStream->BitStreamAllocAttID());
		vBitStream->setBitStreamAllocAttName(currentBitStream->BitStreamAllocAttName());
		dynamic_bitset<> nullEliminatedBitSet(bitCount);

		for (int j = 0,k = 0 ; j <= currentBitSet.size() ; j++)
		{
			if ((NullbitsCountDown < nullBits.size())&& (j == nullBits[NullbitsCountDown]))
			{
				NullbitsCountDown++;
				continue;
			}
			nullEliminatedBitSet[k++] = currentBitSet[j];
		} 
		vBitStream->convert(nullEliminatedBitSet);
		nullEliminatedBitStreams[i] = vBitStream;
	}

	return nullEliminatedBitStreams;
}

NullPreProcessor::~NullPreProcessor(void)
{

}
