#include "StdAfx.h"
#include "AssociateRule.h"

	AssociateRule::AssociateRule(void)
	{
		m_premise_count = 1;
		m_consequence = NULL;
		m_premise = NULL;
	}

	AssociateRule::~AssociateRule(void)
	{
		//cout << "Deleting Associate Rules" << endl;
		delete m_consequence;
		delete m_premise;		
	}
	double AssociateRule::CalculateConfidence()
	{
		m_confidence = (double)m_consequence_count/(double)m_premise_count;
		// Value set for conseqence changes according to the hash function.
		// If a good hash function can be found there is no need to perform this check.
		if (m_confidence > 1)
		{
			m_confidence = 1;
			m_consequence_count = m_premise_count;
		}
		return m_confidence;
	}
