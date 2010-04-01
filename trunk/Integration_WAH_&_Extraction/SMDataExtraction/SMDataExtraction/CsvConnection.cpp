#include "stdafx.h"
#include "CsvConnection.h"
#include "PureStringAttInfo.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <time.h>

using namespace std;

CSVConnectionInfo::CsvConnection::CsvConnection(const char* file_name,const char field_terminator,const char line_terminator,const char enclosure_char):_file_source(file_name),_field_termiator(field_terminator),_line_terminator(line_terminator),_enclosure_char(enclosure_char){
	this->_file_parser.init(this->_file_source);
	this->_file_parser.set_enclosed_char(this->_enclosure_char,ENCLOSURE_OPTIONAL);
	this->_file_parser.set_field_term_char(this->_field_termiator);
	this->_file_parser.set_line_term_char(this->_line_terminator);
}

CSVConnectionInfo::CsvConnection::~CsvConnection(){
	
}

ExtractedCsvDTO* CSVConnectionInfo::CsvConnection::extractData(){
	time_t start,end;
	start = clock();
	ExtractedCsvDTO *exCsv = new ExtractedCsvDTO();
	vector<csv_row> rowSets;

	while (this->_file_parser.has_more_rows())
	{
		csv_row row = this->_file_parser.get_row();
		rowSets.push_back(row);
		if (exCsv->AttributeCount() == 0)
		{
			exCsv->AttributeCount(row.size());
		}
	}

	end = clock();
	cout<<"Time to retrieve CSV data : " << (end - start)<<endl;
	exCsv->RowCount(rowSets.size() - 1);
	int attCount = exCsv->AttributeCount();
	int rowCount = exCsv->RowCount();

	PureStringAttInfo** stringData = new PureStringAttInfo*[exCsv->AttributeCount()];
	for (int i = 0 ; i < attCount ; i++)
	{
		PureStringAttInfo* strData = new PureStringAttInfo();
		strData->attID = i;
		strData->attName = rowSets[0][i];
		
		vector<string> vals;
		vals.resize(rowCount);

		try{
			for (int j = 1 ; j <= rowCount ; j++)
			{
				vals[j - 1] = rowSets[j][i];	
				//cout<<"Index "<<j<<" : value : "<<vals.at(j-1)<<endl;
			}
		}
		catch(std::exception &ex){
			
		}
		strData->setValList(vals);
		stringData[i] = strData;
	}

	exCsv->StringData(stringData);

	return exCsv;
}