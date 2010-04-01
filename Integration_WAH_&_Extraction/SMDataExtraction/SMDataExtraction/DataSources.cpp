#include "stdafx.h"
#include "DataSources.h"
#include <algorithm>

DataSources::DataSources(){

}

DataSources::~DataSources(){

}

void DataSources::insertDataSources(WrapDataSource *ds){
	this->_dsnames.push_back(ds->DataSourceName());
	this->_dataSources.push_back(ds);
}

WrapDataSource* DataSources::operator ()(string dataSourceName){
	int dataSourceID = getDataSourceIDByName(dataSourceName);
	return this->_dataSources[dataSourceID];
}

EncodedAttributeInfo* DataSources::operator ()(string dataSourceName,int attID){
	int dataSourceID = getDataSourceIDByName(dataSourceName);
	return (*this->_dataSources[dataSourceID])(attID);
}

BitStreamInfo* DataSources::operator ()(string dataSourceName,int attID,int bitStreamID){
	int dataSourceID = getDataSourceIDByName(dataSourceName);
	return (*((*this->_dataSources[dataSourceID])(attID)))(bitStreamID);
}

int DataSources::getDataSourceIDByName(string DSName){
	int pos = std::find(this->_dsnames.begin(),this->_dsnames.end(),DSName) - this->_dsnames.begin();
	return pos;
}

int DataSources::noOfdataSources(){
	return this->_dataSources.size();
}