#include "StdAfx.h"
#include "EWAH.h"

EWAH::EWAH(void)
{
	Type(BitStreamInfo::EWAH_COMPRESSION);
}

EWAH::~EWAH(void)
{
	m_bitvector.clear();
}

EWAH::EWAH(WrappedBitVector & _vect)
{	
//	this(void);
	m_bitvector.copy(_vect);
}

BitStreamInfo * EWAH::operator ~()
{
	WrappedBitVector * wrapped_vect = ~(m_bitvector);
	EWAH * new_vect = new EWAH(*(wrapped_vect));
	return new_vect;
}

BitStreamInfo * EWAH::operator &(BitStreamInfo & _right_op)
{	
	EWAH * right_op = dynamic_cast<EWAH *>(&_right_op);
	WrappedBitVector * result_vect = m_bitvector & right_op->m_bitvector;	
	return new EWAH(*(result_vect));;
}

BitStreamInfo * EWAH::operator |(BitStreamInfo & _right_op)
{	
	EWAH * right_op = dynamic_cast<EWAH *>(&_right_op);
	WrappedBitVector * result_vect = m_bitvector | right_op->m_bitvector;	
	return new EWAH(*(result_vect));;
}

unsigned long long EWAH::Count()
{
	return m_bitvector.count();
}

void EWAH::CompressWords(boost::dynamic_bitset<> &bitMap)
{
	WrappedBitVector vect(bitMap);
	m_bitvector.copy(vect);	
}

dynamic_bitset<> EWAH::Decompress()
{
	dynamic_bitset<> temp_bitset((int)m_bitvector.size(),0ul);
	m_bitvector.decompress(temp_bitset);
	return temp_bitset;
}

size_t EWAH::SpaceUtilisation()
{
	return 0;
}

void EWAH::Print()
{
	m_bitvector.print();
}