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
		//count = olap.count(r);
		delete r;
	}
	catch (std::exception &e)
	{
		cout << e.what() << endl;
	}
	return count;
}