#pragma once

#include<fstream>
#include <vector>

#include "csv_parser.hpp"
#include "extractedCsvDTO.h"
#include "CsvConnection.h"
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

		#pragma region Constructors & Destructor
		
		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator,const char enclosure_char) throw(error_csv_file_reader);

		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator) throw(error_csv_file_reader);

		__declspec(dllexport) ~CsvConnection();

		#pragma endregion Constructors & Destructor
		
		/** Extract the data from CSV file, but all the data will be extracted as strings*/
		__declspec(dllexport) ExtractedCsvDTO* extractData() throw(error_pure_data_conversion);

		/** Extract the data from CSV file, here a particular order can be specified using*/
		/*  an int vector where 0 - Int, 1 - Double, 2 - String is the values specified for*/
		/*  for the vector.*/
		__declspec(dllexport) ExtractedCsvDTO* readCSV(vector<int> order) throw(error_csv_file_reader,error_pure_data_conversion);

		/** An internal operation which will not be exposed to the framework. This is used to*/
		/*  extract the attribute names from the first row after reading the CSV data*/
		vector<AttributeProps> getAttributeNames(int attType,vector<int> order);

		/** A reusable method to check whether a particular file exists when the path is given*/
		/*  Will return a boolean value regarding the availability.*/
		__declspec(dllexport) static bool file_exists(const char *fileName);
		
		
	private:
		const char* _file_source;
		string _null_val;
		const char _field_termiator;
		const char _line_terminator;
		const char _enclosure_char;
		csv_parser _file_parser;
		std::fstream* _inputfile;
		vector<string> _attNames;
		bool isnamesset;
	};
	
	
}