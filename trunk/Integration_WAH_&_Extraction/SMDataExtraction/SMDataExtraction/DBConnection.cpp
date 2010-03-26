#include "stdafx.h"
#include "DBConnection.h"
#include <stdexcept>
#include <iostream>

namespace DBConnectionInfo{
	DBConnectionInfo::DBConnection::DBConnection(char* DSN_Name,char* DB_UID,char* DB_PWD){
		this->_DSN_Name = DSN_Name;
		this->_DB_UID = DB_UID;
		this->_DB_PWD = DB_PWD;
// 		this->_DB_Connection_String = "DSN = ";
// 		this->_DB_Connection_String = strcpy(this->_DB_Connection_String,this->_DSN_Name);
// 		this->_DB_Connection_String = strcat(this->_DB_Connection_String,"; UID = ");
// 		this->_DB_Connection_String = strcat(this->_DB_Connection_String,this->_DB_UID);
// 		this->_DB_Connection_String = strcat(this->_DB_Connection_String,"; PWD = ");
// 		this->_DB_Connection_String = strcat(this->_DB_Connection_String,this->_DB_PWD);
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
			//}
			this->_dsnDriverInfo.setDSNDriverName(this->_dbConPtr.getDriver());
			
			return true;
		}
		catch(CGOdbcEx *e)
		{
			std::cerr<<"Error in connecting to database: Possible error in DSN or database authentication info"<<std::endl;
			exit(1);
			//TODO add logging method to record error condition.
			return false;			
		}
	}

	bool DBConnectionInfo::DBConnection::closeConnectionWithDB(){
		
		try{
			this->_dbConPtr.close();
			return true;
		}
		catch(CGOdbcEx *ex){
			//TODO add logging method to record error condition.
			std::cerr<<"Error in closing connection to Database."<<std::endl;
			exit(4);
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
}