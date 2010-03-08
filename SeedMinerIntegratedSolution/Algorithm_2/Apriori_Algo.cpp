#include "Apriori_Algo.h"
#include "WrapDataSource.h"
#include "DBConnection.h"
#include "DBQueryExecution.h"

using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;

Apriori_Algo::Apriori_Algo(void)
{
	try
	{
// 	DBConnection cCon("soyabean","","");
// 	cCon.initiateConnectionToDB();
// 	//DBQueryExecution cPtr("SELECT precip, `plant-growth`, stem	FROM `soybeanTest.csv`");
// 	//cPtr.ExecuteQueryAndBindData(cCon.DBConnectionPtr());
// 	const char* statement="SELECT precip FROM `soybeanTest.csv`";
// 	DBQueryExecution cExec(statement);
// 	cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr());
// 	WrapDataSource *ds = new WrapDataSource(cExec,0);
// 	ds->encodeAtrributes();
// 	//WrapDataSource *wds = new WrapDataSource(cPtr,0);
// 	//wds->encodeAtrributes();
// 	//cout<<wds->noOfAttributes();
// 	cout<<ds->noOfAttributes()<<endl;
	}
	catch(std::exception &ex){

	}

}

Apriori_Algo::~Apriori_Algo(void)
{
	cout<<"Apriori_algo destrcutor calls"<<endl;
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
