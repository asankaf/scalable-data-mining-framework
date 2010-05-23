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
	this->_rangeVals.clear();
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
vector<BitStreamInfo *> RangeSplitter::Splitter(EncodedAttributeInfo * _attribute,int _rows)
{
	//Suppose we are splitting to following three ranges [0,1],(1,2],(2,3]
	vector<double> vect(3);
	vect[0] = 0;
	vect[1] = 1;
	vect[2] = 2;

	vector<BitStreamInfo *> unique_vals(3);
	BitStreamInfo * temp = NULL;

	//Use UGreaterThan as much as possible. It is the most efficient, and is the base for all others.
	unique_vals[2] = AlgoUtils::UGreaterThan(_attribute,vect[2],_rows);

	//Negation operator (~) creates a new BitStreamInfo object, which has to be deleted afterwards.
	//Result gives all values <= 2
	temp = ~(*unique_vals[2]);

	//Gives values > 1 && values <= 2
	unique_vals[1] = *(AlgoUtils::UGreaterThan(_attribute,vect[1],_rows)) & *(temp);

	delete temp;

	//temp = ~(*unique_vals[1]);

	//Gives values <= 1
	unique_vals[0] = (AlgoUtils::ULessThanOrEq(_attribute,vect[1],_rows));

	//delete temp;
	return unique_vals;

}

EncodedMultiCatAttribute* RangeSplitter::SplitRanges()
{
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
			multiAtt->setUniqueValList(prepareUniqueValList(true,doubleAtt->Precision()));
			break;
		}
	default:
		return multiAtt;
	}

	/************************************************************************/
	/* Unique Bit Map Creation.                                                                     */
	/************************************************************************/
	int no_unique_vals = this->_rangeVals.size() - 1;
	vector<BitStreamInfo*> uniqueBitStreams(no_unique_vals);
	BitStreamInfo* temp = AlgoUtils::UGreaterThan(this->_attribute,this->_rangeVals[1],this->_rowCount);
	//cout<< temp->getProcessedBitStream()<<endl;

	for (int i=1 ; i < no_unique_vals - 1 ; i++)
	{
		BitStreamInfo *next_temp = AlgoUtils::UGreaterThan(this->_attribute,this->_rangeVals[i + 1],this->_rowCount);
		//cout << (~(*next_temp))->getProcessedBitStream()<<endl;
		uniqueBitStreams[i] = *temp & (*(~(*next_temp)));
		temp = next_temp;
	}

	uniqueBitStreams[no_unique_vals - 1] = temp;
	
// 	for (int i = 1 ; i < no_unique_vals ; i++)
// 	{
// 		cout<<uniqueBitStreams[i]->getProcessedBitStream()<<endl;
// 	}

	int no_v_bitstreams = (int)(ceil(log10((double)no_unique_vals)/log10(2.0)));
	
	string unique_val_order;
	for (int i = 1 ; i < no_unique_vals ; i++)
	{
		dynamic_bitset<> temp(no_v_bitstreams,(unsigned long)i);
		string s;
		to_string(temp,s);
		unique_val_order.append(s.c_str());
	}
	vector<BitStreamInfo*> v_bitStreams(no_v_bitstreams);
	for (int i = 0 ; i < no_v_bitstreams ; i++)
	{
		BitStreamInfo *bitStream = new VBitStream();
		dynamic_bitset<> bitSet(this->_rowCount);
		bitStream->convert(bitSet);
		v_bitStreams[i] = bitStream;
	}
	int counter = 1;
	int vBitStreams_switcher = 0;
	for (int j = 0 ; j < unique_val_order.size() ; j++)
	{
		vBitStreams_switcher = j % no_v_bitstreams;
		if((j != 0) && (vBitStreams_switcher == 0)) counter++;
		if(unique_val_order[j] == '0') continue;
		else
		{			
			v_bitStreams[no_v_bitstreams - vBitStreams_switcher - 1]=(*uniqueBitStreams[counter]) | (*v_bitStreams[no_v_bitstreams - vBitStreams_switcher - 1]);
			//v_bitStreams[vBitStreams_switcher]->convert((uniqueBitStreams[counter]->getProcessedBitStream()) | (v_bitStreams[vBitStreams_switcher]->getProcessedBitStream()));
			//cout << v_bitStreams[vBitStreams_switcher]->getProcessedBitStream()<<endl;
		}
	}

	for (int k = 0 ; k < no_v_bitstreams ; k++)
	{
		v_bitStreams[k]->setBitStreamAllocAttID(this->_attribute->attributeID());
		v_bitStreams[k]->setBitStreamAllocAttName(this->_attribute->attributeName());
	}

	multiAtt->setNoOfVBitStreams(no_v_bitstreams,this->_rowCount);
	multiAtt->setAttID(this->_attribute->attributeID());
	multiAtt->setAttName(this->_attribute->attributeName());
	multiAtt->setAttType(this->_attribute->attributeType());
	multiAtt->setVBitStreams(v_bitStreams);
	return multiAtt;
}

EncodedMultiCatAttribute* RangeSplitter::SplitIntoEqualRanges( int no_of_ranges )
{
	int attType = (int)this->_attribute->attributeType();	
	double MinVal;
	double difference;
	switch(attType){
		case 0:
			{
				EncodedIntAttribute* intAtt = static_cast<EncodedIntAttribute*>(this->_attribute);
				difference = (long)ceil((double)intAtt->maxAttVal()/(no_of_ranges + 1));
				MinVal = intAtt->minAttVal();
				break;
			}
		case 1:
			{
				EncodedDoubleAttribute* douAtt = static_cast<EncodedDoubleAttribute*>(this->_attribute);
				difference = (double)(douAtt->maxAttVal()/(no_of_ranges + 1));
				MinVal = douAtt->minAttVal();
				break;
			}
	}
	vector<double> ranges(no_of_ranges + 1);
	for (int i = 0 ; i <= no_of_ranges ; i++)
	{
		ranges[i] = MinVal + difference;
		MinVal = ranges[i];
	}
	this->_rangeVals = ranges;
	return SplitRanges();
}