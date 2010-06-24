#include "PureAttInfo.h"
#include "Commons.h"

#include <vector>

#pragma once

using namespace std;

/************************************************************************/
/*    Class  :DecodedTuple.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.03.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :DTO Class to hold the Integer attribute properties
/*    Version: 
/************************************************************************/

class PureIntAttInfo : public PureAttInfo
{
public:

	#pragma region Constructirs & Destructor

	__declspec(dllexport) PureIntAttInfo();
	__declspec(dllexport) ~PureIntAttInfo();

	#pragma endregion Constructirs & Destructor

	#pragma region Getters & Setters

	__declspec(dllexport) long int Upper();
	__declspec(dllexport) long int Lower();
	__declspec(dllexport) void setUpper(long int upperLimit);
	__declspec(dllexport) void setLower(long int lowerLimit);	
	__declspec(dllexport) long int* ValueList();
	__declspec(dllexport) vector<long int> valList(){return this->_valList;}
	__declspec(dllexport) void setValueList(long int *values);
	__declspec(dllexport) void setValList(vector<long int> vals){this->_valList = vals;}

	#pragma endregion Getters & Setters

	/**	Method to initialize vectors in the class*/
	void Init(){
		Commons::InitVector(_valList.begin(),_valList.end());
		_valueList = NULL;
	}
private:
	long int _upper;
	long int _lower;
	long int* _valueList;
	vector<long int> _valList;
};
