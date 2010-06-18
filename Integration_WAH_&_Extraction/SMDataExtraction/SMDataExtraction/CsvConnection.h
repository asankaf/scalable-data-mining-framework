#pragma once
#include "csv_parser.hpp"
#include "extractedCsvDTO.h"
#include "CsvConnection.h"

#include<fstream>
#include <vector>
#include "AttributeType.h"
#include "seedminerexceptions.h"

using namespace std;
using namespace CsvDataExtraction;

/************************************************************************/
/*    Class  :CsvConnection.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:18.04.2010 18:48:51
/*    Author :SEEDMiner
/*    Subj   :CSVConnection class for integrate as one of the Data Feeders
/*			  Where a particular CSV or Tab Seperated Files can easily be
/*			  inject into the framework by supplying necessary parameters.
/*    Version: 
/************************************************************************/

namespace CSVConnectionInfo{
	class CsvConnection{
	public:
		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator,const char enclosure_char) throw(error_csv_file_reader);
		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator) throw(error_csv_file_reader);
		__declspec(dllexport) ~CsvConnection();
		__declspec(dllexport) ExtractedCsvDTO* extractData() throw(error_pure_data_conversion);
		__declspec(dllexport) ExtractedCsvDTO* readCSV(vector<int> order) throw(error_csv_file_reader,error_pure_data_conversion);

		vector<AttributeProps> getAttributeNames(int attType,vector<int> order);
		static bool file_exists(const char *fileName);
		
		
	private:
		const char* _file_source;
		const char _field_termiator;
		const char _line_terminator;
		const char _enclosure_char;
		csv_parser _file_parser;
		std::fstream* _inputfile;
		vector<string> _attNames;
		bool isnamesset;
	};
	
	
}