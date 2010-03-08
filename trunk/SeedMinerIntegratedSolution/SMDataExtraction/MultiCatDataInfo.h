#pragma once
#include <vector>
#include <string>

using namespace std;

class MultiCatDataInfo
{
public:
	__declspec(dllexport) MultiCatDataInfo(vector<string> uniqueDataList);
	__declspec(dllexport) ~MultiCatDataInfo(void);
	__declspec(dllexport) int* getAssignedEncodedNumberList();
	__declspec(dllexport) int noOfBitsNeeded();

private:
	vector<string> _uniqueStrings;
	int _noOfBitsNeeded;
	int* _assignedEncodedNumbers;
};
