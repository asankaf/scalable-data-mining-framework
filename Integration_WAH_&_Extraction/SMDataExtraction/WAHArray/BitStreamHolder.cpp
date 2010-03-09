#include "StdAfx.h"
#include "BitStreamHolder.h"

namespace Algorithm{
	BitStreamHolder::BitStreamHolder(void)
	{
		m_count = -1;
	}

	BitStreamHolder::~BitStreamHolder(void)
	{
	}

	void BitStreamHolder::AddAttributeID(int _att_id)
	{
		m_attribute_no.push_back(_att_id);		
	}

	void BitStreamHolder::Print()
	{
		cout << "Printing Bit Holder" << endl;
		typedef vector<int>::const_iterator vect_iter;

		cout << "Print attribute No " << endl;
		for (vect_iter iter = m_attribute_no.begin(); iter != m_attribute_no.end(); iter++)
		{
			cout <<*(iter);
		}
		cout << endl;
		cout << "Print Unique Streams " << endl;

		for (vect_iter iter = m_bit_stream_no.begin(); iter != m_bit_stream_no.end(); iter++)
		{
				cout <<*(iter);
		}
		cout << endl;

	}

	void BitStreamHolder::AddStreamNo(int _stream_no)
	{
		m_bit_stream_no.push_back(_stream_no);
	}

	bool BitStreamHolder::Contains(int _value, std::vector<int> &_container)
	{

		sort(_container.begin(),_container.end());
		return binary_search(_container.begin(),_container.end(),_value);

	}

	int BitStreamHolder::Count()
	{
// 		if (m_count == -1)
// 		{
//			m_count = m_bit_stream->Count();
	//	}
		return m_bit_stream->Count();
	}
//Handle when vector<int> is not set (or throw an exeption)
	int BitStreamHolder::Hash()
	{
		int factor = 1;
		int hash = 0;
		typedef vector<int>::const_iterator int_iter;
		for (int_iter start = m_bit_stream_no.begin(); start != m_bit_stream_no.end() ; start++)
		{
			hash += ((*(start) + 1) * factor);
			factor *= MULTIPLICATIVE_UNIT;
		}
		return hash;
	}

//	void BitStreamHolder::

	vector<BitStreamHolder *> BitStreamHolder::Difference(BitStreamHolder * _holder,map<int,int> & _index_attribute_map)
	{
		vector<int> left_op = _holder->Bit_stream_no();
		vector<int> right_op = this->m_bit_stream_no;
		sort(left_op.begin(),left_op.end());
		sort(right_op.begin(),right_op.end());
		vector<int> intermediate;
		back_insert_iterator< std::vector<int> > output_iter( intermediate );
		set_difference(left_op.begin(),left_op.end(),right_op.begin(),right_op.end(),output_iter);
		vector<BitStreamHolder *> holder;
		BitStreamHolder * hld  = NULL;
		typedef vector<int>::const_iterator holder_iter;
		for (holder_iter start = intermediate.begin(); start != intermediate.end(); start++)
		{
			hld = new BitStreamHolder();
			hld->AddStreamNo(*(start));
			hld->AddAttributeID(_index_attribute_map[*(start)]);
			holder.push_back(hld);
		}
		return holder;
	}

	BitStreamHolder * BitStreamHolder::Merge(BitStreamHolder * _holder,map<int,int> & _index_attribute_map,vector<BitStreamHolder *> & _index_holder)
	{
		BitStreamHolder * hld;
		size_t right_holder_index = _holder->Bit_stream_no().at(0);
		int att_no = _index_attribute_map[right_holder_index];
		if (this->ContainsAttID(att_no) && !(this->ContainsStreamNo(right_holder_index)))
		{
			hld = NULL;
		}else
		{  hld = new BitStreamHolder();

			vector<int> left_vector = this->m_bit_stream_no;
			vector<int> right_vector = _holder->Bit_stream_no();
			sort(left_vector.begin(),left_vector.end());
			sort(right_vector.begin(),right_vector.end());
			vector<int> result_vector;
			back_insert_iterator< std::vector<int> > output_iter( result_vector );
			set_union(left_vector.begin(),left_vector.end(),right_vector.begin(),right_vector.end(),output_iter);
			hld->Bit_stream_no(result_vector);

			BitStreamInfo * left_op = m_bit_stream;
			BitStreamInfo * right_op = _index_holder.at(_holder->Bit_stream_no().at(0))->Bit_stream();
			BitStreamInfo * result_str = *(left_op) & *(right_op);
			
			hld->Bit_stream(result_str);
			
			vector<int> result_att_vector;
			if (!this->ContainsAttID(att_no))
			{
				vector<int> result_att_vector = m_attribute_no;
				result_att_vector.push_back(att_no)	;
				hld->Attribute_no(result_att_vector);					
			}
		}	
		
		
		return hld;
	}

	void BitStreamHolder::DeleteBitStream()
	{
		delete m_bit_stream;
		m_bit_stream = NULL;
	}

	bool BitStreamHolder::ContainsAttID(int _att_id)
	{
		return Contains(_att_id,m_attribute_no);
	}

	bool BitStreamHolder::ContainsStreamNo(int _stream_no)
	{
		return Contains(_stream_no,m_bit_stream_no);
	}
}