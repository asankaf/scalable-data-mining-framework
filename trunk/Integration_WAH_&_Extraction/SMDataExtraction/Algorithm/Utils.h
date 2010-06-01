#pragma once
#include <math.h>
#include "boost/lexical_cast.hpp"
#include <xstring>
#include "WrapDataSource.h"
#include "LoadSavedDataSources.h"
#include "DataSources.h"
#include "csvconnection.h"
#include "extractedCsvDTO.h"
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

using namespace std;
using namespace CSVConnectionInfo;
class Utils
{
public:
	_declspec(dllexport) Utils(void){}
	
	_declspec(dllexport) ~Utils(void);

	/** The natural logarithm of 2. */
	static double const log2;

	/** The small deviation allowed in double comparisons. */
	static double const SMALL;
	
	/**
	* Tests if a is greater than b.	
	*/
	static bool gr(double a,double b);

	/**
	* Tests if a is smaller than b.	
	*/
	static bool sm(double a,double b);

	/**
	* Tests if a is smaller or equal to b.
	*/
	static bool smOrEq(double a,double b);

	/**
	* Tests if a is equal to b.
	*/
	static bool eq(double a, double b);

	/**
	* Tests if a is greater or equal to b.	
	*/
	static bool grOrEq(double a,double b);

	/**
	* Rounds a double to the given number of decimal places.	
	*/
	static double roundDouble(double value,int afterDecimalPoint);

	/**
	* Converts an integer to a string	
	*/
	_declspec(dllexport) static string toStringVal(int val);

	/**
	* Normalizes the doubles in the array using the given value.	
	*/
	static void Normalize(double * doubles, double sum,size_t _length);

	/**
	* Returns index of maximum element in a given
	* array of doubles. First maximum is returned.	
	*/
	_declspec(dllexport) static int MaxIndex(double * doubles,size_t _length); 

	/**
	* Converts a double value to a string	
	*/
	_declspec(dllexport) static string toStringVal(double val);

	/**
	* Gives a string having a the given number of precissions	
	*/
	_declspec(dllexport) static string toStringVal(double val,int precission);

	/**
	* Normalizes the doubles in the array by their sum.	
	*/
	_declspec(dllexport) static void Utils::Normalize(double * doubles,size_t _length);

	/**
	* Rounds a double and converts it into a formatted decimal-justified String.
	* Trailing 0's are replaced with spaces.
	*/
	_declspec(dllexport) static string doubleToString(double value, int width,int afterDecimalPoint);

	/**
	* Rounds a double and converts it into String.
	*/
	_declspec(dllexport) static string doubleToString(double value, int afterDecimalPoint);

	/** Computes the sum of the elements of an array of doubles.*/
	_declspec(dllexport) static double sum(double* doubles,size_t length);

	/** Gives the string representation of the input value */
	_declspec(dllexport) string toStringVal(size_t _val);

	/** Loads the data source saved as a XML file*/
	_declspec(dllexport) static WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename);

	/** Loads the data source from a CSV*/
	_declspec(dllexport) static WrapDataSource * CreateDataSource(string csv_path);

	/** Loads the number of rows specified from the saved data set*/
	_declspec(dllexport) static WrapDataSource * CreateDataSource(string datafile,string metadFile,string filename,int _limit);

	/** Loads the whole data set from the data files located inside the directory indicated by _data_folder*/
	_declspec(dllexport) static WrapDataSource * CreateDataSourceFromMultipleFiles(string _data_folder,string _meta_File,string _source_name);

	/** Returns a string depicting the memory information about the process*/
	_declspec(dllexport) string GetMemoryInfo(DWORD & _processID );
		
};