#pragma once
#include "c45treenominal.h"
#include "csvconnection.h"
#include "extractedCsvDTO.h"
#include "WrapDataSource.h"
#include "distribution.h"
using namespace CSVConnectionInfo;
class TestC45Nominal
{
public:
	TestC45Nominal(void);
	~TestC45Nominal(void);
	void TestSuite();
	void PrintDistribution(Distribution * _dist);
};
