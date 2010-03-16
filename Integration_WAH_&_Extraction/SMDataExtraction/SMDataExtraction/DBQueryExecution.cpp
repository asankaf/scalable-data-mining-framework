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
using namespace std;


namespace DBQueryExecutionInfo{
	DBQueryExecutionInfo::DBQueryExecution::DBQueryExecution(const char* statement){
		this->_query_stmt = statement;
		_isRowCountSet = false;
	}

	DBQueryExecutionInfo::DBQueryExecution::DBQueryExecution(){ _isRowCountSet = false;}

	bool DBQueryExecutionInfo::DBQueryExecution::ExecuteQueryAndBindData(CGOdbcConnect &cCon){
		clock_t start,end;
		try{
			try{
				this->_stmtPtr = cCon.createStatement();
				start = clock();
				this->_stmtPtr->execute(this->_query_stmt);
				this->_stmtPtr->bindAuto();
				end = clock();
				cout<<"Time to execute and bind data : "<<(end - start)<<endl;
			}
			catch(CGOdbcEx *e){
				cerr<<"Error in executing the query Specified : Possible error in query parsing"<<endl;
				exit(2);
			}

			CGOdbcStmt *pCur= this->_stmtPtr;
			bool BRC;
			int colCount = pCur->getColCount();

			for (int it=0 ; it < colCount ; it++)
			{				
				BRC=false;
				int columnType=pCur->getColumn(it)->iSqlType;
				
				start = clock();
				if ((columnType == SQL_BIT) || (columnType == SQL_TINYINT) || (columnType == SQL_SMALLINT) || (columnType == SQL_INTEGER))
				{
					PureIntAttInfo *intAtt = new PureIntAttInfo();

					intAtt->type = Type.SIGNED_INT;
					intAtt->attName = pCur->getColumn(it)->szName;
					intAtt->attID = it;

					intAtt->setLower((int)(pow(2.0,31.0) - 1));
					intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);
					//long int *values=new long int[this->_rowCount];
					vector<long int> intValList;
					for (BRC = pCur->first(); BRC ; BRC = pCur->next())
					{
						long int tempVal = pCur->getInt(it);
						intValList.push_back(tempVal);
						//values[temp] = pCur->getInt(it);
						if (tempVal > intAtt->Upper())
						{
							intAtt->setUpper(tempVal);
						}
						if (tempVal < intAtt->Lower())
						{
							intAtt->setLower(tempVal);
						}

					}
					if (!_isRowCountSet)
					{
						this->_rowCount = intValList.size();
						_isRowCountSet = true;
					}
					//intAtt->setValueList(values);
					intAtt->setValList(intValList);
					this->_intData.push_back(intAtt);
					end = clock();
					cout<<"Time to Retrieve & bind INT Data : "<< (end - start) <<endl;
				}
				
				else if ((columnType == SQL_DOUBLE) || (columnType == SQL_FLOAT) || (columnType == SQL_NUMERIC) || (columnType == SQL_REAL) || (columnType == SQL_DECIMAL))
				{
					start = clock();
					PureDoubleAttInfo *doubleAtt=new PureDoubleAttInfo();

					doubleAtt->type = Type.SIGNED_INT;
					doubleAtt->attName = pCur->getColumn(it)->szName;
					doubleAtt->attID = it;

					doubleAtt->setLower(pow(2.0,63.0) - 1);
					doubleAtt->setUpper((pow(2.0,63.0) - 1) * -1);

					//double *values = new double[rowCount];
					vector<double> doublevals;
					for (BRC = pCur->first() ; BRC ; BRC = pCur->next())
					{
						double tempVal = pCur->getNumber(it);
						doublevals.push_back(tempVal);
						//values[temp] = pCur->getNumber(it);

						if (tempVal > doubleAtt->Upper())
						{
							doubleAtt->setUpper(tempVal);
						}
						if (tempVal < doubleAtt->Lower())
						{
							doubleAtt->setLower(tempVal);
						}
					}

					if (!_isRowCountSet)
					{
						this->_rowCount = doublevals.size();
						_isRowCountSet = true;
					}
					//doubleAtt->setValueList(values);
					doubleAtt->setValList(doublevals);
					this->_doubleData.push_back(doubleAtt);
					end = clock();
					cout<<"Time to retrieve & bind DOUBLE Data  : "<<(end  - start)<<endl;
				}

				else if ((columnType == SQL_CHAR) || (columnType == SQL_VARCHAR) || (columnType == SQL_LONGVARCHAR) || (columnType < 0))
				{
					start = clock();
					PureStringAttInfo *stringAtt = new PureStringAttInfo();
					stringAtt->type = Type.MULTI_CAT;
					stringAtt->attID = it;
					stringAtt->attName = pCur->getColumn(it)->szName;

					vector<string> vals;

					for (BRC = pCur->first() ; BRC ; BRC=pCur->next())
					{
						vals.push_back(pCur->getChar(it));
					}

					if(!_isRowCountSet){
						this->_rowCount = vals.size();
						this->_isRowCountSet = true;
					}

					stringAtt->setValList(vals);
					this->_stringData.push_back(stringAtt);
					
					end = clock();
					cout<<"Time to retrieve & bind STRING Data  : "<<(end  - start)<<endl;
				}


				else if ((columnType == SQL_DATE) || (columnType == SQL_DATETIME) || (columnType == SQL_TYPE_DATE) || (columnType == SQL_TIMESTAMP))
				{
					start = clock();
					PureIntAttInfo *intAtt = new PureIntAttInfo();
					intAtt->type=Type.TYPE_DATE;
					intAtt->attID = it;
					intAtt->attName = pCur->getColumn(it)->szName;

					//long int* values = new long int[rowCount];
					vector<long int> intVals;

					intAtt->setLower((int)(pow(2.0,31.0) - 1));
					intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);

					for (BRC=pCur->first() ; BRC ; BRC=pCur->next())
					{
						const CGOdbcStmt::DATE* date = pCur->getDate(it);
						int year = date->iYear;
						int month = date->iMonth;
						int day = date->iDay;

						int temp_date_1 = (year * 100) + month;
						int temp_date_2 = (temp_date_1 * 100) + day;
						intVals.push_back(temp_date_2);

						if (temp_date_2 > intAtt->Upper())
						{
							intAtt->setUpper(temp_date_2);
						}
						if (temp_date_2 < intAtt->Lower())
						{
							intAtt->setLower(temp_date_2);
						}

					}

					if (!_isRowCountSet)
					{ 
						this->_rowCount = intVals.size();
						_isRowCountSet = true;
					}
					intAtt->setValList(intVals);
					this->_intData.push_back(intAtt);

					end = clock();
					cout<<"Time to retrieve & bind DATE Data  : "<<(end  - start)<<endl;
				}

			}

		}
		catch(std::exception &ex){
			//TODO add logging details here.
		}

		return true;
	}

	CGOdbcStmt* DBQueryExecutionInfo::DBQueryExecution::DBStatementPtr(){
		return this->_stmtPtr;
	}

	DBQueryExecutionInfo::DBQueryExecution::~DBQueryExecution(){
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
}