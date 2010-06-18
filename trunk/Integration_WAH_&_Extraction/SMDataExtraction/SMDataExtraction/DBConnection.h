#include <vector>
#include <stdhead.h>
#include "DSNDriverInfo.h"
#include "SMException.h"
#include "SEEDMinerExceptions.h"
#include <string>
#include <wtypes.h>

using namespace DSNInfo;
using namespace std;

/************************************************************************/
/*    Class  :DBConnection.h
/*	  Started:18.02.2010 21:28:12
/*    Updated:18.02.2010 21:38:12
/*    Author :SEEDMiner
/*    Subj   :DataBase Connection class. Connection will be established 
/*			  using Microsoft ODBC drivers.
/*    Version: 
/************************************************************************/

namespace DBConnectionInfo{
	class DBConnection{
	public:
		__declspec(dllexport) DBConnection(char* DSN_Name,char* DB_UID,char* DB_PWD);
		__declspec(dllexport) DBConnection(char* DB_Connection_String);
		__declspec(dllexport) ~DBConnection(void);

		//Database Connection exportable methods for external activities.
		__declspec(dllexport) bool initiateConnectionToDB() throw(error_db_connection);
		__declspec(dllexport) bool closeConnectionWithDB()throw(error_db_connection);
		__declspec(dllexport) char* getDBConnectionString(void);
		__declspec(dllexport) CGOdbcConnect DBConnectionPtr(void);
		__declspec(dllexport) DSNDriverInfo DSNDriver(void);
		__declspec(dllexport) vector<string> getDataSourceNames(DSNInfo::DSNDriverInfo::DATASOURCE_TYPE _sourcetype) throw(error_odbc_reg_access);
		__declspec(dllexport) vector<string> QueryKey(HKEY hKey,string ds_string);


	private:
		char* _DSN_Name;
		char* _DB_UID;
		char* _DB_PWD;
		char* _DB_Connection_String;
		CGOdbcConnect _dbConPtr;
		DSNDriverInfo _dsnDriverInfo;

	};
}
