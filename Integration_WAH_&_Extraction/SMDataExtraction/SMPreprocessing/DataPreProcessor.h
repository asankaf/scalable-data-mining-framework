#pragma once
#include "WrapDataSource.h"

class DataPreProcessor
{
public:
	__declspec(dllexport) DataPreProcessor(void);
	__declspec(dllexport) ~DataPreProcessor(void);
	WrapDataSource *m_original_datasource;
};
