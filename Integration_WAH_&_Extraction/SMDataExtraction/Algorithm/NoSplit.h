#pragma once
#include "classifiersplitmodel.h"

class NoSplit :
	public ClassifierSplitModel
{
public:
	NoSplit(void);
	_declspec(dllexport) NoSplit(Distribution * _dist);
	~NoSplit(void);
	_declspec(dllexport) void buildClassifier(DataSource * _source, BitStreamInfo * _existence);
};
