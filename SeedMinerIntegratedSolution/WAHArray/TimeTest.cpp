#include "StdAfx.h"
#include "TimeTest.h"



TimeTest::TimeTest(void)
{
}

TimeTest::~TimeTest(void)
{
}

vector<int> TimeTest::testTimeFor(dynamic_bitset<> leftOp, dynamic_bitset<> rightOp, bool andTest,ofstream & errorLog)
{
	clock_t bitset_begin,bitset_end;
	dynamic_bitset<> resultBitset;
	dynamic_bitset<> test_left_op = leftOp;
	dynamic_bitset<> test_right_op = rightOp;
	if (andTest)
	{
		bitset_begin = clock() * CLK_TCK;
		resultBitset = leftOp & rightOp;
		bitset_end = clock() * CLK_TCK;
	}else
	{
		bitset_begin = clock() * CLK_TCK;
		resultBitset = leftOp | rightOp;
		bitset_end = clock() * CLK_TCK;
	}

	BitStreamInfo  * leftWs = new WAHStructure();
	leftWs->CompressWords(leftOp);
	BitStreamInfo  * rightWs = new WAHStructure();
	rightWs->CompressWords(rightOp);
	BitStreamInfo * compr = NULL;
	clock_t begin,end;
	if (andTest)
	{
		begin = clock() * CLK_TCK;
		compr = *(leftWs) & *(rightWs);
		end = clock()* CLK_TCK;
	}else{
		begin = clock() * CLK_TCK;
		compr = *(leftWs) | *(rightWs);
		end = clock() * CLK_TCK;
	}
	dynamic_bitset <> decompressed = compr->Decompress();
	vector<int>  resultVector;


	if (decompressed != resultBitset)
	{
		errorLog << endl;
		errorLog << "Left OP : " << test_left_op << endl;
		errorLog << "Right OP : " << test_right_op << endl;
		errorLog << endl;
		resultVector.push_back(-1);
	}else{
		resultVector.push_back(end - begin);
	}

	resultVector.push_back(bitset_end - bitset_end);
	delete leftWs,rightWs,compr;
	leftWs = NULL;rightWs = NULL;compr = NULL;

	return resultVector;

}

void TimeTest::writeResultsToCSV( vector<vector<int>> timeDetails,ofstream & outPut,int length )
{
	//outPut<< "Length : "<< length << endl;
	cout <<  "Length : "<< length << endl;
	for (size_t i = 0; i < timeDetails.size(); i++)
	{
		vector<int> result = timeDetails[i];
		outPut << (i * 10)<< " , " << result[0]<<" , "<<result[1]<<" , "<<length<< endl;
	}
	outPut << ""<<endl;
}
void TimeTest::RunAndTest( size_t _start_val,size_t _end_val,size_t _increment )
{
	ofstream output("AND Test.csv");
	output << "Percentage , WAHRuntime, BoostRunTime,Length" << endl;
	ofstream error_log("AND Test Errors.txt");
	for (int i = _start_val; i <= _end_val ; i += _increment)
	{
		vector<vector<int>> result = totalpercentageRun(i,true,error_log);
		writeResultsToCSV(result,output,i);
		output.flush();
		error_log.flush();
	}
	error_log.close();	
	output.close();
}

vector<vector<int>> TimeTest::totalpercentageRun(int _length,bool _and_test,ofstream & _error_log)
{
	vector<vector<int>> result;
	BitsetGenerator bg;	
	//for (int i = 0 ; i <= 100 ; i+=10)
	//{
	//	dynamic_bitset<> leftOp = bg.getRandomBitStream(length,i);
	//	dynamic_bitset<> rightOp = bg.getRandomBitStream(length,i);
	//	int time = testTimeFor(leftOp,rightOp,andTest);
	//	result.push_back(time);
	//}

	for (int i = 0 ; i <= 100 ; i+=10)
	{
		dynamic_bitset<> leftOp = bg.getRandomBitStream(_length,i);
		dynamic_bitset<> rightOp = bg.getRandomBitStream(_length,i);
		vector<int> time = testTimeFor(leftOp,rightOp,_and_test,_error_log);
		result.push_back(time);
	}

	return result;
}

