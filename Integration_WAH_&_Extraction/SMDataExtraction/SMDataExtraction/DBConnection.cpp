#include "stdafx.h"
#include "DBConnection.h"
#include "SEEDMinerExceptions.h"
#include <boost/exception/all.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include <stdexcept>
#include <string>
#include <iostream>
#include <wtypes.h>
#include <vector>
#include "exceptionreader.h"

using namespace boost;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

namespace DBConnectionInfo{
	DBConnectionInfo::DBConnection::DBConnection(char* DSN_Name,char* DB_UID,char* DB_PWD){
		this->_DSN_Name = DSN_Name;
		this->_DB_UID = DB_UID;
		this->_DB_PWD = DB_PWD;
	}

	DBConnectionInfo::DBConnection::DBConnection(char* DB_Connection_String){
		this->_DB_Connection_String = DB_Connection_String;
	}
	
	bool DBConnectionInfo::DBConnection::initiateConnectionToDB(){
		try{
// 			if (this->_DB_Connection_String == NULL)
// 			{
// 				this->_dbConPtr.connect(this->_DB_Connection_String);		
// 			}
// 			else
// 			{
			this->_dbConPtr.connect(this->_DSN_Name,this->_DB_UID,this->_DB_PWD);
			this->_dsnDriverInfo.setDSNDriverName(this->_dbConPtr.getDriver());
			
			return true;
		}
		catch(CGOdbcEx *e)
		{
			string dsn_name(this->_DSN_Name);
			string err = "SM1001";
			string db_error  = ExceptionReader::GetError(err);
			//string db_error = "SEEDMiner Exception: \nError in connecting to database: Possible error in DSN or database authentication info";
			db_error += "\nProvided DSN: " + dsn_name;
			BOOST_THROW_EXCEPTION(error_db_connection(db_error));					
		}
	}

	bool DBConnectionInfo::DBConnection::closeConnectionWithDB(){
		
		try{
			this->_dbConPtr.close();
			return true;
		}
		catch(CGOdbcEx *ex){
			
			string dsn_name(this->_DSN_Name);
			string db_error = "SEEDMiner Exception: \nError in closing connection to Database.";
			db_error += "\nProvided DSN: " + dsn_name;
			BOOST_THROW_EXCEPTION(error_db_connection(db_error));	
			return false;
		}
	}

	DSNInfo::DSNDriverInfo  DBConnectionInfo::DBConnection::DSNDriver(){
		return this->_dsnDriverInfo;
	}

	CGOdbcConnect DBConnectionInfo::DBConnection::DBConnectionPtr(){
		return this->_dbConPtr;
	}

	char* DBConnectionInfo::DBConnection::getDBConnectionString(){
		return this->_DB_Connection_String;
	}

	DBConnectionInfo::DBConnection::~DBConnection(){
		this->closeConnectionWithDB();
		//default destructor.
		std::cout<<"DBConnection destructor calls"<<std::endl;
	}

	vector<string> DBConnectionInfo::DBConnection::getDataSourceNames(DSNInfo::DSNDriverInfo::DATASOURCE_TYPE _sourcetype){
		HKEY hTestKey;
		string dsName;
		switch((int) _sourcetype){
		case 0:
			{
				dsName = "MySQL";
				break;
			}
		case 1:
			{
				dsName = "SQL Native";
				break;
			}
		case 2:
			{
				dsName = "Oracle";
				break;
			}
		case 3:
			{
				dsName = "Microsoft Text";
				break;
			}
		default:
			{
				dsName = "skip";
				break;
			}
		}

		if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			TEXT("SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources"),
			0,
			KEY_READ,
			&hTestKey) == ERROR_SUCCESS
			)
		{
			return QueryKey(hTestKey,dsName);
		}

		RegCloseKey(hTestKey);
	}

	vector<string> DBConnectionInfo::DBConnection::QueryKey(HKEY hKey,string ds_string) 
	{ 
		vector<string> dataSourceNames;

		TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
		DWORD    cbName;                   // size of name string 
		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
		DWORD    cchClassName = MAX_PATH;  // size of class string 
		DWORD    cSubKeys=0;               // number of subkeys 
		DWORD    cbMaxSubKey;              // longest subkey size 
		DWORD    cchMaxClass;              // longest class string 
		DWORD    cValues;              // number of values for key 
		DWORD    cchMaxValue;          // longest value name 
		DWORD    cbMaxValueData;       // longest value data 
		DWORD    cbSecurityDescriptor; // size of security descriptor 
		FILETIME ftLastWriteTime;      // last write time 

		DWORD i, retCode; 

		TCHAR  achValue[MAX_VALUE_NAME]; 
		DWORD cchValue = MAX_VALUE_NAME; 

		// Get the class name and the value count. 
		retCode = RegQueryInfoKey(
			hKey,                    // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 

		// Enumerate the subkeys, until RegEnumKeyEx fails.

		if (cSubKeys)
		{
			//printf( "\nNumber of subkeys: %d\n", cSubKeys);

			for (i=0; i<cSubKeys; i++) 
			{ 
				cbName = MAX_KEY_LENGTH;
				retCode = RegEnumKeyEx(hKey, i,
					achKey, 
					&cbName, 
					NULL, 
					NULL, 
					NULL, 
					&ftLastWriteTime); 
				if (retCode == ERROR_SUCCESS) 
				{
					//_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
				}
			}
		} 

		// Enumerate the key values. 

		if (cValues) 
		{
			//printf( "\nNumber of values: %d\n", cValues);

			for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
			{ 
				cchValue = MAX_VALUE_NAME; 
				achValue[0] = '\0'; 
				retCode = RegEnumValue(hKey, i, 
					achValue, 
					&cchValue, 
					NULL, 
					NULL,
					NULL,
					NULL);

				if (retCode == ERROR_SUCCESS ) 
				{ 
					//_tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
					unsigned long datalen = 255;
					char* val = new char[MAX_VALUE_NAME];
					wcstombs(val,achValue,MAX_VALUE_NAME);
					string s(val);
					delete val;
					LPCSTR name(s.c_str());
					char *readData = new char[255];
					if (RegQueryValueExA(hKey,
						name,
						NULL,
						0,
						(LPBYTE)readData,
						&datalen) == ERROR_SUCCESS){}
					//printf("Srvc Level:\t%s\n",readData);
					string read_str(readData);
					int j;
					if ((j = read_str.find(ds_string.c_str(),0)) != string::npos)
					{
						string dest_str((char*)name);
						dataSourceNames.push_back(dest_str);
						//dataSourceNames.push_back("Test");
					}
				} 
			}
		}
		return dataSourceNames;
	}
}