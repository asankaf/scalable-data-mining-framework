#pragma once

#include "PureAttInfo.h"
#include "Commons.h"

#include <vector>

using namespace std;

/************************************************************************/
/*    Class  :DecodedTuple.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.03.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :DTO Class to hold the Double attribute properties
/*    Version: 
/************************************************************************/

class PureDoubleAttInfo : public PureAttInfo
{
public:

	#pragma region Constructors & Destructor

	__declspec(dllexport) PureDoubleAttInfo();
	__declspec(dllexport) ~PureDoubleAttInfo();

	#pragma endregion Constructors & Destructor

	#pragma region Getters & Setters

	__declspec(dllexport) double Upper();
	__declspec(dllexport) double Lower();
	__declspec(dllexport) void setUpper(double upperLimit);
	__declspec(dllexport) void setLower(double lowerLimit);	
	__declspec(dllexport) double* ValueList();
	__declspec(dllexport) void setValueList(double *values);
	__declspec(dllexport) vector<double> valList(){return this->_valList;}
	__declspec(dllexport) void setValList(vector<double> vals){this->_valList = vals;}

	#pragma endregion Getters & Setters

	/**	Method to initialize vectors in the class*/
	void Init(){
		Commons::InitVector(_valList.begin(),_valList.end());
		_valueList = NULL;
	}

private:
	double _upper;
	double _lower;
	double* _valueList;
	vector<double> _valList;

};
