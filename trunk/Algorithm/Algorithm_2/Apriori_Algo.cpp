#include "Apriori_Algo.h"

Apriori_Algo::Apriori_Algo(void)
{
}

Apriori_Algo::~Apriori_Algo(void)
{
}

void Apriori_Algo::set_min_support(int support)
{
	min_support = support;
}

int Apriori_Algo::get_min_support()
{
	return min_support;
}

int Apriori_Algo::get_confidence()
{
	return confidence;
}

int Apriori_Algo::get_lift()
{
	return lift;
}

void Apriori_Algo::set_First_Support_List(string dataID)
{
	Basic_Op bo;
	first_support_list[first_support_list_id] = bo.support(dataID);
	first_support_list_id++;
}
