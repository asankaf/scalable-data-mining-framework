#include "StdAfx.h"
#include "AssociateRule.h"
namespace Algorithm{
	AssociateRule::AssociateRule(void)
	{
		m_premise_count = 1;
	}

	AssociateRule::~AssociateRule(void)
	{
		delete m_consequence;
		delete m_premise;
	}
	double AssociateRule::CalculateConfidence()
	{
		m_confidence = (double)m_consequence_count/(double)m_premise_count;
		return m_confidence;
	}
}