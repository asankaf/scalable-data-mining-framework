#pragma once
#include "WrapDataSource.h"
#include "boost/exception.hpp"
#include <iostream>
#include "CompressionHandler.h"
#include "smcompressionexceptions.h"
#include "LoadSavedDataSources.h"
#include "configurationreader.h"
#include "DataSources.h"
#include "Utils.h"
class TestCompressionExceptions
{
public:
	TestCompressionExceptions(void);
	~TestCompressionExceptions(void);
	void TestSuite();
	void TestCovertingExceptions();
	WrapDataSource * CreateDataSource();
	void TestNullExceptions();
};
