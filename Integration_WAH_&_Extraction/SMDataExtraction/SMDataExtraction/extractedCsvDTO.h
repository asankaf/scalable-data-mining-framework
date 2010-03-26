#pragma once
#include "csv_parser.hpp"
#include "PureStringAttInfo.h"
#include <string>

namespace CsvDataExtraction{
	class ExtractedCsvDTO{
	public:
		__declspec(dllexport) PureStringAttInfo** StringData() { return _stringData; }
		__declspec(dllexport) void StringData(PureStringAttInfo** val) { _stringData = val; }
		__declspec(dllexport) int RowCount() const { return _rowCount; }
		__declspec(dllexport) void RowCount(int val) { _rowCount = val; }
		__declspec(dllexport) int AttributeCount() const { return _attCount; }
		__declspec(dllexport) void AttributeCount(int val) { _attCount = val; }
		
	private:
		PureStringAttInfo** _stringData;		
		int _rowCount;		
		int _attCount;
		
	};
}