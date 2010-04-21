#include "StdAfx.h"
#include "ClassifierSplitModel.h"

ClassifierSplitModel::ClassifierSplitModel(void)
{
	m_distribution = NULL;
}

ClassifierSplitModel::~ClassifierSplitModel(void)
{
	if (m_distribution != NULL)
	{
		delete m_distribution;
		m_distribution = NULL;
	}
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

string ClassifierSplitModel::dumpLabel(int index,DataSource * data)
{
	string text;
	
	text.append(data->classAttribute()->value(m_distribution->maxClass(index)));
	text.append(" ("+Utils::toStringVal(m_distribution->perBag(index),2));
	if (Utils::gr(m_distribution->numIncorrect(index),0))
		text.append("/"+Utils::toStringVal(m_distribution->numIncorrect(index),2));
	text.append(")");

	return text;
}
