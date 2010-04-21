#include "StdAfx.h"
#include "NoSplit.h"

NoSplit::NoSplit(void)
{
}

string NoSplit::leftSide(DataSource * data)
{
	return "";
}

string NoSplit::rightSide(int index,DataSource * data)
{
	return "";
}

NoSplit::~NoSplit(void)
{
	if (m_distribution == NULL)
	{
		delete m_distribution;
		m_distribution = NULL;
	}
}

NoSplit::NoSplit(Distribution * _dist)
{
	m_distribution = new Distribution(_dist);
	m_numSubsets = 1;	
}

void NoSplit::buildClassifier(DataSource * _source, BitStreamInfo * _existence)
{
	//Setting the dstribution corresponding to this object.
	Dist(new Distribution(_source,_existence));

	//Setting the value of the subsets. In the NoSplit model all the instances fall into 
	//one subset.
	NumSubsets(1);	
}
