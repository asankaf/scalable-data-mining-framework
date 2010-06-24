#pragma once
#include "WrapDataSource.h"
#include "seedminerexceptions.h"

/************************************************************************/
/*    Class  :DataSources.h
/*	  Started:25.02.2010 21:45:12
/*    Updated:21.04.2010 21:45:12
/*    Author :SEEDMiner
/*    Subj   :This is the most coarse grained class in SEEDMiner. This is
/*			  the class that can be viewed as the external data source.
/*    Version: 
/************************************************************************/

class DataSources{
public:
	#pragma region Constructors & Destructors

	__declspec(dllexport) DataSources();
	__declspec(dllexport) ~DataSources();

	#pragma endregion Constructors & Destructors
	
	/** This method provides a wrappeddatasource object pointer when the name of that wrappeddatasource*/
	/** is provided*/
	__declspec(dllexport) WrapDataSource* operator()(string dataSourceName) throw(error_vector_out_of_range);

	/** This method provides an Encoded Attribute object pointer when the name of the wrappeddatasource*/
	/** and the Identity of the Attribute is provided*/
	__declspec(dllexport) EncodedAttributeInfo* operator()(string dataSourceName,int attID) throw(error_vector_out_of_range);
	
	/** This method provides a bitstream object pointer when the name of the wrappeddatasource*/
	/** ,the Identity of the Attribute and the bitstreamID is provided*/
	__declspec(dllexport) BitStreamInfo* operator()(string dataSourceName,int attID,int bitStreamID) throw(error_vector_out_of_range);

	/** Method to insert the provided wrappeddatasource into the wrappeddatasource vector*/
	__declspec(dllexport) void insertDataSources(WrapDataSource* ds);

	/** Method to provide the names of the saved wrappeddatasources*/
	__declspec(dllexport) vector<string> dsNames(){return this->_dsnames;}

	/** Given a wrappeddatasource name, this method will provide the ID of the particular wrappeddatasource, if exists*/
	int getDataSourceIDByName(string DSName);

	/** Method to provide the number of wrappeddatasources that are stored in the vector*/
	int noOfdataSources();

private:
	vector<WrapDataSource*> _dataSources;
	vector<string> _dsnames;
};