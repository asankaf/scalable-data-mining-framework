#include "StdAfx.h"
#include "WAHStructure.h"

using namespace std;
namespace CompressedStructure{
	WAHStructure::WAHStructure(void)
	{
		m_iActiveWordSize = 0;
		m_iMainArray = NULL;
		m_iOriginalStreamSize = 0;
		m_ulActiveWord = 0;
		Type(BitStreamInfo::WAH_COMPRESSION);
	}
	WAHStructure::~WAHStructure(void)
	{
	}

	size_t WAHStructure::SpaceUtilisation()
	{
		size_t space = sizeof(this);		
		space += (m_compressed_stream.size() * sizeof(unsigned long int));
		return space;
	}
	void WAHStructure::BuildArray(boost::dynamic_bitset<>& _bit_map)
	{
		//count = bitMap.count();
		m_iOriginalStreamSize = (int)floor(((float)_bit_map.size()/(float)(WORD_SIZE-1)));
		m_iMainArray = new int[m_iOriginalStreamSize];
		m_iActiveWordSize = _bit_map.size() - m_iOriginalStreamSize * (WORD_SIZE-1);

		if (m_iActiveWordSize > 0)
		{
			boost::dynamic_bitset<> active_word_bit_set (m_iActiveWordSize);		
			int k=0;
			for (size_t i = _bit_map.size()-m_iActiveWordSize; i < _bit_map.size() ; i++)
			{
				active_word_bit_set[k++] = _bit_map[i];
			}		
			_bit_map.resize(m_iOriginalStreamSize*(WORD_SIZE-1));
			m_ulActiveWord = active_word_bit_set.to_ulong();
		}
		boost::dynamic_bitset<> temp(32);
		int main_array_index= 0;
		int counter = 0;
		for (size_t i = 0; i < _bit_map.size() ;)
		{		
			for (int j=0; j < WORD_SIZE-1; j++)
			{
				temp[j]= _bit_map[i];	

				i++;
			}

			m_iMainArray[main_array_index++] = temp.to_ulong();		
		}
	}

	BitStreamInfo * WAHStructure::Clone()
	{
		WAHStructure * structure = new WAHStructure();
		structure->m_compressed_stream = m_compressed_stream;	
		structure->m_iOriginalStreamSize = m_iOriginalStreamSize;
		structure->m_ulActiveWord = m_ulActiveWord;
		structure->m_iActiveWordSize = m_iActiveWordSize;			
		structure->Type(this->Type());
		BitStreamInfo::Clone(structure);

		return structure;
	}
	unsigned long long WAHStructure::Count()
	{
		vector<unsigned long int>::iterator value_iterator = m_compressed_stream.begin();
		unsigned long long count_val = 0;
		while(value_iterator != m_compressed_stream.end())
		{
			unsigned long int temp_value = *(value_iterator);
			int start_bit = GetStartBitValue(temp_value);
			if (LITERAL_WORD == start_bit)
			{
				count_val += GetLiteralCount(temp_value);
			}else if (ONE_GAP_WORD == start_bit)
			{
				count_val += ((WORD_SIZE -1)*(temp_value - ONE_GAP_START_FLAG));
			}
			value_iterator++;
		}

		if (ActiveWordSize() > 0)
		{
			unsigned long int active  = ActiveWord();
			count_val += GetLiteralCount(active);
		}
		return count_val;
	}


	unsigned long int WAHStructure::GetLiteralCount(unsigned long int &_bit_literal)
	{
		unsigned long int result = 0;
		typedef const unsigned char type_byte;
		type_byte * pbyte_ptr = static_cast<type_byte *>(static_cast<const void *>(&_bit_literal));
		int length = sizeof(_bit_literal);
		while(length)
		{
			//result += boost::detail::dynamic_bitset_count_impl::count_table<>::table[*(pbyte_ptr)];
			result += boost::detail::dynamic_bitset_impl::count_table<>::table[*(pbyte_ptr)];
			pbyte_ptr++;
			length--;
		}

		return result;
	}


	void WAHStructure::CompressWords( boost::dynamic_bitset<>& _bit_map )
	{	
		size_t i = 0;
		int temp_index = 0;
		int one_count = 0;
		int zero_count = 0;

		BuildArray(_bit_map);
		while(i < m_iOriginalStreamSize){
			if (m_iMainArray[i] == 0)
			{
				zero_count++;
				if (one_count > 1)
				{
					SetOneCount(one_count,m_compressed_stream);
					one_count = 0;
				}else if (one_count == 1)
				{
					SetLiteral(ONE_LITERAL,m_compressed_stream);
					one_count = 0;
				}

			}else if(m_iMainArray[i] == ONE_LITERAL){
				one_count++;
				if (zero_count > 1)
				{
					SetZeroCount(zero_count,m_compressed_stream);
					zero_count = 0;
				}else if (zero_count == 1)
				{
					SetLiteral(0,m_compressed_stream);
					zero_count = 0;
				}
			}else if (one_count != 0)
			{
				if (one_count == 1)
				{
					SetLiteral(ONE_LITERAL,m_compressed_stream);
					one_count =0;
				}else if(one_count > 1){
					SetOneCount(one_count,m_compressed_stream);
					one_count =0;
				}

				SetLiteral(m_iMainArray[i],m_compressed_stream);
			}else if (zero_count != 0)
			{
				if (zero_count == 1)
				{
					SetLiteral(0,m_compressed_stream);
					zero_count =0;	
				}else if (zero_count > 1)
				{				
					SetZeroCount(zero_count,m_compressed_stream);
					zero_count =0;
				}

				SetLiteral(m_iMainArray[i],m_compressed_stream);

			}else{
				SetLiteral(m_iMainArray[i],m_compressed_stream);
			}
			i++;
		}
		if (one_count == 1)
		{
			SetLiteral(ONE_LITERAL,m_compressed_stream);
			one_count = 0;

		}else if (one_count > 1)
		{
			SetOneCount(one_count,m_compressed_stream);
			one_count = 0;
		}else if (zero_count == 1)
		{
			SetLiteral(0,m_compressed_stream);
			zero_count =0;
		}
		else if (zero_count > 1)
		{
			SetZeroCount(zero_count,m_compressed_stream);
			zero_count =0;
		}		
		delete m_iMainArray;
	}

	void WAHStructure::SetOneCount(unsigned long int _count,vector<unsigned long int> & _compressed)
	{
		unsigned long int _comressed_value = _count;
		_comressed_value = _comressed_value | ONE_GAP_START_FLAG;
		_compressed.push_back(_comressed_value);	

	} 

	void WAHStructure::SetZeroCount(unsigned long int _count,vector<unsigned long int> & _compressed)
	{
		unsigned long int _compressed_value = _count;
		_compressed_value = _compressed_value| ZERO_GAP_START_FLAG;
		_compressed.push_back(_compressed_value);
	} 

	void WAHStructure::SetLiteral(unsigned long int _literal_val,vector<unsigned long int> & _compressed)
	{	
		_compressed.push_back(_literal_val);
	}

	void WAHStructure::printCompressedStream()
	{
		if (&m_compressed_stream != NULL && m_compressed_stream.size() > 0)
		{
			dynamic_bitset<> temp(m_compressed_stream.size() * WORD_SIZE);
			for (size_t i  = 0 ; i < m_compressed_stream.size(); i++)
			{
				CopyIntegerToBitMap(temp,i);
			}

			cout << temp << endl;
		}


		if (ActiveWordSize() > 0 )
		{
			unsigned long int act = (unsigned long int) m_ulActiveWord;
			dynamic_bitset<> active_temp(ActiveWordSize(),act);
			cout << "Active word : "<< active_temp <<endl;
		}

	}

	void WAHStructure::CopyIntegerToBitMap( dynamic_bitset<> &_bitmap,size_t _index )
	{
		int bitmap_start_index = _index*WORD_SIZE;
		unsigned long int val = (unsigned long int) m_compressed_stream[_index];
		dynamic_bitset<> temp(32,val);
		for (size_t i = 0;  i < temp.size(); i++)
		{
			_bitmap[bitmap_start_index++] = temp[i];		
		}
	}

	BitStreamInfo * WAHStructure::operator &(BitStreamInfo & _structure)
	{
		WAHStructure * p_left_operand = dynamic_cast<WAHStructure *>(this);
		WAHStructure * p_right_operand = dynamic_cast<WAHStructure *>(&_structure);		
		return *(p_left_operand) & *(p_right_operand);
	}

	BitStreamInfo * WAHStructure::operator |(BitStreamInfo & _base_structure)
	{
		WAHStructure * p_left_operand = dynamic_cast<WAHStructure *>(this);
		WAHStructure * p_right_operand = dynamic_cast<WAHStructure *>(&_base_structure);		
		return *(p_left_operand) | *(p_right_operand);
	}

	WAHStructure * WAHStructure::operator &(WAHStructure & _structure)
	{
		int compressed_stream_length = m_iOriginalStreamSize;
		vector<unsigned long int> right_operand = _structure.GetCompressedVector();
		vector<unsigned long int> left_operand = m_compressed_stream;
		bool right_operand_zero_run = true;
		bool left_operand_zero_run = true;	
		int left_operand_start_position = 0;
		int left_operand_end_position = 0;
		int right_operand_start_position = 0;
		int right_operand_end_position = 0;
		//int compressed_word_value = 0;
		int left_operand_index = 0;
		int right_operand_index = 0;
		int compressed_word_index = 0;
		bool gap_extension  = false;

		WAHStructure * result_structure = new WAHStructure();
		vector<unsigned long int> compressed_result;

		while(left_operand_end_position < compressed_stream_length || right_operand_end_position < compressed_stream_length)
		{
			if (left_operand_end_position == right_operand_end_position)
			{
				int leftOperandStartBitValue = GetStartBitValue(left_operand.at(left_operand_index));
				if (leftOperandStartBitValue == LITERAL_WORD)
				{
					left_operand_end_position++;
					left_operand_start_position = left_operand_end_position;
					int right_operand_start_bit_value = GetStartBitValue(right_operand.at(right_operand_index));

					if (right_operand_start_bit_value == LITERAL_WORD)
					{
						right_operand_end_position++;
						right_operand_start_position = right_operand_end_position;
						SetValueOnCompressedWord(left_operand.at(left_operand_index++) & right_operand.at(right_operand_index++),compressed_result);
					}else if (right_operand_start_bit_value == ONE_GAP_WORD)
					{
						right_operand_end_position += (right_operand.at(right_operand_index) - ONE_GAP_START_FLAG);	
						right_operand_zero_run = false;
					}
					else if (right_operand_start_bit_value == ZERO_GAP_WORD)
					{
						right_operand_end_position += (right_operand.at(right_operand_index) - ZERO_GAP_START_FLAG);	
						right_operand_zero_run = true;
					}

				}else if (leftOperandStartBitValue == ONE_GAP_WORD)
				{
					left_operand_end_position += (left_operand.at(left_operand_index) - ONE_GAP_START_FLAG);
					left_operand_zero_run = false;
					int rightOperandStartBitValue =  GetStartBitValue(right_operand.at(right_operand_index));
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						right_operand_end_position++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						right_operand_end_position += (right_operand.at(right_operand_index) - ONE_GAP_START_FLAG);	
						if (right_operand_end_position == left_operand_end_position)
						{
							SetValueOnCompressedWord(left_operand.at(left_operand_index),compressed_result);
							left_operand_index++;
							right_operand_index++;
						}
						right_operand_zero_run = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						right_operand_end_position += (right_operand.at(right_operand_index) - ZERO_GAP_START_FLAG);	
						if (right_operand_end_position == left_operand_end_position)
						{
							SetValueOnCompressedWord(right_operand.at(right_operand_index),compressed_result);
							left_operand_index++;
							right_operand_index++;
						}
						right_operand_zero_run = true;
					}

				}else if (leftOperandStartBitValue == ZERO_GAP_WORD)
				{
					left_operand_end_position += (left_operand.at(left_operand_index) - ZERO_GAP_START_FLAG);
					left_operand_zero_run = true;
					int rightOperandStartBitValue = GetStartBitValue(right_operand.at(right_operand_index));
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						right_operand_end_position++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						right_operand_end_position += (right_operand.at(right_operand_index) - ONE_GAP_START_FLAG);	
						if (right_operand_end_position == left_operand_end_position)
						{
							SetValueOnCompressedWord(left_operand.at(left_operand_index),compressed_result);
							left_operand_index++;
							right_operand_index++;
						}
						right_operand_zero_run = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						right_operand_end_position += (right_operand.at(right_operand_index) - ZERO_GAP_START_FLAG);	
						if (right_operand_end_position == left_operand_end_position)
						{
							SetValueOnCompressedWord(left_operand.at(left_operand_index),compressed_result);
							left_operand_index++;
							right_operand_index++;
						}
						right_operand_zero_run = true;
					}

				}

			}
			else if (left_operand_end_position > right_operand_end_position)
			{

				if (gap_extension)
				{
					int leftStartBit = GetStartBitValue(left_operand.at(left_operand_index));
					if (leftStartBit == ZERO_GAP_WORD)
					{
						unsigned long int extra_blocks = left_operand_end_position - right_operand_end_position;
						unsigned long int no_of_blocks = left_operand.at(left_operand_index) - ZERO_GAP_START_FLAG;
						unsigned long int new_gap = left_operand.at(left_operand_index) - no_of_blocks + extra_blocks;
						SetValueOnCompressedWord(new_gap,compressed_result);
						vector<int> result = AlignRightWithLeft(left_operand_end_position,right_operand_index,right_operand_end_position,right_operand,compressed_result,ZERO_GAP,AND,true);
						right_operand_index = result[0];
						right_operand_end_position = result[1];
						if (left_operand_end_position == right_operand_end_position)
						{
							left_operand_index++;
							right_operand_index++;
							gap_extension = false;
						}else if(left_operand_end_position < right_operand_end_position)
						{
							gap_extension = true;
						}
					}else if (leftStartBit == ONE_GAP_WORD)
					{
						vector<int> result = AlignRightWithLeft(left_operand_end_position,right_operand_index,right_operand_end_position,right_operand,compressed_result,ONE_GAP,AND,true);
						right_operand_index = result[0];
						right_operand_end_position = result[1];
						if (left_operand_end_position == right_operand_end_position)
						{
							left_operand_index++;
							right_operand_index++;
							gap_extension = false;
						}else if(left_operand_end_position < right_operand_end_position)
						{
							gap_extension = true;
						}
					}


				}else
				{if (left_operand_zero_run)
				{
					SetValueOnCompressedWord(left_operand[left_operand_index],compressed_result);
					vector<int> result = AlignRightWithLeft(left_operand_end_position,right_operand_index,right_operand_end_position,right_operand,compressed_result,ZERO_GAP,AND,false);
					right_operand_index = result[0];
					right_operand_end_position = result[1];
					if (left_operand_end_position == right_operand_end_position)
					{
						left_operand_index++;
						right_operand_index++;
						gap_extension = false;
					}else if(left_operand_end_position < right_operand_end_position)
					{
						gap_extension = true;
					}
				} 
				else
				{
					vector<int> result = AlignRightWithLeft(left_operand_end_position,right_operand_index,right_operand_end_position,right_operand,compressed_result,ONE_GAP,AND,false);
					right_operand_index = result[0];
					right_operand_end_position = result[1];
					if (left_operand_end_position == right_operand_end_position)
					{
						left_operand_index++;
						right_operand_index++;
						gap_extension = false;
					}else if(left_operand_end_position < right_operand_end_position)
					{
						gap_extension = true;
					}

				}
				}
			}else if (left_operand_end_position < right_operand_end_position)
			{
				if (gap_extension)
				{
					int right_start_bit = GetStartBitValue(right_operand.at(right_operand_index));
					if (right_start_bit == ZERO_GAP_WORD)
					{
						unsigned long int extra_blocks  = right_operand_end_position - left_operand_end_position;
						unsigned long int no_of_blocks  = right_operand.at(right_operand_index) - ZERO_GAP_START_FLAG;
						unsigned long int new_gap  = ZERO_GAP_START_FLAG + extra_blocks ;
						SetValueOnCompressedWord(new_gap ,compressed_result);
						vector<int> result = AlignLeftWithRight(right_operand_end_position,left_operand_index,left_operand_end_position,left_operand,compressed_result,ZERO_GAP,AND,gap_extension);
						left_operand_index = result[0];
						left_operand_end_position = result[1];
						if (left_operand_end_position == right_operand_end_position)
						{
							left_operand_index++;
							right_operand_index++;
							gap_extension = false;
						}else if(right_operand_end_position < left_operand_end_position)
						{
							gap_extension = true;
						}
					}else if (right_start_bit == ONE_GAP_WORD)
					{
						vector<int> result = AlignLeftWithRight(right_operand_end_position,left_operand_index,left_operand_end_position,left_operand,compressed_result,ONE_GAP,AND,gap_extension);
						left_operand_index = result[0];
						left_operand_end_position = result[1];
						if (left_operand_end_position == right_operand_end_position)
						{
							left_operand_index++;
							right_operand_index++;
							gap_extension = false;
						}else if(right_operand_end_position < left_operand_end_position)
						{
							gap_extension = true;
						}
					}

				}else
				{if (right_operand_zero_run)
				{
					SetValueOnCompressedWord(right_operand.at(right_operand_index),compressed_result);
					vector<int> result = AlignLeftWithRight(right_operand_end_position,left_operand_index,left_operand_end_position,left_operand,compressed_result,ZERO_GAP,AND,gap_extension);
					left_operand_index = result[0];
					left_operand_end_position = result[1];
					if (left_operand_end_position == right_operand_end_position)
					{
						left_operand_index++;
						right_operand_index++;
						gap_extension = false;
					}else if(right_operand_end_position < left_operand_end_position)
					{
						gap_extension = true;
					}
				} 
				else
				{
					vector<int> result = AlignLeftWithRight(right_operand_end_position,left_operand_index,left_operand_end_position,left_operand,compressed_result,ONE_GAP,AND,gap_extension);
					left_operand_index = result[0];
					left_operand_end_position = result[1];
					if (left_operand_end_position == right_operand_end_position)
					{
						left_operand_index++;
						right_operand_index++;
						gap_extension = false;
					}else if(right_operand_end_position < left_operand_end_position)
					{
						gap_extension = true;
					}
				}
				}
			}
		}

		if (m_iActiveWordSize > 0)
		{
			result_structure->ActiveWordSize(ActiveWordSize());
			result_structure->ActiveWord(m_ulActiveWord & (_structure.ActiveWord()));
		}
		result_structure->SetCompressedStream(compressed_result);
		result_structure->OriginalStreamSize(OriginalStreamSize());
		return result_structure;
	}



	WAHStructure * WAHStructure::operator |(WAHStructure & structure)
	{
		int compressed_stream_length  = m_iOriginalStreamSize;
		vector<unsigned long int> right_operand  = structure.GetCompressedVector();
		vector<unsigned long int> left_operand  = m_compressed_stream;
		bool right_operand_zero_run  = true;
		bool left_operand_zero_run  = true;	
		int left_operand_start_position  = 0;
		int left_operand_end_position  = 0;
		int right_operand_start_position  = 0;
		int right_operand_end_position  = 0;
		//int compressed_word_value  = 0;
		int left_operand_index  = 0;
		int right_operand_index  = 0;
		int compressed_word_index  = 0;
		bool gap_extension    = false;

		WAHStructure * st = new WAHStructure();
		vector<unsigned long int> compressedResult;

		while(left_operand_end_position  < compressed_stream_length  || right_operand_end_position  < compressed_stream_length )
		{
			if (left_operand_end_position  == right_operand_end_position )
			{
				int leftOperandStartBitValue = GetStartBitValue(left_operand [left_operand_index ]);
				if (leftOperandStartBitValue == LITERAL_WORD)
				{
					left_operand_end_position ++;
					left_operand_start_position  = left_operand_end_position ;
					int rightOperandStartBitValue = GetStartBitValue(right_operand [right_operand_index ]);

					if (rightOperandStartBitValue == LITERAL_WORD)
					{
						right_operand_end_position ++;
						right_operand_start_position  = right_operand_end_position ;
						SetValueOnCompressedWord(left_operand [left_operand_index ++] | right_operand [right_operand_index ++],compressedResult);
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						right_operand_end_position  += (right_operand [right_operand_index ] - ONE_GAP_START_FLAG);	
						right_operand_zero_run  = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						right_operand_end_position  += (right_operand [right_operand_index ] - ZERO_GAP_START_FLAG);	
						right_operand_zero_run  = true;
					}

				}else if (leftOperandStartBitValue == ZERO_GAP_WORD)
				{
					left_operand_end_position  += (left_operand [left_operand_index ] - ZERO_GAP_START_FLAG);
					left_operand_zero_run  = true;
					int rightOperandStartBitValue =  GetStartBitValue(right_operand [right_operand_index ]);
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						right_operand_end_position ++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						right_operand_end_position  += (right_operand [right_operand_index ] - ONE_GAP_START_FLAG);	
						if (right_operand_end_position  == left_operand_end_position )
						{
							SetValueOnCompressedWord(right_operand [right_operand_index ],compressedResult);
							left_operand_index ++;
							right_operand_index ++;
						}
						right_operand_zero_run  = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						right_operand_end_position  += (right_operand [right_operand_index ] - ZERO_GAP_START_FLAG);	
						if (right_operand_end_position  == left_operand_end_position )
						{
							SetValueOnCompressedWord(right_operand [right_operand_index ],compressedResult);
							left_operand_index ++;
							right_operand_index ++;
						}
						right_operand_zero_run  = true;
					}

				}else if (leftOperandStartBitValue == ONE_GAP_WORD)
				{
					left_operand_end_position  += (left_operand [left_operand_index ] - ONE_GAP_START_FLAG);
					left_operand_zero_run  = false;
					int rightOperandStartBitValue = GetStartBitValue(right_operand [right_operand_index ]);
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						right_operand_end_position ++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						right_operand_end_position  += (right_operand [right_operand_index ] - ONE_GAP_START_FLAG);	
						if (right_operand_end_position  == left_operand_end_position )
						{
							SetValueOnCompressedWord(left_operand [left_operand_index ],compressedResult);
							left_operand_index ++;
							right_operand_index ++;
						}
						right_operand_zero_run  = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						right_operand_end_position  += (right_operand [right_operand_index ] - ZERO_GAP_START_FLAG);	
						if (right_operand_end_position  == left_operand_end_position )
						{
							SetValueOnCompressedWord(left_operand [left_operand_index ],compressedResult);
							left_operand_index ++;
							right_operand_index ++;
						}
						right_operand_zero_run  = true;
					}

				}

			}
			else if (left_operand_end_position  > right_operand_end_position )
			{

				if (gap_extension  )
				{
					int leftStartBit = GetStartBitValue(left_operand [left_operand_index ]);
					if (leftStartBit == ONE_GAP_WORD)
					{
						unsigned long int blockRun = left_operand_end_position  - right_operand_end_position ;
						unsigned long int gapRunBlocks = left_operand [left_operand_index ] - ONE_GAP_START_FLAG;
						unsigned long int resultGap = left_operand [left_operand_index ] - gapRunBlocks + blockRun;
						SetValueOnCompressedWord(resultGap,compressedResult);
						vector<int> result = AlignRightWithLeft(left_operand_end_position ,right_operand_index ,right_operand_end_position ,right_operand ,compressedResult,ONE_GAP,OR,gap_extension  );
						right_operand_index  = result[0];
						right_operand_end_position  = result[1];
						if (left_operand_end_position  == right_operand_end_position )
						{
							left_operand_index ++;
							right_operand_index ++;
							gap_extension   = false;
						}else if(left_operand_end_position  < right_operand_end_position )
						{
							gap_extension   = true;
						}
					}else if (leftStartBit == ZERO_GAP_WORD)
					{
						vector<int> result = AlignRightWithLeft(left_operand_end_position ,right_operand_index ,right_operand_end_position ,right_operand ,compressedResult,ZERO_GAP,OR,gap_extension  );
						right_operand_index  = result[0];
						right_operand_end_position  = result[1];
						if (left_operand_end_position  == right_operand_end_position )
						{
							left_operand_index ++;
							right_operand_index ++;
							gap_extension   = false;
						}else if(left_operand_end_position  < right_operand_end_position )
						{
							gap_extension   = true;
						}
					}


				}else
				{if (!left_operand_zero_run )
				{
					SetValueOnCompressedWord(left_operand [left_operand_index ],compressedResult);
					vector<int> result = AlignRightWithLeft(left_operand_end_position ,right_operand_index ,right_operand_end_position ,right_operand ,compressedResult,ONE_GAP,OR,gap_extension  );
					right_operand_index  = result[0];
					right_operand_end_position  = result[1];
					if (left_operand_end_position  == right_operand_end_position )
					{
						left_operand_index ++;
						right_operand_index ++;
						gap_extension   = false;
					}else if(left_operand_end_position  < right_operand_end_position )
					{
						gap_extension   = true;
					}
				} 
				else
				{
					vector<int> result = AlignRightWithLeft(left_operand_end_position ,right_operand_index ,right_operand_end_position ,right_operand ,compressedResult,ZERO_GAP,OR,gap_extension  );
					right_operand_index  = result[0];
					right_operand_end_position  = result[1];
					if (left_operand_end_position  == right_operand_end_position )
					{
						left_operand_index ++;
						right_operand_index ++;
						gap_extension   = false;
					}else if(left_operand_end_position  < right_operand_end_position )
					{
						gap_extension   = true;
					}

				}
				}
			}else if (left_operand_end_position  < right_operand_end_position )
			{
				if (gap_extension  )
				{
					int rightStartBit = GetStartBitValue(right_operand [right_operand_index ]);
					if (rightStartBit == ONE_GAP_WORD)
					{
						unsigned long int blockRun = right_operand_end_position  - left_operand_end_position ;
						//unsigned long int gapRunBlocks = rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG;
						unsigned long int resultGap = ONE_GAP_START_FLAG + blockRun;
						SetValueOnCompressedWord(resultGap,compressedResult);
						vector<int> result = AlignLeftWithRight(right_operand_end_position ,left_operand_index ,left_operand_end_position ,left_operand ,compressedResult,ONE_GAP,OR,gap_extension  );
						left_operand_index  = result[0];
						left_operand_end_position  = result[1];
						if (left_operand_end_position  == right_operand_end_position )
						{
							left_operand_index ++;
							right_operand_index ++;
							gap_extension   = false;
						}else if(right_operand_end_position  < left_operand_end_position )
						{
							gap_extension   = true;
						}
					}else if (rightStartBit == ZERO_GAP_WORD)
					{
						vector<int> result = AlignLeftWithRight(right_operand_end_position ,left_operand_index ,left_operand_end_position ,left_operand ,compressedResult,ZERO_GAP,OR,gap_extension  );
						left_operand_index  = result[0];
						left_operand_end_position  = result[1];
						if (left_operand_end_position  == right_operand_end_position )
						{
							left_operand_index ++;
							right_operand_index ++;
							gap_extension   = false;
						}else if(right_operand_end_position  < left_operand_end_position )
						{
							gap_extension   = true;
						}
					}

				}else
				{if (!right_operand_zero_run )
				{
					SetValueOnCompressedWord(right_operand [right_operand_index ],compressedResult);
					vector<int> result = AlignLeftWithRight(right_operand_end_position ,left_operand_index ,left_operand_end_position ,left_operand ,compressedResult,ONE_GAP,OR,gap_extension  );
					left_operand_index  = result[0];
					left_operand_end_position  = result[1];
					if (left_operand_end_position  == right_operand_end_position )
					{
						left_operand_index ++;
						right_operand_index ++;
						gap_extension   = false;
					}else if(right_operand_end_position  < left_operand_end_position )
					{
						gap_extension   = true;
					}
				} 
				else
				{
					vector<int> result = AlignLeftWithRight(right_operand_end_position ,left_operand_index ,left_operand_end_position ,left_operand ,compressedResult,ZERO_GAP,OR,gap_extension  );
					left_operand_index  = result[0];
					left_operand_end_position  = result[1];
					if (left_operand_end_position  == right_operand_end_position )
					{
						left_operand_index ++;
						right_operand_index ++;
						gap_extension   = false;
					}else if(right_operand_end_position  < left_operand_end_position )
					{
						gap_extension   = true;
					}
				}
				}
			}
		}

		if (m_iActiveWordSize > 0)
		{
			st->ActiveWordSize(ActiveWordSize());
			st->ActiveWord(m_ulActiveWord | (structure.ActiveWord()));
		}
		st->SetCompressedStream(compressedResult);
		st->OriginalStreamSize(OriginalStreamSize());
		return st;
	}



	vector<int> WAHStructure::AlignRightWithLeft( int _left_op_end_pos,int _right_op_index,int _right_op_end_pos,vector<unsigned long int> &_right_operand,vector<unsigned long int> &_result_vector,longer_operand _longer,operation_type _type /*= AND*/,bool _gap_extension_state )
	{
		vector<int> result;
		int index_val=0;
		int op_end_pos = 0;
		bool zero_gap = false;

		if (_type == AND)
		{
			if (_longer == ONE_GAP && !_gap_extension_state)
			{
				/*int rightStartBit = getStartBitValue(rightOperand[rightOpIndex]);
				if (rightStartBit == LITERAL_WORD)
				{*/
				SetValueOnCompressedWord(_right_operand[_right_op_index],_result_vector);
				//}
			}

			while(_left_op_end_pos > _right_op_end_pos){
				int start_bit = GetStartBitValue(_right_operand[++_right_op_index]);
				if (start_bit == LITERAL_WORD)
				{
					_right_op_end_pos++;			
				}else if (start_bit == ONE_GAP_WORD)
				{
					_right_op_end_pos += (_right_operand[_right_op_index] - ONE_GAP_START_FLAG);			
				}else if (start_bit == ZERO_GAP_WORD)
				{
					_right_op_end_pos += (_right_operand[_right_op_index] - ZERO_GAP_START_FLAG);			
				}
				index_val = _right_op_index;
				op_end_pos = _right_op_end_pos;	

				if (_longer == ONE_GAP)
				{
					if (_left_op_end_pos >= _right_op_end_pos)
					{
						SetValueOnCompressedWord(_right_operand[_right_op_index],_result_vector);
					}else{
						unsigned long int extra_blocks = _right_op_end_pos - _left_op_end_pos;
						unsigned long int  right_gap_value = _right_operand[_right_op_index];
						int startBitValue = GetStartBitValue(right_gap_value);
						unsigned long int new_gap_word = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = right_gap_value - ONE_GAP_START_FLAG;
							new_gap_word = (totalBlocks - extra_blocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = right_gap_value - ZERO_GAP_START_FLAG;
							new_gap_word = (totalBlocks - extra_blocks) + ZERO_GAP_START_FLAG;
						}
						SetValueOnCompressedWord(new_gap_word,_result_vector);

					}
				}
			}
		} 
		else if (_type == OR)
		{
			// When performing OR operation the initial literal of the left oerand is not written to the compressed
			//result. This code block performs that. 
			if ((_longer == ZERO_GAP) & !_gap_extension_state)
			{
				// 				int rightStartBit = getStartBitValue(rightOperand[rightOpIndex]);
				// 				if (rightStartBit == LITERAL_WORD)
				// 				{
				SetValueOnCompressedWord(_right_operand[_right_op_index],_result_vector);
				//				}


			}

			while(_left_op_end_pos > _right_op_end_pos){
				int startBit = GetStartBitValue(_right_operand[++_right_op_index]);
				if (startBit == LITERAL_WORD)
				{
					_right_op_end_pos++;			
				}else if (startBit == ONE_GAP_WORD)
				{
					_right_op_end_pos += (_right_operand[_right_op_index] - ONE_GAP_START_FLAG);			
				}else if (startBit == ZERO_GAP_WORD)
				{
					_right_op_end_pos += (_right_operand[_right_op_index] - ZERO_GAP_START_FLAG);			
				}
				index_val = _right_op_index;
				op_end_pos = _right_op_end_pos;	

				if (_longer == ZERO_GAP)
				{
					// In an OR operation when the GAP is zero, corresponding literals in  the other operand(right operand)
					//should be copied on to the compressed result.
					if (_left_op_end_pos >= _right_op_end_pos)
					{
						SetValueOnCompressedWord(_right_operand[_right_op_index],_result_vector);
					}else{

						// If the right operand encounters a gap and extends beyond the left operand this code executes
						// Even though the right operand extends beyond, the overlapping segment should 
						//be copied to compressed result as it the performing operation is OR
						unsigned long int extraBlocks = _right_op_end_pos - _left_op_end_pos;
						unsigned long int  rightGapValue = _right_operand[_right_op_index];
						int startBitValue = GetStartBitValue(rightGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = rightGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = rightGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						SetValueOnCompressedWord(newGapWord,_result_vector);

					}
				}
			}
		}
		result.push_back(index_val);
		result.push_back(op_end_pos);

		return result;
	}

	vector<int> WAHStructure::AlignLeftWithRight( int rightOpEndPos,int leftOpIndex,int leftOpEndPos,vector<unsigned long int> &leftoperand,vector<unsigned long int> &resultVector, longer_operand longer,operation_type type /*= AND*/,bool gapExtensionState )
	{
		vector<int> result;
		int indexVal=0;
		int opEndPos = 0;

		if (type == AND)
		{
			if ((longer == ONE_GAP) & !gapExtensionState)
			{
				// 				int leftStartBit = getStartBitValue(leftoperand[leftOpIndex]);
				// 				if (leftStartBit == LITERAL_WORD)
				// 				{
				SetValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
				//}


			}

			while(leftOpEndPos < rightOpEndPos){
				int startBit = GetStartBitValue(leftoperand[++leftOpIndex]);
				if (startBit == LITERAL_WORD)
				{
					leftOpEndPos++;

				}else if (startBit == ONE_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ONE_GAP_START_FLAG);

				}else if (startBit == ZERO_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ZERO_GAP_START_FLAG);

				}

				if (longer == ONE_GAP)
				{
					if (leftOpEndPos <= rightOpEndPos)
					{
						SetValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
					}else{
						unsigned long int extraBlocks = leftOpEndPos-rightOpEndPos ;
						unsigned long int  leftGapValue = leftoperand[leftOpIndex];
						int startBitValue = GetStartBitValue(leftGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = leftGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = leftGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						SetValueOnCompressedWord(newGapWord,resultVector);

					}
				}
			}
			// No need of setting values here. leftOpIndex and leftOpEndPos can be directly
			// inserted to the vector. 
			indexVal = leftOpIndex;
			opEndPos = leftOpEndPos;
		} 
		else if (type == OR)
		{
			if ((longer == ZERO_GAP) & !gapExtensionState)
			{
				// 				int leftStartBit = getStartBitValue(leftoperand[leftOpIndex]);
				// 				if (leftStartBit == LITERAL_WORD)
				// 				{
				SetValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
				//}

			}

			while(leftOpEndPos < rightOpEndPos){
				int startBit = GetStartBitValue(leftoperand[++leftOpIndex]);
				if (startBit == LITERAL_WORD)
				{
					leftOpEndPos++;

				}else if (startBit == ONE_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ONE_GAP_START_FLAG);

				}else if (startBit == ZERO_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ZERO_GAP_START_FLAG);

				}

				if (longer == ZERO_GAP)
				{
					if (leftOpEndPos <= rightOpEndPos)
					{
						SetValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
					}else{
						unsigned long int extraBlocks = leftOpEndPos-rightOpEndPos ;
						unsigned long int  leftGapValue = leftoperand[leftOpIndex];
						int startBitValue = GetStartBitValue(leftGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = leftGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = leftGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						SetValueOnCompressedWord(newGapWord,resultVector);

					}
				}
			}
			// No need of setting values here. leftOpIndex and leftOpEndPos can be directly
			// inserted to the vector. 
			indexVal = leftOpIndex;
			opEndPos = leftOpEndPos;
		}


		result.push_back(indexVal);
		result.push_back(opEndPos);
		return result;
	}

	int WAHStructure::GetStartBitValue( size_t _word )
	{
		int startBitValue= 0;
		unsigned long int result = _word & ZERO_GAP_START_FLAG;
		if (result == 0)
		{
			startBitValue = LITERAL_WORD;
		}
		else 
		{
			result = _word & ONE_GAP_START_FLAG;
			if (result == ZERO_GAP_START_FLAG)
			{
				startBitValue = ZERO_GAP_WORD;
			}else
			{
				startBitValue = ONE_GAP_WORD;
			}

		}
		return startBitValue;
	} 

	//This method checks the previous word and the current word if possible
	void WAHStructure::SetValueOnCompressedWord( unsigned long int _word, vector<unsigned long int> &_compressed_result )
	{
		if (_compressed_result.size() == 0)
		{

			_compressed_result.push_back(_word);

		}
		else {
			unsigned long int lastWord = 0;

			lastWord = _compressed_result[_compressed_result.size()-1];

			// Maximum maximum number represented for the value has not been considered yet.
			// Remember to use it before final version.
			unsigned long int result = 0;
			if (_word == ZERO_LITERAL && lastWord == ZERO_LITERAL )
			{
				_compressed_result[_compressed_result.size()-1] = ZERO_GAP_START_FLAG | 2;
			}
			else if (_word == ZERO_LITERAL && GetStartBitValue(lastWord) == ZERO_GAP_WORD)
			{
				_compressed_result[_compressed_result.size()-1] = lastWord + 1;
			}
			else if (_word == ONE_LITERAL && lastWord == ONE_LITERAL)
			{
				_compressed_result[_compressed_result.size()-1] = ONE_GAP_START_FLAG + 2;
			}
			else if (_word == ONE_LITERAL && GetStartBitValue(lastWord) == ONE_GAP_WORD)
			{
				_compressed_result[_compressed_result.size()-1] = lastWord + 1;
			}
			else if (GetStartBitValue(_word) == ONE_GAP_WORD && GetStartBitValue(lastWord) == ONE_GAP_WORD)
			{
				_compressed_result[_compressed_result.size()-1] = _word + lastWord - ONE_GAP_START_FLAG;
			}
			else if(GetStartBitValue(_word) == ZERO_GAP_WORD && GetStartBitValue(lastWord) == ZERO_GAP_WORD)
			{
				_compressed_result[_compressed_result.size()-1] = _word + lastWord - ZERO_GAP_START_FLAG;
			}else
			{
				_compressed_result.push_back(_word);
			}
		}
	}


	void WAHStructure::SetCompressedStream(vector<unsigned long int> &x)
	{
		m_compressed_stream = x;
	}


	vector<unsigned long int>& WAHStructure::GetCompressedVector()
	{
		return m_compressed_stream;
	}

	int WAHStructure::GetSpaceUtilisation()
	{
		return 0;
	}

	dynamic_bitset<> WAHStructure::ConvertVectorToBitmap(std::vector<unsigned long int> &decompressedVector)
	{
		int size = decompressedVector.size();
		dynamic_bitset<> result(size * (WORD_SIZE - 1));
		int bitmapIndex = 0;
		for (int i=0; i < size ; i++)
		{
			dynamic_bitset<> temp(WORD_SIZE-1,decompressedVector[i]);
			for (int j = 0; j < WORD_SIZE-1 ;)
			{
				result[bitmapIndex++] = temp[j++];
			}
		}
		return result;
	}


	vector<unsigned long int> WAHStructure::ExpandCompressedVector(vector<unsigned long int> & compressedVector)
	{

		vector<unsigned long int> result;
		int resultIndex = 0;
		for (size_t i = 0 ; i < compressedVector.size() ; i++)
		{
			unsigned long int vectorValue = compressedVector[i];
			int startBit = GetStartBitValue(vectorValue);
			if (startBit == LITERAL_WORD)
			{
				result.push_back(vectorValue);
			}else if (startBit == ONE_GAP_WORD)
			{
				int blocks  = vectorValue - ONE_GAP_START_FLAG;
				for (int blockNo = 0; blockNo < blocks ; blockNo++)
				{
					result.push_back(ONE_LITERAL);
				}
			}else if (startBit == ZERO_GAP_WORD)
			{
				int blocks  = vectorValue - ZERO_GAP_START_FLAG;
				for (int blockNo = 0; blockNo < blocks ; blockNo++)
				{
					result.push_back(ZERO_LITERAL);
				}

			}
		}

		return result;
	}

	dynamic_bitset<> WAHStructure::Decompress()
	{
		vector<unsigned long int> result = ExpandCompressedVector(m_compressed_stream);
		dynamic_bitset<> resultBitset = ConvertVectorToBitmap(result);
		if (m_iActiveWordSize > 0)
		{
			dynamic_bitset<> temp(m_iActiveWordSize,m_ulActiveWord);
			for (size_t i = 0; i < temp.size() ; i++)
			{
				resultBitset.push_back(temp[i]);
			}
		}
		return resultBitset;
	}

	/*WAHStructure * WAHStructure::operator ~(){
	WAHStructure * result = new WAHStructure();
	vector<unsigned long int> newCompressedStream;
	vector<unsigned long int>::iterator vectorIterator  = m_compressed_stream.begin();
	while(vectorIterator != m_compressed_stream.end())
	{
	unsigned long int vectorValue = *(vectorIterator);
	int startBitValue = GetStartBitValue(vectorValue);
	if (startBitValue == LITERAL_WORD)
	{
	newCompressedStream.push_back((~vectorValue) & ONE_LITERAL);
	}else if (startBitValue == ONE_GAP_WORD)
	{
	newCompressedStream.push_back((vectorValue - ONE_GAP_START_FLAG + ZERO_GAP_START_FLAG));
	}else if(startBitValue == ZERO_GAP_WORD)
	{
	newCompressedStream.push_back((vectorValue - ZERO_GAP_START_FLAG + ONE_GAP_START_FLAG));
	}
	vectorIterator++;
	}
	result->SetCompressedStream(newCompressedStream);
	result->OriginalStreamSize(m_iOriginalStreamSize);
	if (m_iActiveWordSize > 0)
	{
	result->ActiveWordSize(m_iActiveWordSize);
	result->ActiveWord(~m_ulActiveWord);
	}
	return result;
	}
	*/
	BitStreamInfo * WAHStructure::operator ~(){
		WAHStructure * result = new WAHStructure();
		WAHStructure * current_structure = dynamic_cast<WAHStructure *>(this);
		vector<unsigned long int> newCompressedStream;
		vector<unsigned long int>::iterator vectorIterator  = current_structure->GetCompressedVector().begin();
		while(vectorIterator != current_structure->GetCompressedVector().end())
		{
			unsigned long int vectorValue = *(vectorIterator);
			int startBitValue = GetStartBitValue(vectorValue);
			if (startBitValue == LITERAL_WORD)
			{
				newCompressedStream.push_back((~vectorValue) & ONE_LITERAL);
			}else if (startBitValue == ONE_GAP_WORD)
			{
				newCompressedStream.push_back((vectorValue - ONE_GAP_START_FLAG + ZERO_GAP_START_FLAG));
			}else if(startBitValue == ZERO_GAP_WORD)
			{
				newCompressedStream.push_back((vectorValue - ZERO_GAP_START_FLAG + ONE_GAP_START_FLAG));
			}
			vectorIterator++;
		}
		result->SetCompressedStream(newCompressedStream);
		result->OriginalStreamSize(m_iOriginalStreamSize);
		if (current_structure->ActiveWordSize() > 0)
		{
			//Very Inefficient method. Should Rewrite after evaluation
			result->ActiveWordSize(current_structure->ActiveWordSize());
			dynamic_bitset<> temp(ActiveWordSize(),ActiveWord());
			temp.flip();
			result->ActiveWord(temp.to_ulong());
		}
		return result;
	}
}