#pragma once
#include "csv_parser.hpp"
#include "extractedCsvDTO.h"
#include "CsvConnection.h"

#include<fstream>
#include <vector>
#include "AttributeType.h"

using namespace std;
using namespace CsvDataExtraction;

namespace CSVConnectionInfo{
	class CsvConnection{
	public:
		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator,const char enclosure_char);
		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator);
		__declspec(dllexport) ExtractedCsvDTO* extractData();
		__declspec(dllexport) ExtractedCsvDTO* readCSV(vector<int> order);
		vector<AttributeProps> getAttributeNames(int attType,vector<int> order);
		__declspec(dllexport) ~CsvConnection();

		
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