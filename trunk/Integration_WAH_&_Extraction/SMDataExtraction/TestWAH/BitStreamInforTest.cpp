#include "StdAfx.h"
#include "BitStreamInforTest.h"

namespace CompressedStructure{
	namespace TestStructure{
		TestBitStreamInfo::TestBitStreamInfo(void)
		{
		}

		TestBitStreamInfo::~TestBitStreamInfo(void)
		{
		}
		bool TestBitStreamInfo::EncodeDecodeTest(dynamic_bitset<> & _bit_map)
		{
			dynamic_bitset<> copy_of_bitmap = _bit_map;		
			BitStreamInfo * super_test = new WAHStructure();
			cout << "Original Bitmap : " << copy_of_bitmap << endl;
			super_test->CompressWords(copy_of_bitmap);
			WAHStructure * ws = dynamic_cast<WAHStructure *>(super_test);
			cout << "Compressed Words : " << endl;
			ws->printCompressedStream();
			dynamic_bitset<> decompressed_word = super_test->Decompress();
			cout << "Decompressed Words : " << decompressed_word<<endl;
			bool state = decompressed_word == _bit_map;
			assert(state);

			delete super_test,ws;
			return true;
		}

		void TestBitStreamInfo::PureBistDecompression()
		{
			cout << "Test : Pure Bit Decompression Test" << endl;
			BitsetGenerator *bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
			EncodeDecodeTest(x);	
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);			
			EncodeDecodeTest(x);	
			cout << ""<< endl;	

			delete bg;
		}

		void TestBitStreamInfo::decompressionTestSuite()
		{
			PureBistDecompression();
			BitDecompression();
			testComplement();
		}

		void TestBitStreamInfo::BitDecompression()
		{
			BitsetGenerator bg;
			dynamic_bitset<> x = bg.getMiddleCompressedBitset(3*31,31,3*31 -1,false);			
			EncodeDecodeTest(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(3*31,31,3*31 -1,true);			
			EncodeDecodeTest(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(5*31,31,4*31 -1,false);			
			EncodeDecodeTest(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(5*31,31,4*31 -1,true);			
			EncodeDecodeTest(x);
			cout << ""<< endl;	

			x = bg.getMiddleCompressedBitset(5*31,0,4*31 -1,false);			
			EncodeDecodeTest(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(5*31,0,4*31 -1,true);			
			EncodeDecodeTest(x);
			cout << ""<< endl;			

			x = bg.getMiddleCompressedBitset(6*31,31,5*31 -1,false);			
			EncodeDecodeTest(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			EncodeDecodeTest(x);
			cout << ""<< endl;
		}

		void TestBitStreamInfo::testComplement()
		{
			BitsetGenerator bg;
			dynamic_bitset<> x = bg.getMiddleCompressedBitset(3*31,31,3*31 -1,false);			
			complementAccuracy(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(3*31,31,3*31 -1,true);			
			complementAccuracy(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(5*31,31,4*31 -1,false);			
			complementAccuracy(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(5*31,31,4*31 -1,true);			
			complementAccuracy(x);
			cout << ""<< endl;	

			x = bg.getMiddleCompressedBitset(5*31,0,4*31 -1,false);			
			complementAccuracy(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(5*31,0,4*31 -1,true);			
			complementAccuracy(x);
			cout << ""<< endl;


			x = bg.getMiddleCompressedBitset(6*31,31,5*31 -1,false);			
			complementAccuracy(x);
			cout << ""<< endl;

			x = bg.getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			complementAccuracy(x);
			cout << ""<< endl;
		}

		bool TestBitStreamInfo::complementAccuracy(dynamic_bitset<> & bitset)
		{
			BitStreamInfo * bs = new WAHStructure();
			dynamic_bitset<> comp = bitset;
			WAHStructure * ws = dynamic_cast<WAHStructure *>(bs);
			cout << "Original Stream : " << bitset << endl;
			cout << "Original Compressed Stream : " <<endl;
			bs->CompressWords(comp);
			ws->printCompressedStream();
			bitset.flip();
			cout <<"Complement Bitstream : " << bitset << endl;

			BitStreamInfo * bs2 = (~(*(bs)));
			cout << "Complement of the structure" << endl;
			WAHStructure * ws2 = dynamic_cast<WAHStructure *>(bs2);
			ws2->printCompressedStream();
			cout << "Starting to decompress " << endl;
			dynamic_bitset<> decom = bs2->Decompress();
			cout << "Decompressed negation : " << decom << endl;
			assert(decom == bitset);
			return true;
		}

		void TestBitStreamInfo::OperatorTest()
		{
			//Test contiguous streams entirely consisting of 1's and 0's
			pureBitTest();

			//LO = 11000000000000...11 RO =001010111000...0 Left operand Compressed(contiguous ones) Right operand not compressed
			testLeftCompressedRightLiteralTestOne();

			//LO = 001010111000...0 RO = 11000000000000...11 Right operand Compressed(contiguous ones) Left operand not compressed
		//	testRightCompressedLeftLiteraTestOne();

			//LO = 10000000000000...11 RO = 001010111000...0 Left operand Compressed(contiguous zeros) Right operand not compressed
		//	testLeftCompressedRightLiteralTestZero();

			//LO = 10000000000000...11 RO = 001010111000...0 Left operand Compressed(contiguous zeros) Right operand not compressed
		//	testRightCompressedLeftLiteralTestZero();
		}
		
		void TestBitStreamInfo::pureBitTest()
		{
			cout << "Test : Pure Bit Test" << endl;
			BitsetGenerator *bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
			dynamic_bitset<> y = bg->getPureBitset(31 * 5,false);
			RunTest(x,y,1,AND);
			RunTest(x,y,1,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,false);
			y = bg->getPureBitset(31 * 5,true);
			RunTest(x,y,2,AND);
			RunTest(x,y,2,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,true);
			RunTest(x,y,3,AND);
			RunTest(x,y,3,OR);
			cout << ""<< endl;

			x = bg->getPureBitset(31 * 5,true);
			y = bg->getPureBitset(31 * 5,false);
			RunTest(x,y,4,AND);
			RunTest(x,y,4,OR);
			cout << ""<< endl;

			delete bg;
		}

		void TestBitStreamInfo::testLeftCompressedRightLiteralTestOne(){
			cout << "Test : Left Compressed Right Literal(One)" << endl;
			TestWAHArray wahTest;
			BitsetGenerator * bg = new BitsetGenerator();
			dynamic_bitset<> x = bg->getPureBitset(31 * 2,true);
			dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
			cout << "Test : 1"<<endl;
			wahTest.printOperands(x,y);
			bool resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;	
			cout << ""<< endl;

			cout << "Test : 2"<<endl;
			x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,true);
			y = bg->getAlternatingBitset(31*3);
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 3"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			y = bg->getAlternatingBitset(5*31);
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 4"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 5"<<endl;
			x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,true);
			y = bg->getAlternatingBitset(5*31);
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 6"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,true) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 7"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,true) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;

			cout << "Test : 8"<<endl;
			x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
			y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,true) & bg->getAlternatingBitset(6*31);
			//cout <<"Result Size : " <<y.size()<< endl;
			//cout << "Bit stream : " << y << endl;
			wahTest.printOperands(x,y);
			resul  = testAccuracy(x,y,AND);			
			testAccuracy(x,y,OR);
			cout << "Accuracy : "<< resul<<endl;
			cout << ""<< endl;
			delete bg;
		}

		bool TestBitStreamInfo::testAccuracy( dynamic_bitset<> leftOp, dynamic_bitset<> rightOp,operation_type type /*= AND*/ )
		{
			dynamic_bitset<> resultBitMap ; 
			if (type == AND)
			{
				resultBitMap = leftOp & rightOp;
			}else{
				resultBitMap = leftOp | rightOp;
			}

			BitStreamInfo * left = new WAHStructure();
			BitStreamInfo * right = new WAHStructure();
			//left->buildArray(leftOp);
			//right->buildArray(rightOp);
			WAHStructure * left_op = dynamic_cast<WAHStructure *>(left);
			WAHStructure * right_op = dynamic_cast<WAHStructure *>(right);
			left->CompressWords(leftOp);
			right->CompressWords(rightOp);

			cout << "Compressed Word left : "<< endl;
			left_op->printCompressedStream();
			cout << ""<< endl;
			
			cout << "Compressed Word right : "<< endl;
			right_op->printCompressedStream();
			cout << "" << endl;
			BitStreamInfo * result = NULL;
			if (type == AND)
			{
				result = *(left) & *(right);		
			}else
			{
				result = *(left) | *(right);		
			}
			cout << "Result : " << endl;
			WAHStructure * _result = dynamic_cast<WAHStructure *>(result);
			_result->printCompressedStream();
			dynamic_bitset<> decompressedResult = result->Decompress();
			//dynamic_bitset<> testAbleBitmap = resultBitMap & decompressedResult;
			bool state = (resultBitMap == decompressedResult);			
			assert(state);
			delete left,right,result;
			return state;
		}

		void TestBitStreamInfo::RunTest( dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,operation_type type )
		{
			cout << "Test No : " << TestNo << endl;
			BitStreamInfo * leftOp = new WAHStructure();
			BitStreamInfo * rightOp = new WAHStructure();
			leftOp->CompressWords(leftoperand);
			rightOp->CompressWords(rightOperand);
			cout << "Print Left Compressed Stream : " << endl;
			WAHStructure * left_op = dynamic_cast<WAHStructure *>(leftOp);
			WAHStructure * right_op = dynamic_cast<WAHStructure *>(rightOp);
			left_op->printCompressedStream();
			cout << "print Right Compressed Stream : " << endl;
			right_op->printCompressedStream();
			BitStreamInfo * result = NULL;

			if (type == AND)
			{
				cout << "AND test : " << endl;
				result = *(leftOp) & *(rightOp);
			}else {
				cout << "OR test : " << endl;
				result = *(leftOp) | *(rightOp);
			}
			cout << "Test Result : " << endl;
			WAHStructure * _result = dynamic_cast<WAHStructure *>(result);
			_result->printCompressedStream();

			delete leftOp,rightOp,result;	
		}
	}
}