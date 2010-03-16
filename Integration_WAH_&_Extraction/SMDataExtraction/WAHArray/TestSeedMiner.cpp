#include "StdAfx.h"
#include "TestSeedMiner.h"
#include "CompressionHandler.h"
#include "Apriori.h"
#include "TestApriori.h"


TestSeedMiner::TestSeedMiner(void)
{
}

TestSeedMiner::~TestSeedMiner(void)
{
}

vector<AssociateRule *> TestSeedMiner::TestAlgorithm(WrapDataSource *_wrapped, float _support, float _confidence)
{
	Apriori ap;
	CompressionHandler ch;
	ch.ConvertTo(_wrapped,BitStreamInfo::VERTICAL_STREAM_FORMAT);
	ap.No_of_items(50);
	ap.No_of_rules(100);
	ap.Support((int)(_wrapped->noOfRows() * (_support/100)));
	ap.Confidence(_confidence);
	ap.RunAlgorithm(_wrapped);
	return ap.Rules();
}



void TestSeedMiner::TestSM(){
	DBConnection cCon("SOY","","");
	cCon.initiateConnectionToDB();
	char* command = "SELECT  * FROM soyabeantest ";
	//char* command = "SELECT  precip,date,class FROM soyabeantest LIMIT 100";
	DBQueryExecution cExec(command);
	cout << "Loaded Data" << endl;
	cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr());

	WrapDataSource *ds = new WrapDataSource(cExec,0);	
	cout <<"Start encoding atts : " << endl;
	ds->encodeAtrributes();
 	//cout << "Space Before Compression: " << ds->SpaceUtilsation() << endl;
	CompressionHandler cmpH;	
	cout << "Start Compressing Data " << endl;
 	cmpH.ConvertTo(ds,BitStreamInfo::WAH_COMPRESSION);
	cout << "Starting to run Algo" << endl;
	vector<AssociateRule *> vect= TestAlgorithm(ds,5,90);
	cout << "Starting to print : "<< endl;
	TestApriori tap;
	tap.PrintRules(vect);
	//cout << "Space After Compression: " << ds->SpaceUtilsation() << endl;
	//cout <<"Start Compression : " << endl;
	//cout <<"Finished Applying Compression : " << endl;
	//cout << "Started Applying algo" << endl;
 // 	Apriori ap;
 // 	ap.Support(1);
 // 	ap.Confidence(90);
 // 	ap.RunAlgorithm(ds);
 // 	TestApriori tap;
 // 	tap.PrintRules(ap.Rules());
	//cout <<"Finished Applying Algo" << endl;
}
