#pragma once
#include "datasource.h"
#include "ModelSelection.h"
#include "BitStreamInfo.h"
#include "classifiersplitmodel.h"
#include "Utils.h"

class C45ModelSelection : public ModelSelection
{
public:
	C45ModelSelection(void);
	~C45ModelSelection(void);
	_declspec(dllexport) C45ModelSelection(int minNoObj, DataSource * allData,BitStreamInfo * _existence_bitmap, bool useMDLcorrection) ;
	_declspec(dllexport) ClassifierSplitModel * selectModel(DataSource * _data,BitStreamInfo * _existence_bitmap);

private:
	/** Minimum number of objects in interval. */
	int m_minNoObj;               

	/** Use MDL correction? */
	bool m_useMDLcorrection;         

	/** All the training data */
	DataSource * m_allData; // 

	/** Existence Bitmap */
	BitStreamInfo * m_existence_bitmap;
};
