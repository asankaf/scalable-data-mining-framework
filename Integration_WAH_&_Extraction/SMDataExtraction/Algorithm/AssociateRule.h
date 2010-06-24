#ifndef _ASSOCIATERULE_H
#define _ASSOCIATERULE_H

#include <vector>
#include <string>
#include <iostream>
#include "smalgorithmexceptions.h"
using namespace std;
/************************************************************************
*    Class  :AssociateRule	  
*    Author :Amila De Silva
*    Subj   :
* Class for storing Association Rules. An associate rule consists of an 
* antecedent and consequent. Antecedant gives the items to be looked at.
* 
*    Version: 1
************************************************************************/
	class AssociateRule
	{
	public:
		/***
		* Constructor
		*/
		_declspec(dllexport) AssociateRule(void);

		/***
		* Destructor
		*/
		_declspec(dllexport) ~AssociateRule(void);

		/***
		* Gives the antecedent vector. The vector contains the itemsets encoded as integers
		*/
		_declspec(dllexport) vector<int> Antecedant() const { return m_antecedant; }

		/***
		* Sets the set of antecedent for this rule.
		*/
		_declspec(dllexport) void Antecedant(vector<int> val) { m_antecedant = val; }

		/***
		* Gives the consequent vector.
		*/
		_declspec(dllexport) vector<int> Consequent() const { return m_consequent; }

		/***
		* Sets the consequents for this rule
		*/
		_declspec(dllexport) void Consequent(vector<int> _val) { m_consequent = _val; }

		/***
		* Returns the support for this rule.
		*/
		_declspec(dllexport) int Support() const { return m_support; }

		/***
		* Sets the support for this rule.
		*/
		_declspec(dllexport) void Support(int _val) { m_support = _val; }

		/***
		* Gives the confidence for this rule.
		*/
		_declspec(dllexport) double Confidence() const { return m_confidence; }

		/***
		* Sets the confidence of the current rule.
		*/
		_declspec(dllexport) void Confidence(double _val) { m_confidence = _val; }

		/***
		* Gives the string representation of the association rule.
		*/
		_declspec(dllexport) std::string Rule() const { return m_rule; }

		/***
		* Sets the string representation of the Current rule.
		*/
		_declspec(dllexport) void Rule(std::string _val) { m_rule = _val; }

		/***
		* Gets the array containing items in the consequence.
		*/
		_declspec(dllexport) int * Consequence() const { return m_consequence; }

		/***
		* Sets the array containing items in the consequence.
		*/
		_declspec(dllexport) void Consequence(int * _val) { m_consequence = _val; }

		/***
		* Gets the length of the item array
		*/
		_declspec(dllexport) int Array_length() const { return m_array_length; }

		/***
		* Sets the length of the item array
		*/
		_declspec(dllexport) void Array_length(int _val) { m_array_length = _val; }

		/***
		* Returns the array for Antecedant
		*/
		_declspec(dllexport) int * Premise() const { return m_premise; }

		/***
		* Sets the array for antecedent
		*/
		_declspec(dllexport) void Premise(int * _val) { m_premise = _val; }

		/***
		* Gets the support of the antecedent
		*/
		_declspec(dllexport) int Premise_count() const { return m_premise_count; }

		/***
		* Sets the support of the antecedent
		*/
		_declspec(dllexport) void Premise_count(int _val) { m_premise_count = _val; }

		/***
		* Gets the support of the consequence
		*/
		_declspec(dllexport) int Consequence_count() const { return m_consequence_count; }

		/***
		* Sets the support of the consequence
		*/
		_declspec(dllexport) void Consequence_count(int _val) { m_consequence_count = _val; }

		/***
		* Calculate the confidence for this rule
		*/
		_declspec(dllexport) double CalculateConfidence() throw (algorithm_exception);

	private:
		/** The items of the antecedent*/ 
		vector<int> m_antecedant;		

		/** The array length*/ 
		int m_array_length;		

		/** The items in the Antecedent*/ 
		int * m_premise;		

		/** The items in the consequence*/ 
		int * m_consequence;		

		/** The items in the consequence*/ 
		vector<int> m_consequent;	

		/** The string representation of the rule*/ 
		string m_rule;		

		/** The minimum support*/ 
		int m_support;		

		/** The confidence of the rule*/ 
		double m_confidence;

		/** The support of the antecedent*/ 
		int m_premise_count;		

		/** Stores the support of the consequence*/ 
		int m_consequence_count;		
	};

#endif