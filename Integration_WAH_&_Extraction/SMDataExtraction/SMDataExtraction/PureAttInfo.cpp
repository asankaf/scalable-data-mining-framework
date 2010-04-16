#include "StdAfx.h"
#include "PureAttInfo.h"
#include <math.h>

PureAttInfo::PureAttInfo(void)
{

}

PureAttInfo::~PureAttInfo(void)
{
}

boost::dynamic_bitset<> PureAttInfo::existanceBitSet;
vector<bool> PureAttInfo::existanceBitMap;
int PureAttInfo::NULLVAL = 0;