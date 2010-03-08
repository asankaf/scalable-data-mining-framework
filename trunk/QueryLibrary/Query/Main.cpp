#include "Query.h"
#include "Symbol.h"

void main()
{
		Query q("a(1,2)");
		bool b = typeid(Query).name()==typeid(q).name();
		cout<<b<<endl;
		system("pause");	
}
