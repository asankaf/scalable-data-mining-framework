#include "WrapDataSource.h"

class DataSources{
public:
	__declspec(dllexport) DataSources();
	__declspec(dllexport) ~DataSources();
	__declspec(dllexport) WrapDataSource* operator()(int dataSourceID);
	__declspec(dllexport) EncodedAttributeInfo* operator()(int dataSourceID,int attID);
	__declspec(dllexport) BitStreamInfo* operator()(int dataSourceID,int attID,int bitStreamID);
	__declspec(dllexport) void insertDataSources(WrapDataSource* ds);

private:
	vector<WrapDataSource*> _dataSources;
};