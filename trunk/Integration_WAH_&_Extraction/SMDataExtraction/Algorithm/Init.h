#ifndef INIT_H
#define INIT_H
#include "Utils.h"
#include "EntropyBasedSplitCrit.h"
#include "C45Split.h"

/**Initialising constants defined in Utils*/
const double Utils::log2 = log10((double)2);
const double Utils::SMALL = 1e-6;

/**Initialising constants defined in EntropyBasedSplitCrit*/
double EntropyBasedSplitCrit::log2 = log10((double)2); 

/**Initialising constants defined in C45Split*/
InfoGainSplitCrit * C45Split::infoGainCrit = new InfoGainSplitCrit();
GainRatioSplitCrit * C45Split::gainRatioCrit = new GainRatioSplitCrit();
#endif

