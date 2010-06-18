#pragma once
#include "WrapDataSource.h"
#include "seedminerexceptions.h"

class DataSources{
public:
	__declspec(dllexport) DataSources();
	__declspec(dllexport) ~DataSources();
	__declspec(dllexport) WrapDataSource* operator()(string dataSourceName) throw(error_vector_out_of_range);
	__declspec(dllexport) EncodedAttributeInfo* operator()(string dataSourceName,int attID) throw(error_vector_out_of_range);
	__declspec(dllexport) BitStreamInfo* operator()(string dataSourceName,int attID,int bitStreamID) throw(error_vector_out_of_range);
	__declspec(dllexport) void insertDataSources(WrapDataSource* ds);
	__declspec(dllexport) vector<string> dsNames(){return this->_dsnames;}
	int getDataSourceIDByName(string DSName);
	int noOfdataSources();

private:
	vector<WrapDataSource*> _dataSources;
	vector<string> _dsnames;
};