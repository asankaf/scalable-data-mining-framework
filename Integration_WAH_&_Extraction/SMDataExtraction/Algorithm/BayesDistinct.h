#pragma once
#include "BitStreamInfo.h"
#include <xstring>

/************************************************************************
*    Class  :BayesDistinct	  
*    Author :Amila De Silva
*    Subj   :
* Class used for representing DistinctValues for NaiveBayes classifier.
*    Version: 1
************************************************************************/
class BayesDistinct
	{
	public:
		/***
		* Constructor
		*/
		BayesDistinct(void);
		
		/***
		* Initializes the BayesDistinct with the given Bitstream. 
		*/
		BayesDistinct(BitStreamInfo * _value);
		
		/***
		* Initializes the BayesDistinct with the given Bitstream and the 
		* attribute name.
		*/
		BayesDistinct(BitStreamInfo * _value,string _distinc_val);

		/***
		* Destructor
		*/
		~BayesDistinct(void);

		/***
		* Returns the value of the Bitstream
		*/
		BitStreamInfo * Value() const { return m_value; }

		/***
		* Sets the value of the Bitstream
		*/
		void Value(BitStreamInfo * val) { m_value = val; }

		/***
		* Gets the value of the Bitstream
		*/
		std::string name() const { return m_stringVal; }

		/***
		* Sets the value of the Bitstream
		*/
		void name(std::string val) { m_stringVal = val; }

		/***
		* Prints the contents of this class
		*/
		void Print();

	private:
		/***
		* Holds the contents of the DistinctValue
		*/
		BitStreamInfo * m_value;

		/***
		* Holds the string representation of the value.
		*/
		string m_stringVal;

};
