#include "EncodedDoubleAttribute.h"
#include "EncodedIntAttribute.h"
#include "WrapDataSource.h"
#pragma once

class DiscretizeData
{
public:
	__declspec(dllexport) DiscretizeData(void);
	__declspec(dllexport) DiscretizeData(long rowCount){this->_rowCount = rowCount;}
	__declspec(dllexport) DiscretizeData(WrapDataSource *ds){this->_ctsDs = ds;this->_rowCount = ds->noOfRows();}
	__declspec(dllexport) EncodedIntAttribute* DiscretizeCtsAttribute(EncodedDoubleAttribute* _ctsAtt);
	__declspec(dllexport) void DiscretizeAllCtsAttributes();
	__declspec(dllexport) void DiscretizeSelectedCtsAtts(vector<int> zeroBasedAttIDs);
	__declspec(dllexport) ~DiscretizeData(void);

private:
	WrapDataSource* _ctsDs;
	long _rowCount;
};
