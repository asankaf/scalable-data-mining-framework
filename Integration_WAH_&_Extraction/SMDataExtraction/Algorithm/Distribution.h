#pragma once
#include "datasource.h"
#include "BitStreamInfo.h"
#include "Utils.h"
#include <vector>


class Distribution
{
public:

	Distribution(void);

	/** Creates and initializes a new distribution.*/
	Distribution(int numBags,int numClasses);

	Distribution(Distribution * toMerge);

	Distribution(DataSource * source, BitStreamInfo * _existence_map);

	_declspec(dllexport) void Print();

	~Distribution(void);

	/**
	* Returns class with highest frequency over all bags.
	*/
	int maxClass();

	/**
	* Returns number of (possibly fractional) instances of given class.
	*/
	double perClass(int classIndex);

	/**
	* Checks if at least two bags contain a minimum number of instances.
	*/
	bool check(double minNoObj);

	/** Creates a distribution depending on existence bitmap */
	void add(int bagIndex,DataSource * instance, BitStreamInfo * _existence_map); 

	/**Public getters and setters*/

	double Total() const { return totaL; }

	void Total(double val) { totaL = val; }

	int numClasses() const { return m_perClassLength; }

	void numClasses(int val) { m_perClassLength = val; }

	int numBags() const { return m_perBagLength; }

	void numBags(int val) { m_perBagLength = val; }

	/**
	* Returns number of (possibly fractional) instances in given bag.
	*/
	double perBag(int bagIndex);
	
		/**
	* Returns number of (possibly fractional) instances of given class in 
	* given bag.
	*/
	double perClassPerBag(int bagIndex, int classIndex);
		
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
	
	void ClearClassPerBags();

	void ClearClass();

	void ClearBags();

	/** Arrays are initialized by default to maximum negative number. Here it is set to zero.*/
	void initialiseArrays();
	
};
