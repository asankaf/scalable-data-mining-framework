#pragma once
#include "BitStreamInfo.h"
#include "EncodedAttributeInfo.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include "EncodedDoubleAttribute.h"
#include "boost/tokenizer.hpp"

#include <fstream>
#include <iostream>
#include <hash_map>

using namespace stdext;
using namespace std;

/************************************************************************/
/*    Class  :Commons.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.05.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :Common utility class
/*    Version: 
/************************************************************************/

class Commons
{
public:

	#pragma region Constructors & Destructor

	Commons(void);
	~Commons(void);

	#pragma endregion Constructors & Destructor
	
	/**	Template method to delete a vector*/
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

	/** Template method to initialize a vector*/
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

	/** Gives the value of the property indicated by _property*/
	_declspec(dllexport)static  string ReadConfiguration(string & _property);

	/** Gives the value of the property indicated by _property*/
	_declspec(dllexport)static  string ReadConfiguration(const string & _property);

	/** Creating the configuration file, given by the location _path*/
	_declspec(dllexport) static void BuildFile(string & _path);

	/** Tokenises the string and add to the hash map*/
	static void Tokenise(string & _str,hash_map<string,string> & _map);

	/** Tokenises the string and add to the hash map*/
	static void Tokenise(string & _str,string _delimiter,hash_map<string,string> & _map);

	/** Method to read a file and populate it into a vector*/
	static vector<string> ReadFile(string & _path);

	private :
	static hash_map<string,string> m_map;
	
	
};
