#pragma once
#include "array_t.h"
#include "bitvector.h"
#include <iostream>

using namespace std;
using namespace ibis;

class TestArray
{
public:
	_declspec(dllexport) TestArray(void);
	_declspec(dllexport) ~TestArray(void);
	_declspec(dllexport) void TestSuite();
	_declspec(dllexport) void CleanArray();

};
