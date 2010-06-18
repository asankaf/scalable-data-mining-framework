#include "stdafx.h"
#include "math.h"

#include "DBQueryExecution.h"
#include "PureIntAttInfo.h"
#include "PureDoubleAttInfo.h"
#include "PureStringAttInfo.h"
#include "attributetype.h"
#include "SMException.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stack>
#include <list>
#include "Commons.h"
#include "ExceptionReader.h"
#include "ExceptionCodes.h"
using namespace std;


namespace DBQueryExecutionInfo{
	DBQueryExecutionInfo::DBQueryExecution::DBQueryExecution(const char* statement){
		Init();
		this->_query_stmt = statement;
		_isRowCountSet = false;
	}

	DBQueryExecutionInfo::DBQueryExecution::DBQueryExecution(){ Init();_isRowCountSet = false;}

	void DBQueryExecution::Init(){
		Commons::InitVector(this->_intData.begin(),this->_intData.end());
		Commons::InitVector(this->_doubleData.begin(),this->_doubleData.end());
		Commons::InitVector(this->_stringData.begin(),this->_stringData.end());
	}

	bool DBQueryExecutionInfo::DBQueryExecution::ExecuteQueryAndBindData(CGOdbcConnect &cCon){
		clock_t start,end;

		try{
			this->_stmtPtr = cCon.createStatement();
			start = clock();
			this->_stmtPtr->execute(this->_query_stmt);
			this->_stmtPtr->bindAuto();
			setRowCount();
			end = clock();
			cout<<"Time to execute and bind data : "<<(end - start)<<endl;
		}
		catch(CGOdbcEx *e){
			error_db_query_execution ex;
			ex << error_message(ExceptionReader::GetError(SM1012));
			ex << error_code(SM1012);
			BOOST_THROW_EXCEPTION(ex);
		}

		CGOdbcStmt *pCur= this->_stmtPtr;
		bool BRC;
		int colCount = pCur->getColCount();
		PureAttInfo::existanceBitSet.resize(this->_rowCount);

		try
		{
			for (int it=0 ; it < colCount ; it++)
			{				
				BRC=false;
				int columnType=pCur->getColumn(it)->iSqlType;

				//start = clock();
				if ((columnType == SQL_BIT) || (columnType == SQL_TINYINT) || (columnType == SQL_SMALLINT) || (columnType == SQL_INTEGER))
				{
					PureIntAttInfo *intAtt = new PureIntAttInfo();

					intAtt->type = Type.SIGNED_INT;
					intAtt->attName = pCur->getColumn(it)->szName;
					intAtt->attID = it;

					intAtt->setLower((int)(pow(2.0,31.0) - 1));
					intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);
					vector<long int> intValList;
					intValList.resize(this->_rowCount);
					int currRow = 0;
					for (BRC = pCur->first(); (BRC && currRow < this->_rowCount) ; BRC = pCur->next())
					{						
						if (pCur->isNull(it))
						{
							//PureAttInfo::existanceBitSet[currRow] = false;
							intValList[currRow] = PureAttInfo::NULLVAL;
							currRow++;
							continue;
						}
						PureAttInfo::existanceBitSet[currRow] = true;
						long int tempVal = pCur->getInt(it);
						intValList[currRow] = tempVal;
						if (tempVal > intAtt->Upper())
						{
							intAtt->setUpper(tempVal);
						}
						if (tempVal < intAtt->Lower())
						{
							intAtt->setLower(tempVal);
						}
						currRow++;
					}
					intAtt->setValList(intValList);
					this->_intData.push_back(intAtt);
					//end = clock();
					//cout<<"Time to Retrieve & bind INT Data : "<< (end - start) <<endl;
				}

				else if ((columnType == SQL_DOUBLE) || (columnType == SQL_FLOAT) || (columnType == SQL_NUMERIC) || (columnType == SQL_REAL) || (columnType == SQL_DECIMAL))
				{

					//start = clock();
					PureDoubleAttInfo *doubleAtt=new PureDoubleAttInfo();

					doubleAtt->type = Type.SIGNED_INT;
					doubleAtt->attName = pCur->getColumn(it)->szName;
					doubleAtt->attID = it;

					doubleAtt->setLower(pow(2.0,63.0) - 1);
					doubleAtt->setUpper((pow(2.0,63.0) - 1) * -1);

					vector<double> doublevals;
					doublevals.resize(this->_rowCount);
					int currRow = 0;
					for (BRC = pCur->first() ; (BRC && currRow < this->_rowCount) ; BRC = pCur->next())
					{
						if (pCur->isNull(it))
						{
							//PureAttInfo::existanceBitSet[currRow] = false;
							doublevals[currRow] = PureAttInfo::NULLVAL;
							currRow++;
							continue;
						}
						PureAttInfo::existanceBitSet[currRow] = true;
						double tempVal = pCur->getNumber(it);
						doublevals[currRow] = tempVal;

						if (tempVal > doubleAtt->Upper())
						{
							doubleAtt->setUpper(tempVal);
						}
						if (tempVal < doubleAtt->Lower())
						{
							doubleAtt->setLower(tempVal);
						}
						currRow++;
					}

					doubleAtt->setValList(doublevals);
					this->_doubleData.push_back(doubleAtt);
					//end = clock();
					//cout<<"Time to retrieve & bind DOUBLE Data  : "<<(end  - start)<<endl;
				}

				else if ((columnType == SQL_CHAR) || (columnType == SQL_VARCHAR) || (columnType == SQL_LONGVARCHAR) || (columnType < 0))
				{

					//start = clock();
					PureStringAttInfo *stringAtt = new PureStringAttInfo();
					stringAtt->type = Type.MULTI_CAT;
					stringAtt->attID = it;
					stringAtt->attName = pCur->getColumn(it)->szName;

					vector<string> vals;
					//TempStringObjects* tempVals = new TempStringObjects[this->_rowCount];
					vals.resize(this->_rowCount);
					int currRow = 0;
					for (BRC = pCur->first() ; (BRC && currRow < this->_rowCount) ; BRC=pCur->next())
					{
						string tempStr = pCur->getChar(it);
						if (pCur->isNull(it) || (strcmp(tempStr.c_str(),"?") == 0))
						{
							//PureAttInfo::existanceBitSet[currRow] = false;
							vals[currRow] = "?";
							currRow++;
							continue;
						}
						PureAttInfo::existanceBitSet[currRow] = true;
						vals[currRow] = tempStr;
						//TempStringObjects strObj(vals[currRow],currRow);
						//tempVals[currRow] = strObj;
						currRow++;
					}

					//stringAtt->setValObjects(tempVals);
					stringAtt->setValList(vals);
					this->_stringData.push_back(stringAtt);

					//end = clock();
					//cout<<"Time to retrieve & bind STRING Data  : "<<(end  - start)<<endl;
				}


				else if ((columnType == SQL_DATE) || (columnType == SQL_DATETIME) || (columnType == SQL_TYPE_DATE) || (columnType == SQL_TIMESTAMP))
				{

					//start = clock();
					PureIntAttInfo *intAtt = new PureIntAttInfo();
					intAtt->type=Type.TYPE_DATE;
					intAtt->attID = it;
					intAtt->attName = pCur->getColumn(it)->szName;

					//long int* values = new long int[rowCount];
					vector<long int> intVals;
					intVals.resize(this->_rowCount);
					int currRow = 0;

					intAtt->setLower((int)(pow(2.0,31.0) - 1));
					intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);

					for (BRC=pCur->first() ; (BRC && currRow) ; BRC=pCur->next())
					{
						if (pCur->isNull(it))
						{
							//PureAttInfo::existanceBitSet[currRow] = false;
							intVals[currRow] = PureAttInfo::NULLVAL;
							currRow++;
							continue;
						}
						PureAttInfo::existanceBitSet[currRow] = true;
						const CGOdbcStmt::DATE* date = pCur->getDate(it);
						int year = date->iYear;
						int month = date->iMonth;
						int day = date->iDay;

						int temp_date_1 = (year * 100) + month;
						int temp_date_2 = (temp_date_1 * 100) + day;
						intVals[currRow] = temp_date_2;

						if (temp_date_2 > intAtt->Upper())
						{
							intAtt->setUpper(temp_date_2);
						}
						if (temp_date_2 < intAtt->Lower())
						{
							intAtt->setLower(temp_date_2);
						}
						currRow++;
					}

					intAtt->setValList(intVals);
					this->_intData.push_back(intAtt);

					//end = clock();
					//cout<<"Time to retrieve & bind DATE Data  : "<<(end  - start)<<endl;
				}
				else
				{
					error_db_unhandled_datatype ex;
					ex << error_message(ExceptionReader::GetError(SM1013));
					ex << error_code(SM1013);
					BOOST_THROW_EXCEPTION(ex);
				}

			}
		}
		catch(...)
		{
			error_binding_pure_data ex;
			ex << error_message(ExceptionReader::GetError(SM1014));
			ex << error_code(SM1014);
			BOOST_THROW_EXCEPTION(ex);
		}
		end = clock();
		cout<<"Total time to bind data : "<<(end - start)<<endl;
		return true;
	}


	CGOdbcStmt* DBQueryExecutionInfo::DBQueryExecution::DBStatementPtr(){
		return this->_stmtPtr;
	}

	DBQueryExecutionInfo::DBQueryExecution::~DBQueryExecution(){
		Commons::DeleteVector(this->_intData.begin(),this->_intData.end());
		Commons::DeleteVector(this->_doubleData.begin(),this->_doubleData.end());
		Commons::DeleteVector(this->_stringData.begin(),this->_stringData.end());
		cout<<"Destructor calls"<<endl;
	}

	std::vector<PureIntAttInfo*> DBQueryExecution::RetievedIntData(){
		return this->_intData;
	}

	std::vector<PureDoubleAttInfo*> DBQueryExecution::RetrievedDoubleData(){
		return this->_doubleData;
	}

	std::vector<PureStringAttInfo*> DBQueryExecution::RetrievedStringData(){
		return this->_stringData;
	}

	int DBQueryExecution::RowCount(){
		return this->_rowCount;
	}

	void DBQueryExecution::setRowCount(){
		try
		{
			CGOdbcStmt *pCur = this->_stmtPtr;
			pCur->last();
			this->_rowCount = pCur->getRowNo();
		}
		catch(...)
		{
			error_object_null ex;
			ex << error_message(ExceptionReader::GetError(SM1011));
			ex << error_code(SM1011);
			BOOST_THROW_EXCEPTION(ex);
		}
	}
}