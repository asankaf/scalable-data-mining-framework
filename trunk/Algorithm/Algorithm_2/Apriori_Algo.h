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

public:
	Apriori_Algo();
	~Apriori_Algo();

	void set_min_support(int support);
	int get_min_support();
	int get_confidence();
	int get_lift();
	int calc_Antecedent_Support(string antecedent_list[]);
	int calc_Confidence(string antecedent_list[], string consequent);

};

#endif