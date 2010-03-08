#pragma once
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <iostream>
#include "BitStreamInfo.h"
#include "WAHStructure.h"
#include "BitsetGenerator.h"
#include "TestWAHArray.h"

using namespace std;
using namespace boost;
namespace CompressedStructure{
	namespace TestStructure{
		class TestBitStreamInfo
		{
			enum operation_type{OR,AND};
		public:
			TestBitStreamInfo(void);
			~TestBitStreamInfo(void);
			bool EncodeDecodeTest(dynamic_bitset<> & _bit_map);
			void PureBistDecompression();
			void BitDecompression();
			void decompressionTestSuite();
			bool complementAccuracy(dynamic_bitset<> & bitset);
			void testComplement();
			void testLeftCompressedRightLiteralTestOne();			
			void OperatorTest();
			void pureBitTest();
			bool testAccuracy( dynamic_bitset<> leftOp, dynamic_bitset<> rightOp,operation_type type /*= AND*/ );
			void RunTest( dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,operation_type type );
		};
	}
}