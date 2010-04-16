#include "StdAfx.h"
#include "ClassifierSplitModel.h"

ClassifierSplitModel::ClassifierSplitModel(void)
{
}

ClassifierSplitModel::~ClassifierSplitModel(void)
{
}

void ClassifierSplitModel::Print()
{
	cout << "Printing ClassifierSplitModel :" << endl;
	m_distribution->Print();
	cout << "No of subsets : " << m_numSubsets << endl;
	cout << endl;
}

bool ClassifierSplitModel::checkModel()
{
	if (m_numSubsets > 0)
		return true;
	else
		return false;
}
