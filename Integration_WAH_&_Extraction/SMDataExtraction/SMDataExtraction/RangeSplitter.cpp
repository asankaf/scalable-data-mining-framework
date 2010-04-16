#include "StdAfx.h"
#include "RangeSplitter.h"
#include "EncodedIntAttribute.h"

#include <sstream>
#include "EncodedDoubleAttribute.h"

RangeSplitter::RangeSplitter(void)
{
}

RangeSplitter::~RangeSplitter(void)
{
}

EncodedMultiCatAttribute* RangeSplitter::SplitRangesInNumericAtts(){
	EncodedMultiCatAttribute* multiAtt = new EncodedMultiCatAttribute();
	int attType = (int)this->_attribute->attributeType();
	switch(attType)
	{
	case 0:
		{
			EncodedIntAttribute* intAtt = static_cast<EncodedIntAttribute*>(this->_attribute);
			if (intAtt->minAttVal() < this->_rangeVals[0])
			{
				this->_rangeVals[0] = intAtt->minAttVal();
			}
			if (intAtt->maxAttVal() > this->_rangeVals[this->_rangeVals.size() - 1])
			{
				this->_rangeVals[this->_rangeVals.size() - 1] = intAtt->maxAttVal();
			}
			if (this->_rangeVals.size() == 1)
			{
				this->_rangeVals.push_back(intAtt->maxAttVal());
			}
			multiAtt->setUniqueValList(prepareUniqueValList());
			
			dynamic_bitset<> *convertdBitArray = new dynamic_bitset<>[this->_rowCount];
			int temp = 0;
			if (this->_maxValAssigned == 1)
			{
				temp = 1;
			}
			else temp = (int)(ceil(log10((double)this->_maxValAssigned)/log10(2.0)));

			multiAtt->setNoOfVBitStreams(temp,this->_rowCount);

			for (int i = 1; i <= this->_rowCount ; i++)
			{
				int currVal = intAtt->decodeTheTuple(i);
				convertdBitArray[i - 1] = convertInt(currVal,temp);
			}

			
			BitStreamInfo **bitStreams = new BitStreamInfo*[temp];
			for (int k = 0 ; k < temp ; k++)
			{
				BitStreamInfo* temp_bitStream = new VBitStream();
				dynamic_bitset<> temp_bitSet(this->_rowCount);
				for (int l = 0 ; l < this->_rowCount ; l++)
				{
					bool val = convertdBitArray[l][k];
					temp_bitSet[l] = val;
				}
				temp_bitStream->convert(temp_bitSet);
				temp_bitStream->setBitStreamAllocAttID(intAtt->attributeID());
				temp_bitStream->setBitStreamAllocAttName(intAtt->attributeName());
				bitStreams[k] = temp_bitStream;
			}
			multiAtt->setVBitStreams(bitStreams);
			break;
		}
	case 1:
		{
			EncodedDoubleAttribute* doubleAtt = static_cast<EncodedDoubleAttribute*>(this->_attribute);

			if (doubleAtt->minAttVal() < this->_rangeVals[0])
			{
				this->_rangeVals[0] = doubleAtt->minAttVal();
			}
			if (doubleAtt->maxAttVal() > this->_rangeVals[this->_rangeVals.size() - 1])
			{
				this->_rangeVals[this->_rangeVals.size() - 1] = doubleAtt->maxAttVal();
			}
			if (this->_rangeVals.size() == 1)
			{
				this->_rangeVals.push_back(doubleAtt->maxAttVal());
			}

			for (int i = 0 ; i < this->_rangeVals.size() ; i++)
			{
				this->_rangeVals[i] *= doubleAtt->Precision();
			}

			multiAtt->setUniqueValList(prepareUniqueValList(true,doubleAtt->Precision()));

			dynamic_bitset<> *convertdBitArray = new dynamic_bitset<>[this->_rowCount];
			int temp = 0;
			if (this->_maxValAssigned == 1)
			{
				temp = 1;
			}
			else temp = (int)(ceil(log10((double)this->_maxValAssigned)/log10(2.0)));

			multiAtt->setNoOfVBitStreams(temp,this->_rowCount);

			for (int i = 1; i <= this->_rowCount ; i++)
			{
				int currVal = (long)doubleAtt->decodeTheTuple(i,true);
				convertdBitArray[i - 1] = convertInt(currVal,temp);
			}

			BitStreamInfo **bitStreams = new BitStreamInfo*[temp];
			for (int k = 0 ; k < temp ; k++)
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
			multiAtt->setVBitStreams(bitStreams);
			break;
		}
	}

	return multiAtt;
}

vector<string> RangeSplitter::prepareUniqueValList(bool isDoubleAtt,long precision){
	vector<string> uniqueVals;
	for (int i = 0 ; i < (this->_rangeVals.size() - 1) ; i++)
	{
		std::stringstream ss_min;
		isDoubleAtt ? (ss_min<<this->_rangeVals[i]/precision) : ss_min<<this->_rangeVals[i];
		string minVal = ss_min.str();
		std::stringstream ss_max;
		isDoubleAtt ? (ss_max<<this->_rangeVals[i+1]/precision) : ss_max<<this->_rangeVals[i+1];
		string maxVal = ss_max.str();
		uniqueVals.push_back(minVal + " - " + maxVal);
		this->_maxValAssigned++;
	}
	return uniqueVals;
}

boost::dynamic_bitset<> RangeSplitter::convertInt(int val,int no_v_bitstreams){
	int valAssign = this->_maxValAssigned - 1;
	for (int i = (this->_rangeVals.size() - 1) ; i > 1 ; i--)
	{
		if ((this->_rangeVals[i] >= val) && (val > this->_rangeVals[i - 1]))
		{
			break;
		}
		else valAssign--;
	}
	boost::dynamic_bitset<> bitConverted(no_v_bitstreams,valAssign);
	return bitConverted;
}