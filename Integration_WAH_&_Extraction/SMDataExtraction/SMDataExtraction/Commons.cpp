#include "StdAfx.h"
#include "Commons.h"
#include "EncodedDoubleAttribute.h"

Commons::Commons(void)
{
}

Commons::~Commons(void)
{
}

void Commons::PrintArray(double * _double_arr,int _length)
{
	for (size_t i =0 ; i < _length ;i++)
	{
		cout << "Index " << i << " : " << _double_arr[i]<< endl;
	}
}

void Commons::PrintArray(double **_double_arr, int _length, int _width)
{
	for (size_t i = 0; i < _length ; i++)
	{
		cout << "Printing Row " << i << endl;
		for (size_t j = 0 ; j < _width ; j++)
		{
			cout << _double_arr[i][j] << "\t";
		}
		cout << endl;
	}
}

void Commons::InitArray( double * _double_arr,int _length,double _init_val )
{
	for (size_t i = 0 ; i < _length ; i++)
	{
		_double_arr[i] = _init_val;
	}
}

void Commons::InitArray(double ** _double_arr,int _length,int _width,double _init_val)
{
	for (size_t i = 0 ; i < _length ; i++)
	{
		for (size_t j = 0 ; j < _width ; j++)
		{
			_double_arr[i][j] = _init_val;
		}
	}
}