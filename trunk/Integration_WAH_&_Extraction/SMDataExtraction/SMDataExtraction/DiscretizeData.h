#include "EncodedDoubleAttribute.h"
#include "EncodedIntAttribute.h"
#pragma once

class DiscretizeData
{
public:
	__declspec(dllexport) DiscretizeData(void);
	__declspec(dllexport) DiscretizeData(EncodedDoubleAttribute* doubleAtt,long rowCount){this->_ctsAtt = doubleAtt;this->_rowCount = rowCount;}
	__declspec(dllexport) EncodedIntAttribute* DiscretizeCtsAttribute();
	__declspec(dllexport) ~DiscretizeData(void);

private:
	EncodedDoubleAttribute* _ctsAtt;
	long _rowCount;
};
