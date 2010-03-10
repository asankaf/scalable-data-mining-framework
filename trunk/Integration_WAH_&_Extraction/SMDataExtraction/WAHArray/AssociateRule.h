#include <vector>
#include <string>
#pragma once
using namespace std;
namespace Algorithm{
	class AssociateRule
	{
	public:
		AssociateRule(void);
		~AssociateRule(void);
		vector<int> Antecedant() const { return m_antecedant; }
		void Antecedant(vector<int> val) { m_antecedant = val; }
		vector<int> Consequent() const { return m_consequent; }
		void Consequent(vector<int> val) { m_consequent = val; }
		int Support() const { return m_support; }
		void Support(int val) { m_support = val; }
		int Confidence() const { return m_confidence; }
		void Confidence(int val) { m_confidence = val; }
		std::string Rule() const { return m_rule; }
		void Rule(std::string val) { m_rule = val; }

	private:
		vector<int> m_antecedant;		
		vector<int> m_consequent;	
		string m_rule;		
		int m_support;		
		int m_confidence;		
	};
}