#include "stdafx.h"
#include "DataSources.h"

DataSources::DataSources(){

}

DataSources::~DataSources(){

}

void DataSources::insertDataSources(WrapDataSource *ds){
	this->_dataSources.push_back(ds);
}

WrapDataSource* DataSources::operator ()(int dataSourceID){
	return this->_dataSources[dataSourceID];
}

EncodedAttributeInfo* DataSources::operator ()(int dataSourceID,int attID){
	return (*this->_dataSources[dataSourceID])(attID);
}

BitStreamInfo* DataSources::operator ()(int dataSourceID,int attID,int bitStreamID){
	return (*((*this->_dataSources[dataSourceID])(attID)))(bitStreamID);
}