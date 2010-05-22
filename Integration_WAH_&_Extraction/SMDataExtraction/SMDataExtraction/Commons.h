#include "BitStreamInfo.h"
#include "EncodedAttributeInfo.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <iostream>

using namespace std;
#pragma once

class Commons
{
public:
	Commons(void);
	~Commons(void);
	
	template <typename iter_type>
	static void DeleteVector(iter_type first, iter_type last)
	{
		for (iter_type cur = first; cur != last; ++cur)
		{
			if((*cur) != NULL)
			{
				delete *cur;
				*cur = NULL;
			}
		}
	}

	template <typename iter_type>
	static void InitVector(iter_type first, iter_type last)
	{
		for (iter_type cur = first; cur != last; ++cur)
		{

			*cur = NULL;
		}
	}

	/** Utility to print a double array*/
	_declspec(dllexport) static void PrintArray(double * _double_arr,int _length);

	/** Method to print a 2D array*/
	_declspec(dllexport) static void PrintArray(double ** _double_arr,int _length, int _width);

	/** Initializes a one dimensional double array by a given value*/
	_declspec(dllexport) static void InitArray(double * _double_arr,int _length,double _init_val);

	/** Initializes a two dimensional double array by a given value*/
	_declspec(dllexport) static void InitArray(double ** _double_arr,int _length,int _width,double _init_val);

	/*
	BitStreamInfo * BitStreamGenerator(EncodedAttributeInfo * attribute,dynamic_bitset<> & _bit_stream);
		BitStreamInfo * UGreaterThan( EncodedAttributeInfo * attribute, unsigned long value,int rows );
		BitStreamInfo * UGreaterThanInt(EncodedAttributeInfo * attribute,unsigned long input_value,int noOfRows);*/
	
};
