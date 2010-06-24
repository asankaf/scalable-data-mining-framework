#pragma once

#include "BitStreamInfo.h"

#include <bitset>
#include <vector>
#include <string>
#include <math.h>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;

/************************************************************************/
/*    Class  :VBitStream.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:18.04.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :A concrete implementation of the BitStreamInfo class. This
/*			  class is responsible for holding behaviours and other features
/*			  that non-compressed vertical bitstreams possess
/*    Version: 
/************************************************************************/

class VBitStream : public BitStreamInfo
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) VBitStream(int bitCount);
	__declspec(dllexport) VBitStream();
	__declspec(dllexport) ~VBitStream(void);

	#pragma endregion Constructors & Destructor
	
	/** Overridden Method to decompress the bitstream if the particular bitstream is already compressed*/
	/** Since the bitstream is not in compressed version, then also it will simply returns the uncompressed bitstream*/
	__declspec(dllexport) dynamic_bitset<> Decompress();

	/** Overridden, Kind of a dummy Method to compress the bitset when the bitset is provided as a boost dynamic_bitset*/
	/** Since under this class only non-compressed versions of bitstreams are kept, no compression is done*/
	__declspec(dllexport) void CompressWords(boost::dynamic_bitset<>& bitMap);

	/** Overridden Method to return number of 1's available in a particular bitstream*/
	__declspec(dllexport) unsigned long long Count();	

	/** Overridden Method to return the amount of space used by the particular bitstream*/
	__declspec(dllexport) size_t SpaceUtilisation();

	/** Overridden Operator to return the inversion of the bits in the current bitstream*/
	/** The method will simply return a same type of bitstream with bits inverted   */
	__declspec(dllexport) BitStreamInfo* operator ~();

	/** Overridden Operator to perform AND operation between two bitstreams, Input to this*/
	/** operator is the other bitstream and this will simply return another bitstream   */
	__declspec(dllexport) BitStreamInfo* operator &(BitStreamInfo &);

	/** Overridden Operator to perform OR operation between two bitstreams, Input to this */
	/** operator is the other bitstream and this will simply return another bitstream   */
	__declspec(dllexport) BitStreamInfo* operator |(BitStreamInfo &);

	/** Overridden method declared in the superclass.
	Provides a clone of the VBitStream object **/
	__declspec(dllexport) BitStreamInfo* Clone();


private:
	static const int JUNK_VAL = 2147483647;
};
