#include "StdAfx.h"
#include "DiscretizeData.h"
#include "AttributeType.h"

DiscretizeData::DiscretizeData(void)
{
}

DiscretizeData::~DiscretizeData(void)
{
	delete this->_ctsDs;
}

EncodedIntAttribute* DiscretizeData::DiscretizeCtsAttribute(EncodedDoubleAttribute* _ctsAtt)
{
	EncodedDoubleAttribute* doubleAtt = _ctsAtt;
	EncodedIntAttribute* intAtt = new EncodedIntAttribute();
	intAtt->setSignBitSet(doubleAtt->signBitSet());
	intAtt->setAttID(doubleAtt->attributeID());
	intAtt->setAttName(doubleAtt->attributeName());
	intAtt->setAttType(ATT_TYPE::SIGNEDINT_VAL);

	long int maxVal = (long int)doubleAtt->maxAttVal();
	long int minVal = (long int)doubleAtt->minAttVal();
	intAtt->setMaxVal(maxVal);
	intAtt->setMinVal(minVal);
	
	long int* vals = new long int[this->_rowCount];
	for (int i = 0 ; i < this->_rowCount ; i++)
	{
		long int val = (long int)(doubleAtt->decodeTheTuple(i+1));
		vals[i] = val;
	}
	int no_v_bitStreams = 0;
	if (maxVal == 0)
	{
		no_v_bitStreams = 1;
	}
	else no_v_bitStreams = (int)ceil(log10((double)maxVal)/log10(2.0));
	intAtt->setNoOfVBitStreams(no_v_bitStreams,this->_rowCount);

	dynamic_bitset<> *convertdBitArray = new dynamic_bitset<>[this->_rowCount];

	
	for (int j=0 ; j < this->_rowCount ; j++)
	{
		dynamic_bitset<> bitSet(no_v_bitStreams,(unsigned long)abs(vals[j]));
		convertdBitArray[j] = bitSet;
	}		
	delete vals;
	
	BitStreamInfo **bitStreams = new BitStreamInfo*[no_v_bitStreams];
	for (int k = 0 ; k < no_v_bitStreams ; k++)
	{
		BitStreamInfo* temp_bitStream = new VBitStream();
		dynamic_bitset<> temp_bitSet(this->_rowCount);
		for (int l = 0 ; l < this->_rowCount ; l++)
		{
			bool val = convertdBitArray[l][k];
			temp_bitSet[l] = val;
		}
		temp_bitStream->convert(temp_bitSet);
		temp_bitStream->setBitStreamAllocAttID(doubleAtt->attributeID());
		temp_bitStream->setBitStreamAllocAttName(doubleAtt->attributeName());
		bitStreams[k] = temp_bitStream;
	}
	vector<BitStreamInfo*> bitStream(bitStreams,bitStreams+no_v_bitStreams);
	intAtt->setVBitStreams(bitStream);

	return intAtt;
}
void DiscretizeData::DiscretizeAllCtsAttributes()
{
	vector<EncodedAttributeInfo*> encodedAtts;
	vector<EncodedIntAttribute*> encodedDisctretedIntAtts;
	encodedAtts.resize(this->_ctsDs->noOfAttributes());
	
	for (int i = 0 ; i < this->_ctsDs->noOfAttributes() ; i++)
	{
		EncodedAttributeInfo* att = this->_ctsDs->codedAttributes()[i];
		int attType = (int)att->attributeType();
		if (attType == 1)
		{
			EncodedIntAttribute* convertedIntAtt = DiscretizeCtsAttribute(static_cast<EncodedDoubleAttribute*>(att));
			delete att;
			encodedAtts[i] = convertedIntAtt;
			encodedDisctretedIntAtts.push_back(convertedIntAtt);
		}
		else encodedAtts[i] = att;
	}
	this->_ctsDs->CodedAtts(encodedAtts);
	this->_ctsDs->discretizeCtsAtts(encodedDisctretedIntAtts);	
}

void DiscretizeData::DiscretizeSelectedCtsAtts( vector<int> zeroBasedAttIDs )
{
	vector<EncodedAttributeInfo*> encodedAtts;
	vector<EncodedIntAttribute*> encodedDisctretedIntAtts;
	encodedAtts.resize(this->_ctsDs->noOfAttributes());

	std::sort(zeroBasedAttIDs.begin(),zeroBasedAttIDs.end());
	int selectAttCounter = 0;
	for (int i = 0 ; i < this->_ctsDs->noOfAttributes() ; i++)
	{
		EncodedAttributeInfo* att = this->_ctsDs->codedAttributes()[i];
		if (selectAttCounter < zeroBasedAttIDs.size() && i == zeroBasedAttIDs[selectAttCounter])
		{
			int attType = (int)att->attributeType();
			if (attType == 1)
			{
				EncodedIntAttribute* convertedIntAtt = DiscretizeCtsAttribute(static_cast<EncodedDoubleAttribute*>(att));
				delete att;
				encodedAtts[i] = convertedIntAtt;
				encodedDisctretedIntAtts.push_back(convertedIntAtt);
			}
			selectAttCounter++;
		}		
		else encodedAtts[i] = att;
	}
	this->_ctsDs->CodedAtts(encodedAtts);
	this->_ctsDs->discretizeCtsAtts(encodedDisctretedIntAtts);	
}
