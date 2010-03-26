#include <vector>
#include <string>
#pragma once
using namespace std;
namespace Algorithm{
	class AssociateRule
	{
	public:
		_declspec(dllexport) AssociateRule(void);
		_declspec(dllexport) ~AssociateRule(void);
		_declspec(dllexport) vector<int> Antecedant() const { return m_antecedant; }
		_declspec(dllexport) void Antecedant(vector<int> val) { m_antecedant = val; }
		_declspec(dllexport) vector<int> Consequent() const { return m_consequent; }
		_declspec(dllexport) void Consequent(vector<int> val) { m_consequent = val; }
		_declspec(dllexport) int Support() const { return m_support; }
		_declspec(dllexport) void Support(int val) { m_support = val; }
		_declspec(dllexport) double Confidence() const { return m_confidence; }
		_declspec(dllexport) void Confidence(double val) { m_confidence = val; }
		_declspec(dllexport) std::string Rule() const { return m_rule; }
		_declspec(dllexport) void Rule(std::string val) { m_rule = val; }
		_declspec(dllexport) int * Consequence() const { return m_consequence; }
		_declspec(dllexport) void Consequence(int * val) { m_consequence = val; }
		_declspec(dllexport) int Array_length() const { return m_array_length; }
		_declspec(dllexport) void Array_length(int val) { m_array_length = val; }
		_declspec(dllexport) int * Premise() const { return m_premise; }
		_declspec(dllexport) void Premise(int * val) { m_premise = val; }
		_declspec(dllexport) int Premise_count() const { return m_premise_count; }
		_declspec(dllexport) void Premise_count(int val) { m_premise_count = val; }
		_declspec(dllexport) int Consequence_count() const { return m_consequence_count; }
		_declspec(dllexport) void Consequence_count(int val) { m_consequence_count = val; }
		_declspec(dllexport) double CalculateConfidence();

	private:
		vector<int> m_antecedant;		
		int m_array_length;		
		int * m_premise;		
		int * m_consequence;		
		vector<int> m_consequent;	
		string m_rule;		
		int m_support;		
		double m_confidence;
		int m_premise_count;		
		int m_consequence_count;		
	};
}