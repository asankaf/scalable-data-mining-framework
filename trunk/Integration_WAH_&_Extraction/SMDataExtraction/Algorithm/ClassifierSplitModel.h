#pragma once
#include "distribution.h"
#include <iostream>

using namespace std;
class ClassifierSplitModel
{
public:
	_declspec(dllexport) ClassifierSplitModel(void);
	_declspec(dllexport) virtual ~ClassifierSplitModel(void);
	_declspec(dllexport) virtual void buildClassifier(DataSource * _source, BitStreamInfo * _existence) = 0;
	_declspec(dllexport) int NumSubsets() const { return m_numSubsets; }
	_declspec(dllexport) void NumSubsets(int val) { m_numSubsets = val; }
	_declspec(dllexport) Distribution * Dist() const { return m_distribution; }
	_declspec(dllexport) void Dist(Distribution * val) { m_distribution = val; }
	bool checkModel();
	_declspec(dllexport) void Print();
	vector<BitStreamInfo *> Existence_maps() const { return m_existence_maps; }

	void Existence_maps(vector<BitStreamInfo *>  & val) { m_existence_maps = val; }

protected:
	/** Distribution of class values. */  
	Distribution * m_distribution;  
	
	/** Number of created subsets. */
	int m_numSubsets;

	/** Holds the existence bitmaps corresponding to each value under current attribtue*/
	vector<BitStreamInfo *> m_existence_maps;	
	
};
