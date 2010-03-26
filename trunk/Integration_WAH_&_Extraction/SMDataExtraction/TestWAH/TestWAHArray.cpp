#include "StdAfx.h"
#include "TestWAHArray.h"
#include "BitsetGenerator.h"
#include <vector>
#include <iostream>
namespace CompressedStructure{
	namespace TestStructure{
		TestWAHArray::TestWAHArray(void)
		{
		}

		TestWAHArray::~TestWAHArray(void)
		{
		}


		void TestWAHArray::testDecompress()
		{	
			cout << "Test decompress : " << endl;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> bitSet  = bg->getPureBitset(62,false);
			WAHStructure * st = new WAHStructure();
			cout << "Original Bitset : " <<bitSet <<endl;
			//st->buildArray(bitSet);
			st->CompressWords(bitSet);
			cout << "Compressed Words : " << endl;
			st->printCompressedStream();
			dynamic_bitset<> bitSet2 = st->Decompress();
			cout << "decompressed Structure  : " << bitSet2<<endl;
			bitSet2 = bitSet & bitSet2;
			cout << "Count : " << bitSet2.to_ulong() << endl;

			delete bg,st;
		}

		bool TestWAHArray::testAccuracy( dynamic_bitset<> leftOp, dynamic_bitset<> rightOp,operation_type type /*= AND*/ )
		{
			dynamic_bitset<> resultBitMap ; 
			if (type == AND)
			{
				resultBitMap = leftOp & rightOp;
			}else{
				resultBitMap = leftOp | rightOp;
			}

			WAHStructure * left = new WAHStructure();
			WAHStructure * right = new WAHStructure();
			//left->buildArray(leftOp);
			//right->buildArray(rightOp);
			left->CompressWords(leftOp);
			right->CompressWords(rightOp);
			cout << "Compressed Word left : "<< endl;
			left->printCompressedStream();
			cout << ""<< endl;
			cout << "Compressed Word right : "<< endl;
			right->printCompressedStream();
			cout << "" << endl;
			WAHStructure * result = NULL;
			if (type == AND)
			{
				result = *(left) & *(right);		
			}else
			{
				result = *(left) | *(right);		
			}
			cout << "Result : " << endl;
			result->printCompressedStream();
			dynamic_bitset<> decompressedResult = result->Decompress();
			//dynamic_bitset<> testAbleBitmap = resultBitMap & decompressedResult;
			bool state = (resultBitMap == decompressedResult);			
			assert(state);
			delete left,right,result;
			return state;
		}

		void TestWAHArray::EncodeDecodeTest(){
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> temp = bg->getPureBitset(35,true);
			dynamic_bitset<> temp1 = temp; 
			WAHStructure * ws = new WAHStructure();
			cout << "original Bitmap : " << temp << endl;

			//ws->buildArray(temp1);
			ws->CompressWords(temp1);
			ws->printCompressedStream();	

			dynamic_bitset<> decoded = ws->Decompress();
			if (decoded == temp)
			{
				cout << "Decompress Success" << endl;
			}else
			{
				cout << "Decoded Bitmap : " << decoded << endl;	
				cout << "Decompress Unsuccess" << endl;
				cout << "Temp Size : " << temp.size() << endl;
				cout << "Decoded Size : " << decoded.size() << endl;
			}
			delete ws,bg;
		}

		void TestWAHArray::testLeftCompressedRightLiteralTestZero(){
			cout << "Test : Left Compressed Right Literal (Zero)" << endl;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 2,false);
			dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
			printOperands(x,y);
			bool resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,false);
			y = bg->getAlternatingBitset(31*3);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			y = bg->getAlternatingBitset(5*31);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,false);
			y = bg->getAlternatingBitset(5*31);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;
			delete bg;
		}

		void TestWAHArray::testRightCompressedLeftLiteralTestZero(){
			cout << "Test : Right Compressed Left Literal (Zero)" << endl;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> y = bg->getPureBitset(31 * 2,false);
			dynamic_bitset<> x = bg->getAlternatingBitset(31*2);
			printOperands(x,y);
			bool resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,false);
			y = bg->getAlternatingBitset(31*3);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);			
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			x = bg->getAlternatingBitset(5*31);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			y = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,false);
			x = bg->getAlternatingBitset(5*31);
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			x = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			x = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			x = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;
			delete bg;
		}

		void TestWAHArray::pureBiteTestDecompressionTest()
		{
			cout << "Test : Pure Bit Test Decompression Test" << endl;
			BitsetGenerator *bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
			dynamic_bitset<> y = bg->getPureBitset(31 * 5,false);
			bool test = testAccuracy(x,y);
			printOperands(x,y);
			cout << "Test 1" << endl;
			cout << "Accuracy : " << test<<endl;
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,false);
			y = bg->getPureBitset(31 * 5,true);
			cout << "Test 2" << endl;
			test  = testAccuracy(x,y);
			testAccuracy(x,y,OR);

			printOperands(x,y);
			cout << "Accuracy : " << test<<endl;
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,true);
			cout << "Test 3" << endl;
			test  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			printOperands(x,y);
			cout << "Accuracy : " << test<<endl;
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,false);
			cout << "Test 4" << endl;
			test  = testAccuracy(x,y);
			testAccuracy(x,y,OR);
			printOperands(x,y);
			cout << "Accuracy : " << test<<endl;
			cout << ""<< endl;

			delete bg;
		}

		void TestWAHArray::testLeftCompressedRightLiteralTestOne(){
			cout << "Test : Left Compressed Right Literal(One)" << endl;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 2,true);
			dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
			cout << "Test : 1"<<endl;
			printOperands(x,y);
			bool resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;	
			cout << ""<< endl;

			cout << "Test : 2"<<endl;
			x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,true);
			y = bg->getAlternatingBitset(31*3);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 3"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			y = bg->getAlternatingBitset(5*31);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 4"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 5"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,true);
			y = bg->getAlternatingBitset(5*31);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 6"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,true) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 7"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,true) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 8"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,true) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;
			delete bg;
		}

		void TestWAHArray::testRightCompressedLeftLiteraTestOne(){
			cout << "Test : Right Compressed left Literal (One)" << endl;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getAlternatingBitset(31*2);
			dynamic_bitset<> y = bg->getPureBitset(31 * 2,true);
			cout << "Test : 1"<<endl;
			printOperands(x,y);
			bool resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 2"<<endl;
			x = bg->getAlternatingBitset(31*3);
			y = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,true);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 3"<<endl;
			x = bg->getAlternatingBitset(5*31);
			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 4"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 5"<<endl;
			x = bg->getAlternatingBitset(5*31);
			y = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,true);
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 6"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,true) & bg->getAlternatingBitset(6*31);
			y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 7"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,true) & bg->getAlternatingBitset(6*31);
			y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 8"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,true) & bg->getAlternatingBitset(6*31);
			y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			printOperands(x,y);
			resul  = testAccuracy(x,y);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;
			delete bg;
		}


		void TestWAHArray::printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp)
		{
			cout << "Left Operand : " << leftOp << endl;
			cout << "Right Operand : " << rightOp << endl;
		}

		void TestWAHArray::pureBitTest()
		{
			cout << "Test : Pure Bit Test" << endl;
			BitsetGenerator *bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
			dynamic_bitset<> y = bg->getPureBitset(31 * 5,false);
			RunTest(x,y,1);
			RunTest(x,y,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,false);
			y = bg->getPureBitset(31 * 5,true);
			RunTest(x,y,2);
			RunTest(x,y,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,true);
			RunTest(x,y,3);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,false);
			RunTest(x,y,4);
			cout << ""<< endl;

			delete bg;
		}

		void TestWAHArray::pureBitORTest()
		{
			cout << "Test : Pure Bit OR Test" << endl;
			BitsetGenerator *bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
			dynamic_bitset<> y = bg->getPureBitset(31 * 5,false);
			RunTest(x,y,1,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,false);
			y = bg->getPureBitset(31 * 5,true);
			RunTest(x,y,2,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,true);
			RunTest(x,y,3,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,false);
			RunTest(x,y,4,OR);
			cout << ""<< endl;

			delete bg;
		}

		void TestWAHArray::testLeftCompressedRightLiteral(){
			cout << "Test : Left Compressed Right Literal" << endl;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 2,false);
			dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
			RunTest(x,y,1);
			RunTest(x,y,1,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,false);
			y = bg->getAlternatingBitset(31*3);
			RunTest(x,y,2);
			RunTest(x,y,2,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			y = bg->getAlternatingBitset(5*31);
			RunTest(x,y,3);
			RunTest(x,y,3,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
			RunTest(x,y,4);
			RunTest(x,y,4,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,false);
			y = bg->getAlternatingBitset(5*31);
			RunTest(x,y,5);
			RunTest(x,y,5,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			RunTest(x,y,6);
			RunTest(x,y,6,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			RunTest(x,y,7);
			RunTest(x,y,7,OR);
			cout << ""<< endl;

			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
			y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,false) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			RunTest(x,y,8);
			RunTest(x,y,8,OR);
			cout << ""<< endl;
			delete bg;
		}


		void TestWAHArray::testRandomBitStream(int startingLength,int stopLength,int percentage)
		{
			BitsetGenerator bg;
			dynamic_bitset<> leftOp,rightOp;
			dynamic_bitset<> leftOpCopy,rightOpCopy;

			for (int length = startingLength; length < stopLength ; length *=10)
			{
				cout << "Test Length: "<< length<< endl;

				leftOp = bg.getRandomBitStream(length,percentage);	
				//cout << "left operand : "<<leftOp << endl;
				rightOp = bg.getRandomBitStream(length,percentage);
				//cout << "right operand : "<<rightOp << endl;
				testAccuracy(leftOp,rightOp);
				testAccuracy(leftOp,rightOp,OR);
				cout << "Test END: "<< length<< endl;
				cout << ""<< endl;
			}

		}

		void TestWAHArray::RunTest( dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,operation_type type /*= operation_type.AND*/ )
		{
			cout << "Test No : " << TestNo << endl;
			WAHStructure * leftOp = new WAHStructure();
			WAHStructure * rightOp = new WAHStructure();
			leftOp->CompressWords(leftoperand);
			rightOp->CompressWords(rightOperand);
			cout << "Print Left Compressed Stream : " << endl;
			leftOp->printCompressedStream();
			cout << "print Right Compressed Stream : " << endl;
			rightOp->printCompressedStream();
			WAHStructure * result = NULL;

			if (type == AND)
			{
				cout << "AND test : " << endl;
				result = *(leftOp) & *(rightOp);
			}else {
				cout << "OR test : " << endl;
				result = *(leftOp) | *(rightOp);
			}
			cout << "Test Result : " << endl;
			result->printCompressedStream();


			delete leftOp,rightOp,result;	
		}

		void TestWAHArray::OperatorTest()
		{
			//Test contiguous streams entirely consisting of 1's and 0's
			pureBitTest();

			//LO = 11000000000000...11 RO =001010111000...0 Left operand Compressed(contiguous ones) Right operand not compressed
			testLeftCompressedRightLiteralTestOne();

			//LO = 001010111000...0 RO = 11000000000000...11 Right operand Compressed(contiguous ones) Left operand not compressed
			testRightCompressedLeftLiteraTestOne();

			//LO = 10000000000000...11 RO = 001010111000...0 Left operand Compressed(contiguous zeros) Right operand not compressed
			testLeftCompressedRightLiteralTestZero();

			//LO = 10000000000000...11 RO = 001010111000...0 Left operand Compressed(contiguous zeros) Right operand not compressed
			testRightCompressedLeftLiteralTestZero();
		}

		void TestWAHArray::testBitCount()
		{
			BitsetGenerator bg;
			for (int i = 0; i < 10 ; i++)
			{
				cout <<"Test : "<< i << endl;
				for (int j = 0; j < 110 ; j += 10)
				{
					cout << "Percentage : " << j << endl;
					dynamic_bitset<> bitSet = bg.getRandomBitStream(100*i,j);
					bitCountAccuracy(bitSet);
					cout << "" << endl;
				}
				cout << "" << endl;
			}

		}
		bool TestWAHArray::bitCountAccuracy(dynamic_bitset<>& bitSet)
		{
			WAHStructure ws;
			unsigned long long count = (unsigned long long)bitSet.count();
			ws.CompressWords(bitSet);
			unsigned long long countCompressed = ws.Count();
			cout << "Bitset count : " << count << endl;
			cout << "Compressed word count" << countCompressed<<endl;
			assert(countCompressed == count);
			return true;
		}

		bool TestWAHArray::complementAccuracy(dynamic_bitset<> & bitset)
		{
			/*WAHStructure * ws = new WAHStructure();
			dynamic_bitset<> comp = bitset;
			cout << "Original Stream : " << bitset << endl;
			cout << "Original Compressed Stream : " <<endl;
			ws->CompressWords(comp);
			ws->printCompressedStream();
			bitset.flip();
			cout <<"Complement Bitstream : " << bitset << endl;

			WAHStructure * ws2 = (~(*(ws)));
			cout << "Complement of the structure" << endl;
			ws2->printCompressedStream();
			cout << "Starting to decompress " << endl;
			dynamic_bitset<> decom = ws2->Decompress();
			cout << "Decompressed negation : " << decom << endl;
			assert(decom == bitset);*/
			return true;
		}

		void TestWAHArray::testComplementStrings(vector<string> & stringColl)
		{
			/*BitsetGenerator bg;

			vector<string>::iterator it = stringColl.begin();
			while(it != stringColl.end())
			{
				dynamic_bitset<> temp = bg.buildFromString(*(it));
				dynamic_bitset<> tempClone = temp; 
				it++;
				WAHStructure  * ws = new WAHStructure();
				cout << "Original Bit Stream : " << temp << endl;
				ws->CompressWords(temp);
				cout << "Compressed Words : " << endl;
				ws->printCompressedStream();
				ws = ~(*(ws));
				tempClone.flip();
				dynamic_bitset<> t2 = ws->Decompress();
				cout << "Decompressed Stream : " << t2 << endl;
				assert(t2 == tempClone);
			}*/
		}

		void TestWAHArray::failedCases()
		{
			vector<string> strVector;			
			string str = "111111111111111111111111111111100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111";			
			string str2 = "010101010101010101010101010101010101010101010101010101010101010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010101010101010101010101010101";

			BitsetGenerator bg;
			dynamic_bitset<> st1 =  bg.buildFromString(str);
			dynamic_bitset<> st2 =  bg.buildFromString(str2);

			WAHStructure  * ws2 = new WAHStructure();
			ws2->CompressWords(st1);
			WAHStructure  * ws1 = new WAHStructure();
			ws1->CompressWords(st2);

			WAHStructure * result = new WAHStructure();
			result = *(ws2) | *(ws1);


		}

		

		void TestWAHArray::testComplement()
		{
			BitsetGenerator bg;
			dynamic_bitset<> bitSet;

			for (int i= 1; i < 10 ; i++)
			{
				cout << "Negation length : " << i << endl;
				for (int j = 0 ; j < 110 ; j+= 10)
				{
					cout << "Percentage : " << j << endl;
					bitSet = bg.getRandomBitStream(100 * i,j);
					complementAccuracy(bitSet);
				}

			}

		}
	}}