/*
#include <iostream>
#include <string>

#include "Symbol.h"
#include "And.h"
#include "Not.h"
#include "Or.h"
#include "DataSourceId.h"

using namespace std;

void main()
{
	Symbol* a = new And("and");
	Symbol* n = new Not("not");
	Symbol* o = new Or("or");
	Symbol* id = new DataSourceId("student_data",0,0);

	cout<<a->getName()<<endl;
	cout<<n->getName()<<endl;
	cout<<o->getName()<<endl;
	cout<<id->getName()<<endl;

	cout<<(id->getType()==TYPEDATASOURCEID)<<endl;
	system("pause");
}
*/
