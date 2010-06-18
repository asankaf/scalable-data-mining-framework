#include "StdAfx.h"
#include "MultiCatDataInfo.h"
#include <math.h>
#include <iostream>
#include "ExceptionReader.h"
#include "ExceptionCodes.h"

MultiCatDataInfo::MultiCatDataInfo(std::set<string> uniqueDataList)
{
	this->_uniqueStrings = uniqueDataList;
}

int* MultiCatDataInfo::getAssignedEncodedNumberList(){
	try
	{
		int uniqueRows = this->_uniqueStrings.size();
		this->_noOfBitsNeeded = (int) ceil(log10((double)uniqueRows / 2.0));
		int noOfAssignedInts = (int) pow(2.0 , (double)this->_noOfBitsNeeded);
		this->_assignedEncodedNumbers = new int[noOfAssignedInts];

		int temp = 0;
		for (; temp<noOfAssignedInts ; temp++)
		{
			this->_assignedEncodedNumbers[temp] = temp;
		}
	}
	catch(...){
		error_vector_out_of_range ex;
		string err = ExceptionReader::GetError(SM1007);
		err += "-> @Retrieving assigned number list for multicat values.";
		ex << error_message(err);
		ex << error_code(SM1007);
		BOOST_THROW_EXCEPTION(ex);
	}
	return this->_assignedEncodedNumbers;
}

MultiCatDataInfo::~MultiCatDataInfo(void)
{
	delete this->_assignedEncodedNumbers;
	this->_uniqueStrings.clear();
}
