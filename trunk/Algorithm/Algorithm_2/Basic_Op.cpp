#include "Basic_Op.h"

Basic_Op::Basic_Op()
{
}

Basic_Op::~Basic_Op()
{
}

string Basic_Op::build_Query(int which, string dataIDs[], string operation)
{
	string query;
	while (which>0)
	{
		query += dataIDs[which-1] + operation;
		which--;
	}
	return query;
}

int Basic_Op::support(string query)
{
	int count;
	try 
	{
		Query *r = new Query(query);
		Olap olap;
		count = olap.count(r);
		delete r;
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
	return count;
}

int Basic_Op::calc_Antecedent_Support(int num_list_ids, string antecedent_list[])
{
	Basic_Op bo;
	string query = bo.build_Query(num_list_ids, antecedent_list, "&");

	int support = bo.support(query);

	return support;

}

int Basic_Op::calc_Confidence(int num_list_ids, string antecedent_list[], string consequent)
{
	Basic_Op bo;
	string pre_query = bo.build_Query(num_list_ids, antecedent_list, "&");
	string query = pre_query + "&" + consequent;

	int supp_x_and_y = bo.support(query);
	int supp_x_only = bo.support(pre_query);

	return supp_x_and_y/supp_x_only;
}