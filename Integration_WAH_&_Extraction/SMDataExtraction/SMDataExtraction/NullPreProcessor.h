#pragma once
#include "WrapDataSource.h"
#include "DataPreProcessor.h"

class NullPreProcessor : DataPreProcessor
{
public:
	__declspec(dllexport) NullPreProcessor(WrapDataSource *remaining_ds){m_original_datasource = remaining_ds;}
	__declspec(dllexport) ~NullPreProcessor(void);
	__declspec(dllexport) WrapDataSource* NullEliminatedDatasource() { return this->ds; }
	__declspec(dllexport) void elimiateNullValues();
	vector<int> getInactiveBitIDs(boost::dynamic_bitset<> bitSet);
	vector<BitStreamInfo*> nullEliminatedBitstreams(vector<BitStreamInfo*> bitStreams,vector<int> nullBits);

private:
	WrapDataSource *ds;	
};

