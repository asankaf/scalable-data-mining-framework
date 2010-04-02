#pragma once
#include <vector>

namespace AttributeInfo{
	class PureIntAttributeInfo{
	public:
		__declspec(dllexport) PureIntAttributeInfo();
		__declspec(dllexport) ~PureIntAttributeInfo();
		__declspec(dllexport) int Upper();
		__declspec(dllexport) int Lower();
		__declspec(dllexport) int* ValueList();
		__declspec(dllexport) void setValueList(int *values);
	
	private:
		int _upper;
		int _lower;
		int* _valueList;

	};
}