#pragma once

#include <string>

class TempStringObjects{
public:
	__declspec(dllexport) TempStringObjects(){}
	__declspec(dllexport) TempStringObjects(string m_val,int m_index){this->val = m_val;this->index = m_index;}
	__declspec(dllexport) std::string Val() const { return val; }
	__declspec(dllexport) void Val(std::string val) { val = val; }
	__declspec(dllexport) int Index() const { return index; }
	__declspec(dllexport) void Index(int val) { index = val; }
	__declspec(dllexport) bool operator <( const TempStringObjects& a) const
	{
		return a.Val() > this->val ;
	}
private:
	int index;
	string val;
};