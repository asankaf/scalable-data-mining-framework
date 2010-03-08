#pragma once

#ifndef APRIORI_ALGO
#define APRIORI_ALGO

#include "Basic_Op.h"

using namespace std;

class Apriori_Algo
{
private:
	int min_support;
	int confidence;
	int lift;
	static int first_support_list_id;
	static int first_support_list[];


public:
	Apriori_Algo();
	~Apriori_Algo();

	void set_min_support(int support);
	int get_min_support();
	int get_confidence();
	int get_lift();
	void Apriori_Algo::set_First_Support_List(string dataID);

};

#endif