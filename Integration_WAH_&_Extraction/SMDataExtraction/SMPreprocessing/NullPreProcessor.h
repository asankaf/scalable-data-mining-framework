#pragma once
#include "WrapDataSource.h"
#include "DataPreProcessor.h"

/************************************************************************/
/*    Class  :NullPreProcessor.h
/*	  Started:18.05.2010 21:28:12
/*    Updated:28.05.2010 21:38:12
/*    Author :SEEDMiner
/*    Subj   :Preprocessor class to Remove null value rows
/*    Version: 
/************************************************************************/

class NullPreProcessor : DataPreProcessor
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) NullPreProcessor(WrapDataSource *remaining_ds){m_original_datasource = remaining_ds;}
	__declspec(dllexport) ~NullPreProcessor(void);

	#pragma endregion Constructors & Destructor

	/** Getter for the null eliminated reconstructed data source*/
	__declspec(dllexport) WrapDataSource* NullEliminatedDatasource() { return this->ds; }

	/**	Method to eliminate null rows from the data source*/
	__declspec(dllexport) void elimiateNullValues();

	/** Method to return the bit indices where the value is zero*/
	vector<int> getInactiveBitIDs(boost::dynamic_bitset<> bitSet);

	/**	Internally utilized method to retrieve null eliminated bit streams when the original bit streams are provided*/
	vector<BitStreamInfo*> nullEliminatedBitstreams(vector<BitStreamInfo*> bitStreams,vector<int> nullBits);

private:
	WrapDataSource *ds;	
};

