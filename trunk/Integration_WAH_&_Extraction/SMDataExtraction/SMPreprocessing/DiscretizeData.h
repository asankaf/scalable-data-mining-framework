#include "EncodedDoubleAttribute.h"
#include "EncodedIntAttribute.h"
#include "WrapDataSource.h"
#pragma once

/************************************************************************/
/*    Class  :Discretize.h
/*	  Started:18.05.2010 21:28:12
/*    Updated:28.05.2010 21:38:12
/*    Author :SEEDMiner
/*    Subj   :Preprocessor class to discretize the continuous data
/*    Version: 
/************************************************************************/

class DiscretizeData
{
public:
	#pragma region Consturctors & Destructor

	__declspec(dllexport) DiscretizeData(void);
	__declspec(dllexport) DiscretizeData(long rowCount){this->_rowCount = rowCount;}
	__declspec(dllexport) DiscretizeData(WrapDataSource *ds){this->_ctsDs = ds;this->_rowCount = ds->noOfRows();}
	__declspec(dllexport) ~DiscretizeData(void);

	#pragma endregion Consturctors & Destructor

	/**	Upon passing a continuous attribute, this method will provide the discrete integer attribute*/
	__declspec(dllexport) EncodedIntAttribute* DiscretizeCtsAttribute(EncodedDoubleAttribute* _ctsAtt);

	/**	Method to discretize all the cts attributes in the data source at once*/
	__declspec(dllexport) void DiscretizeAllCtsAttributes();

	/**	Method to discretize just the cts attributes provided by the parameter vector*/
	__declspec(dllexport) void DiscretizeSelectedCtsAtts(vector<int> zeroBasedAttIDs);
	

private:
	WrapDataSource* _ctsDs;
	long _rowCount;
};
