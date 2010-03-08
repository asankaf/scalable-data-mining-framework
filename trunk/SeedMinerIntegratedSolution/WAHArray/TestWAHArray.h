#pragma once
#include "WAHStructure.h"
namespace CompressedStructure{
	namespace TestStructure{
		class TestWAHArray
		{
			enum operation_type{OR,AND};
		public:
			
			TestWAHArray(void);
			~TestWAHArray(void);
			void RunTest(dynamic_bitset<> &_left_operand,dynamic_bitset<> & _right_operand,int _itest_no,operation_type type = AND);
			void pureBitTest();
			void pureBitORTest();
			void testLeftCompressedRightLiteral();
			void testDecompress();
			bool testAccuracy(dynamic_bitset<> leftOp, dynamic_bitset<> rightOp,operation_type type = AND);
			void pureBiteTestDecompressionTest();
			void printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp);
			void testLeftCompressedRightLiteralTestZero();
			void testLeftCompressedRightLiteralTestOne();
			void EncodeDecodeTest();
			void testRightCompressedLeftLiteraTestOne();
			void OperatorTest();
			void testRightCompressedLeftLiteralTestZero();
			bool bitCountAccuracy(dynamic_bitset<>& bitSet);
			void testRandomBitStream(int startingLength,int stopLength,int percentage);	
			void testBitCount();
			bool complementAccuracy(dynamic_bitset<> & bitset);
			void testComplement();
			void testComplementStrings(vector<string>  & stringColl);
			void failedCases();
			enum TEST_TYPE{AND_TEST,OR_TEST,NOT_TEST,COUNT_TEST};

		};
	}
}