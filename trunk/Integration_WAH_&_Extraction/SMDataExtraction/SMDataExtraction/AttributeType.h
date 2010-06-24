#pragma once

/************************************************************************/
/*    AttributeType.h
/*	  Started:28.03.2010 21:45:12
/*    Updated:18.05.2010 21:45:12
/*    Author :SEEDMiner
/*    Subj   :Storage class to store widely using enums and other struts
/*    Version: 
/************************************************************************/


// struct to represent the attribute type
struct Type 
{
	static const int UNSIGNED_INT = 0;
	static const int SIGNED_INT = 1;
	static const int MULTI_CAT = 2;
	static const int TYPE_DATE = 3;
	static const int SKIP = 4;

	
};

//An enum to represent attribute type
__declspec(dllexport) static enum ATT_TYPE{

	SIGNEDINT_VAL = 0,
	DOUBLE_VAL = 1,
	DATE_VAL = 2,
	MULTICAT_VAL = 3,
	SKIP_VAL = 4

};

//struct to represent attribute properties
struct AttributeProps{
	string Name;
	int ID;
};