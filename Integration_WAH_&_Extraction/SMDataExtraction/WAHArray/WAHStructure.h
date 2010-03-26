#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "Math.h"
#include "BitStreamInfo.h"
#include <vector>
#include "Math.h"
#include <iostream>

using namespace std;
using namespace  boost;
namespace CompressedStructure{
	class WAHStructure:public BitStreamInfo
	{

	public:
		_declspec(dllexport) WAHStructure(void);	
		_declspec(dllexport) ~WAHStructure(void);	
		
		//Methods inherited from BitstreamInfo class		
		_declspec(dllexport) void CompressWords(boost::dynamic_bitset<>& _bit_map);		
		_declspec(dllexport) dynamic_bitset<> Decompress();				
		_declspec(dllexport) int GetSpaceUtilisation();
		_declspec(dllexport) BitStreamInfo* operator~();
		_declspec(dllexport) virtual BitStreamInfo* operator &(BitStreamInfo &);
		_declspec(dllexport) BitStreamInfo* operator |(BitStreamInfo &);
		_declspec(dllexport) unsigned long long Count();	
		_declspec(dllexport) size_t SpaceUtilisation();
		
		// Overloaded operators
	
		_declspec(dllexport) WAHStructure * operator & (WAHStructure&);
		_declspec(dllexport) WAHStructure * operator |(WAHStructure & _structure);

		//Methods not exposed to public interface but used among the structures	
	
		_declspec(dllexport) void SetCompressedStream(vector<unsigned long int> &_compressed_vector);			
		_declspec(dllexport) void printCompressedStream();
		_declspec(dllexport) vector<unsigned long int>& GetCompressedVector();
		_declspec(dllexport) unsigned long int GetOriginalStreamLength();

		//Public Getters and setters
		_declspec(dllexport) const int ActiveWordSize() const { return m_iActiveWordSize; }
		_declspec(dllexport) void ActiveWordSize(int val) { m_iActiveWordSize = val; }
		_declspec(dllexport) unsigned long int ActiveWord() const { return m_ulActiveWord; }
		_declspec(dllexport) void ActiveWord(unsigned long int val) { m_ulActiveWord = val; }
		_declspec(dllexport) int OriginalStreamSize() const { return m_iOriginalStreamSize; }
		_declspec(dllexport) void OriginalStreamSize(unsigned long int val) { m_iOriginalStreamSize = val; }


		enum operation_type {AND,OR};

		//Constants
		static const unsigned long int ZERO_LITERAL = 0;
		static const unsigned long int ONE_LITERAL = 2147483647;
		static const unsigned long int ONE_GAP_START_FLAG = 3221225472;
		static const unsigned long int ZERO_GAP_START_FLAG = 2147483648;
		static const int LITERAL_WORD = 0;
		static const int ZERO_GAP_WORD = 1;
		static const int ONE_GAP_WORD = 2;
		

	private:

		enum longer_operand{ONE_GAP,ZERO_GAP};
		void BuildArray(boost::dynamic_bitset<>& _bitmap);
		dynamic_bitset<> ConvertVectorToBitmap(vector<unsigned long int> &_decompressed_vector);
		vector<unsigned long int> ExpandCompressedVector(vector<unsigned long int>& _compressed_vector);
		vector<int> AlignRightWithLeft(int _left_op_end_pos,int _right_op_index,int _right_op_end_pos,vector<unsigned long int> &_right_operand,vector<unsigned long int> &_result_vector,longer_operand _longer,operation_type _type,bool _gap_state);	
		vector<int> AlignLeftWithRight(int _right_op_end_pos,int _left_op_index,int _left_op_end_pos,vector<unsigned long int> &_left_operand,vector<unsigned long int> &_result_vector, longer_operand _longer,operation_type _type ,bool _gap_extension_state);	
		void SetOneCount(unsigned long int _count,vector<unsigned long int> & _compressed);
		void SetZeroCount(unsigned long int _count,vector<unsigned long int> & _compressed);
		void SetLiteral(unsigned long int _literal_val,vector<unsigned long int> & _compressed);
		void CopyIntegerToBitMap(dynamic_bitset<> &_bitmap,size_t _index);
		int GetStartBitValue(size_t _word);	
		void SetValueOnCompressedWord(unsigned long int _word, vector<unsigned long int> &_compressed_result);			
		unsigned long int GetLiteralCount(unsigned long int &_bit_literal);
	

		//Private Attributes
		vector<unsigned long int> m_compressed_stream;	
		int * m_iMainArray;
		unsigned long int m_iOriginalStreamSize;			
		unsigned long int m_ulActiveWord;	
		int m_iActiveWordSize;	
		static const int WORD_SIZE = 32;		

	};
}