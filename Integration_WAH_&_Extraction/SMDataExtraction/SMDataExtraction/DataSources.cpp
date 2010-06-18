#include "stdafx.h"
#include "DataSources.h"
#include <algorithm>
#include "seedminerexceptions.h"
#include "exceptionreader.h"
#include "ExceptionCodes.h"

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
	
	try
	{
		return this->_dataSources.at(dataSourceID);
	}
	catch(...)
	{
		error_vector_out_of_range ex;
		string error = ExceptionReader::GetError(SM1007);
		error += "-> Retrieving WrapDataSource.";
		ex << error_message(error);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
}

EncodedAttributeInfo* DataSources::operator ()(string dataSourceName,int attID){
	int dataSourceID = getDataSourceIDByName(dataSourceName);
	try
	{
		return (*this->_dataSources.at(dataSourceID))(attID);
	}
	catch(...)
	{
		error_vector_out_of_range ex;
		string error = ExceptionReader::GetError(SM1007);
		error += "-> Retrieving WrapDataSource.";
		ex << error_message(error);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
}

BitStreamInfo* DataSources::operator ()(string dataSourceName,int attID,int bitStreamID){
	int dataSourceID = getDataSourceIDByName(dataSourceName);
	 
	try
	{
		return (*((*this->_dataSources.at(dataSourceID))(attID)))(bitStreamID);
	}
	catch(...)
	{
		error_vector_out_of_range ex;
		string error = ExceptionReader::GetError(SM1007);
		error += "-> Retrieving WrapDataSource.";
		ex << error_message(error);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
}

int DataSources::getDataSourceIDByName(string DSName){
	int pos = -1;
	try
	{
		pos = std::find(this->_dsnames.begin(),this->_dsnames.end(),DSName) - this->_dsnames.begin();
	}
	catch(...)
	{
		error_vector_out_of_range ex;
		string error = ExceptionReader::GetError(SM1007);
		error += "-> Retrieving WrapDataSource.";
		ex << error_message(error);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
	return pos;
}

int DataSources::noOfdataSources(){
	return this->_dataSources.size();
}