#pragma once
#include "bitvector.h"
#include "boost/dynamic_bitset.hpp"
#include <ostream>
#include <iostream>
#include <vector>

using namespace ibis;
using namespace std;
using namespace boost;
class WrappedBitVector
{
public:
	_declspec(dllexport) WrappedBitVector(void);
	_declspec(dllexport) WrappedBitVector(const vector< unsigned long > &arr);		
	_declspec(dllexport) WrappedBitVector(const WrappedBitVector &bv);
	_declspec(dllexport) WrappedBitVector(const bitvector &bv);
	_declspec(dllexport) WrappedBitVector(dynamic_bitset<> & _bitset);
	_declspec(dllexport) ~WrappedBitVector(void);
	_declspec(dllexport) void operator &=(WrappedBitVector & _vect);
	_declspec(dllexport) void adjustSize(unsigned long nv, unsigned long nt);
	_declspec(dllexport) void set(int val, unsigned long n);
	_declspec(dllexport) void setBit(const unsigned long i, int val);

	_declspec(dllexport) unsigned long size();
	_declspec(dllexport) void compress();
	_declspec(dllexport) size_t compressible();
	_declspec(dllexport) void decompress();
	_declspec(dllexport) void decompress(dynamic_bitset<> & _bitset);
	_declspec(dllexport) void write(vector<unsigned long> &arr);	
	_declspec(dllexport) unsigned long bytes();		
	_declspec(dllexport) void clear();
	_declspec(dllexport) WrappedBitVector & copy(WrappedBitVector & _copy);
	_declspec(dllexport) ostream & print(ostream &);
	_declspec(dllexport) void print();
	_declspec(dllexport) bitvector & getBitVector();

	_declspec(dllexport) WrappedBitVector * operator&(WrappedBitVector & _right);
	_declspec(dllexport) WrappedBitVector * operator|(WrappedBitVector & _right);
	_declspec(dllexport) WrappedBitVector * operator~();
	_declspec(dllexport) unsigned long long	count();
	_declspec(dllexport) unsigned long SpaceUtilsation();


private:
	bitvector m_bitvector;
};
