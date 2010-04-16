#include "StdAfx.h"
#include "NoSplit.h"

NoSplit::NoSplit(void)
{
}

NoSplit::~NoSplit(void)
{
	Distribution * dist = Dist();
	if (dist != NULL)
	{
		delete dist;
		Dist(NULL);
	}
}

NoSplit::NoSplit(Distribution * _dist)
{
	Dist(new Distribution(_dist));
	NumSubsets(1);
}

void NoSplit::buildClassifier(DataSource * _source, BitStreamInfo * _existence)
{
	//Setting the dstribution corresponding to this object.
	Dist(new Distribution(_source,_existence));

	//Setting the value of the subsets. In the NoSplit model all the instances fall into 
	//one subset.
	NumSubsets(1);	
}
