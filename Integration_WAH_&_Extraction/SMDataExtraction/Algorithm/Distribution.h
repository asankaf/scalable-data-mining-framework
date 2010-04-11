#pragma once

class Distribution
{
public:
	Distribution(void);
	Distribution(int numBags,int numClasses);
	~Distribution(void);
	
private:

	/** Weight of instances per class per bag. */
	double ** m_perClassPerBag ; 

	/** Weight of instances per bag. */
	double * m_perBag;           

	/** Length of the m_perBag array */
	int m_perBagLength;

	/** Weight of instances per class. */
	double * m_perClass;         

	/** Length of the m_perClass array */
	int m_perClassLength;

	/** Total weight of instances. */
	double totaL;
};
