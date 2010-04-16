#pragma once

struct Type 
{
	static const int UNSIGNED_INT = 0;
	static const int SIGNED_INT = 1;
	static const int MULTI_CAT = 2;
	static const int TYPE_DATE = 3;
	static const int SKIP = 4;

	
};

__declspec(dllexport) static enum ATT_TYPE{

	SIGNEDINT_VAL = 0,
	DOUBLE_VAL = 1,
	DATE_VAL = 2,
	MULTICAT_VAL = 3,
	SKIP_VAL = 4

};

