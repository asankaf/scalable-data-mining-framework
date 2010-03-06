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

int Apriori_Algo::calc_Antecedent_Support(string antecedent_list[])
{
	Basic_Op bo;
	string query = bo.build_Query(antecedent_list->length(), antecedent_list, "&");

	int support = bo.support(query);

	return support;

}

int Apriori_Algo::calc_Confidence(string antecedent_list[], string consequent)
{
	Basic_Op bo;
	string pre_query = bo.build_Query(antecedent_list->length(), antecedent_list, "&");
	string query = pre_query + "&" + consequent;

	int supp_x_and_y = bo.support(query);
	int supp_x_only = bo.support(pre_query);

	return supp_x_and_y/supp_x_only;
}
//add to class.cpp end_here
