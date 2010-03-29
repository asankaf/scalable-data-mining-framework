#include "WrappedBitVector.h"

WrappedBitVector::WrappedBitVector(void)
{
}

WrappedBitVector::~WrappedBitVector(void)
{
	m_bitvector.clear();
	//delete m_bitvector;
}

WrappedBitVector::WrappedBitVector(const WrappedBitVector &bv)
{
	//m_bitvector = bitvector temp(bv.m_bitvector);
	bitvector temp(bv.m_bitvector);
	m_bitvector.copy(temp);
}

WrappedBitVector::WrappedBitVector(const ibis::bitvector &bv)
{
	m_bitvector.copy(bv);
}

ostream & WrappedBitVector::print(ostream & _stream)
{
	return m_bitvector.print(_stream);
}

WrappedBitVector::WrappedBitVector(dynamic_bitset<> & _bitset)
{
	bitvector temp;	
	for (size_t i = 0; i < _bitset.size() ; i++)
	{
		temp +=  _bitset[i];
	}
	m_bitvector.copy(temp);
}

void WrappedBitVector::decompress()
{
m_bitvector.decompress();
}

size_t WrappedBitVector::compressible()
{
	return (size_t)m_bitvector.compressible();
}

void WrappedBitVector::compress()
{
	m_bitvector.compress();
}

WrappedBitVector & WrappedBitVector::copy(WrappedBitVector & _copy)
{
	m_bitvector.copy(_copy.m_bitvector);
	return _copy;
}

void WrappedBitVector::decompress(dynamic_bitset<> & _bitset)
{

	array_t<bitvector::word_t> temp_arr;
	m_bitvector.decompress();
	m_bitvector.write(temp_arr);
	bitvector::word_t ss = m_bitvector.size();
//	_bitset.resize((unsigned long)40);
//   _bitset.resize((size_t)m_bitvector.size());
 	dynamic_bitset<> temp_bits;
 	size_t k = 0;
	for (size_t i = 0; i < temp_arr.size();i++)
	{			
		if (i == temp_arr.size()-2)
		{
			temp_bits = dynamic_bitset<>((int)temp_arr[i+1],(unsigned long)(temp_arr[i]));
			i = temp_arr.size();
		}else{
			temp_bits = dynamic_bitset<>(31,(unsigned long)(temp_arr[i]));	
		}
		for (int j = temp_bits.size()-1; j > -1 ;j--)
		{
			_bitset[k++] = temp_bits[j];
		}
	}
}

WrappedBitVector::WrappedBitVector(const vector< unsigned long > &arr)
{
	array_t<bitvector::word_t> temp_arr(arr.size());
	for (size_t i = 0; i < arr.size(); i++)
	{
		temp_arr[i] = arr[i];
	}
	bitvector temp(temp_arr);
	m_bitvector.copy(temp);
}

bitvector & WrappedBitVector::getBitVector()
{
	return m_bitvector;
}

void WrappedBitVector::print()
{
	cout << m_bitvector << endl;
}

void WrappedBitVector::adjustSize(unsigned long nv, unsigned long nt)
{
	m_bitvector.adjustSize((bitvector::word_t)(nv),(bitvector::word_t)(nt));
}

void WrappedBitVector::set(int val, unsigned long n)
{
	m_bitvector.set(val,(bitvector::word_t)(n));
}

unsigned long WrappedBitVector::size()
{
	return (unsigned long)m_bitvector.size();
}



void WrappedBitVector::write(vector<unsigned long> &arr)
{
	array_t<bitvector::word_t> temp_arr;
	m_bitvector.write(temp_arr);
	//Inefficient method. Check whether there's a direct constructor available to copy.
	// 	for (size_t i = 0; i < temp_arr.size() ; i++)
	// 	{		
	// 		arr.push_back((unsigned long)temp_arr[i]);
	// 	}
	for (array_t<bitvector::word_t>::const_iterator iter = temp_arr.begin(); iter != temp_arr.end(); iter++)
	{
		arr.push_back(*(iter));
	}
}

unsigned long WrappedBitVector::bytes()
{
	return m_bitvector.bytes();
}

void WrappedBitVector::clear()
{
	m_bitvector.clear();
}

WrappedBitVector * WrappedBitVector::operator &(WrappedBitVector &_right)
{
	bitvector * result_vect = m_bitvector & _right.m_bitvector;
	WrappedBitVector * result = new WrappedBitVector(*(result_vect));
	return result;
}

WrappedBitVector * WrappedBitVector::operator |(WrappedBitVector &_right)
{
	bitvector * result_vect = m_bitvector | _right.m_bitvector;
	WrappedBitVector * result = new WrappedBitVector(*(result_vect));
	return result;
}

WrappedBitVector * WrappedBitVector::operator ~()
{
	WrappedBitVector * result = new WrappedBitVector(m_bitvector);
	result->m_bitvector.flip();
	return result;
}

unsigned long long WrappedBitVector::count()
{
	return (unsigned long long)(m_bitvector.cnt());
}
