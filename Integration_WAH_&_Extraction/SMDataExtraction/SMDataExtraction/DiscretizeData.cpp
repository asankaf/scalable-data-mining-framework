#include "StdAfx.h"
#include "DiscretizeData.h"

DiscretizeData::DiscretizeData(void)
{
}

DiscretizeData::~DiscretizeData(void)
{
}

EncodedIntAttribute* DiscretizeData::DiscretizeCtsAttribute()
{
	EncodedDoubleAttribute* doubleAtt = this->_ctsAtt;
	EncodedIntAttribute* intAtt = new EncodedIntAttribute();
	intAtt->setSignBitMap(doubleAtt->SignBitMap());
	intAtt->setAttID(doubleAtt->attributeID());
	intAtt->setAttName(doubleAtt->attributeName());
	intAtt->setAttType(doubleAtt->attributeType());

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
	intAtt->setVBitStreams(bitStreams);

	return intAtt;
}