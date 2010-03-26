#pragma once
#include "BitsetGenerator.h"
#include <vector>
#include <string>
#include "WAHStructure.h"
#include "BitStreamInfo.h"
#include <iostream>
#include <fstream>


using namespace std;
using namespace CompressedStructure;
using namespace CompressedStructure::TestStructure;

class TimeTest
{
public:
	TimeTest(void);
	~TimeTest(void);
	vector<int> testTimeFor(dynamic_bitset<> leftOp, dynamic_bitset<> rightOp, bool andTest,ofstream & errorLog);
	vector<vector<int>> TimeTest::totalpercentageRun(int _length,bool _and_test,ofstream & _error_log);
	void writeResultsToCSV(vector<vector<int>> timeDetails,ofstream & outPut,int length);
	void RunAndTest(size_t _start_val,size_t _end_val,size_t _increment);
	void RunORTest(size_t _start_val,size_t _end_val,size_t _increment);

};
