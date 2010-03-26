#pragma once
#include <vector>
#include <set>
#include <string>

using namespace std;

class MultiCatDataInfo
{
public:
	__declspec(dllexport) MultiCatDataInfo(std::set<string> uniqueDataList);
	__declspec(dllexport) ~MultiCatDataInfo(void);
	__declspec(dllexport) int* getAssignedEncodedNumberList();
	__declspec(dllexport) int noOfBitsNeeded();

private:
	std::set<string> _uniqueStrings;
	int _noOfBitsNeeded;
	int* _assignedEncodedNumbers;
};
