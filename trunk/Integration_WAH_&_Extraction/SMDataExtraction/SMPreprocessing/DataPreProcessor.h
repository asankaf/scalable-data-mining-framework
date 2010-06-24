#pragma once
#include "WrapDataSource.h"

/************************************************************************/
/*    Class  :DataPreprocessor.h
/*	  Started:18.05.2010 21:28:12
/*    Updated:18.05.2010 21:38:12
/*    Author :SEEDMiner
/*    Subj   :Base class for all preprocessor filters
/*    Version: 
/************************************************************************/
class DataPreProcessor
{
public:
	#pragma region Constructors & Destructor

	__declspec(dllexport) DataPreProcessor(void);
	__declspec(dllexport) ~DataPreProcessor(void);

	#pragma endregion Constructors & Destructor
	
	/**	original wrapped data source*/
	WrapDataSource *m_original_datasource;
};
