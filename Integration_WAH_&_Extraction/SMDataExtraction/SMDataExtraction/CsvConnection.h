#pragma once
#include "csv_parser.hpp"
#include "extractedCsvDTO.h"

using namespace CsvDataExtraction;

namespace CSVConnectionInfo{
	class CsvConnection{
	public:
		__declspec(dllexport) CsvConnection(const char* file_name,const char field_terminator,const char line_terminator,const char enclosure_char);
		__declspec(dllexport) ExtractedCsvDTO* extractData();
		__declspec(dllexport) ~CsvConnection();
	
	private:
		const char* _file_source;
		const char _field_termiator;
		const char _line_terminator;
		const char _enclosure_char;
		csv_parser _file_parser;
	};
}