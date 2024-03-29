#pragma once
#include "datasource.h"
#include "BitStreamInfo.h"
#include "Utils.h"
#include <vector>

/************************************************************************
*    Class  :Distribution	  
*    Author :Amila De Silva
*    Subj   :
* Class for handling a distribution of class values.
*    Version: 1
************************************************************************/
class Distribution
{
public:

	/***
	* Constructor
	*/
	Distribution(void);

	/***
	* Creates and initializes a new distribution.
	*/
	Distribution(int numBags,int numClasses);

	/**
	* Creates distribution with only one bag by merging all
	* bags of given distribution.
	*/
	Distribution(Distribution * toMerge);

	/**
	* Creates distribution with only one bag by merging all
	* bags of given distribution.
	*/
	Distribution(DataSource * source, BitStreamInfo * _existence_map);

	/***
	* Prints the values of the distribution to the console.
	*/
	_declspec(dllexport) void Print();

	/***
	* Destructor
	*/
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

	/***
	* Returns the Total weights of the instances
	*/
	double Total() const { return totaL; }

	/***
	* Sets the Total weights of the instances
	*/
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

	/**
	* Returns perBag(index)-numCorrect(index).
	*/
	double numIncorrect(int index);

	/**
	* Returns perClassPerBag(index,maxClass(index)).
	*/
	double numCorrect(int index);

	/**
	* Returns class with highest frequency for given bag.
	*/
	int maxClass(int index);

	/**
	* Returns perClass(maxClass()).
	*/
	double numCorrect();

	/**
	* Returns total-numCorrect().
	*/
	double numIncorrect();

	/**
	* Returns index of bag containing maximum number of instances.
	*/
	int maxBag();

		
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
