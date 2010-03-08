#pragma once
#ifndef BASIC_OP
#define BASIC_OP

#include <iostream>
#include <exception>
#include <string>
#include "Query\Query.h"
#include "Query\Olap.h"

using namespace std;

class Basic_Op
{
public:
	Basic_Op();
	~Basic_Op();

	string build_Query(int which, string dataIDs[], string operation);
	int support(string query);



};
#endif