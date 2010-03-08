#include "stdafx.h"
#include "DSNDriverInfo.h"
using namespace DSNInfo;

namespace DSNInfo{
		DSNDriverInfo::DSNDriverInfo(){
			this->_driverName = "";
		}

		DSNDriverInfo::DSNDriverInfo(const char* driverName){
			this->_driverName = driverName;	
		}

		DSNDriverInfo::~DSNDriverInfo(){
			//Default destructor.
		}

		void DSNDriverInfo::setDSNDriverName(const char *driverName){
			this->_driverName = driverName;
		}
	
		const char* DSNDriverInfo::DriverName(){
			return this->_driverName;
		}
}