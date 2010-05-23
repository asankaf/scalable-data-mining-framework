/************************************************************************/
/*    Class  :DSNDriverInfo.h
/*	  Started:18.02.2010 21:45:12
/*    Updated:18.02.2010 21:45:12
/*    Author :SEEDMiner
/*    Subj   :Utility class to store DSN driver details.
/*    Version: 
/************************************************************************/

namespace DSNInfo{
	class DSNDriverInfo
	{
	public:
		enum DATASOURCE_TYPE{
			MySQL = 0,
			SQL_SERVER = 1,
			ORACLE = 2,
			TEXT_CSV = 3
		};
		__declspec(dllexport) DSNDriverInfo();
		__declspec(dllexport) DSNDriverInfo(const char* driverName);
		__declspec(dllexport) ~DSNDriverInfo(void);
		__declspec(dllexport) const char* DriverName(void);
		__declspec(dllexport) void setDSNDriverName(const char *driverName);

	private:
		const char* _driverName;
	};
}
