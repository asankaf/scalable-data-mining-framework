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
using namespace std;


namespace DBQueryExecutionInfo{
	DBQueryExecutionInfo::DBQueryExecution::DBQueryExecution(const char* statement){
		this->_query_stmt = statement;
	}

	DBQueryExecutionInfo::DBQueryExecution::DBQueryExecution(){}

	bool DBQueryExecutionInfo::DBQueryExecution::ExecuteQueryAndBindData(CGOdbcConnect cCon){
		try{
			try{
			this->_stmtPtr = cCon.createStatement();
			this->_stmtPtr->execute(this->_query_stmt);
			this->_stmtPtr->bindAuto();
			}
			catch(CGOdbcEx *e){
				cerr<<"Error in executing the query Specified : Possible error in query parsing"<<endl;
				exit(2);
			}

			CGOdbcStmt *pCur= this->_stmtPtr;
			bool BRC;

			int rowCount = 0;
			for (BRC = pCur->first(); BRC ; BRC = pCur->next())
			{
				rowCount++;
			}
			this->_rowCount = rowCount;		
			
			for (int it=0 ; it < pCur->getColCount() ; it++)
			{				
				BRC=false;
				int columnType=pCur->getColumn(it)->iSqlType;
				
				if ((columnType == SQL_BIT) || (columnType == SQL_TINYINT) || (columnType == SQL_SMALLINT) || (columnType == SQL_INTEGER))
				{
					PureIntAttInfo *intAtt = new PureIntAttInfo();

					intAtt->type = Type.SIGNED_INT;
					intAtt->attName = pCur->getColumn(it)->szName;
					intAtt->attID = it;
					
					intAtt->setLower((int)(pow(2.0,31.0) - 1));
					intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);
					long int *values=new long int[this->_rowCount];
					int temp = 0 ;
					for (BRC = pCur->first(); BRC ; BRC = pCur->next())
					{
						values[temp] = pCur->getInt(it);
						if (values[temp] > intAtt->Upper())
						{
							intAtt->setUpper(values[temp]);
						}
						if (values[temp] < intAtt->Lower())
						{
							intAtt->setLower(values[temp]);
						}
						temp += 1;
					}
					
					intAtt->setValueList(values);
					this->_intData.push_back(intAtt);
				}

				else if ((columnType == SQL_DOUBLE) || (columnType == SQL_FLOAT) || (columnType == SQL_NUMERIC) || (columnType == SQL_REAL) || (columnType == SQL_DECIMAL))
				{
					PureDoubleAttInfo *doubleAtt=new PureDoubleAttInfo();

					doubleAtt->type = Type.SIGNED_INT;
					doubleAtt->attName = pCur->getColumn(it)->szName;
					doubleAtt->attID = it;

					doubleAtt->setLower(pow(2.0,63.0) - 1);
					doubleAtt->setUpper((pow(2.0,63.0) - 1) * -1);

					double *values = new double[rowCount];
					int temp = 0;
					bool BRC;
					for (BRC = pCur->first() ; BRC ; BRC = pCur->next())
					{
						values[temp] = pCur->getNumber(it);
						
						if (values[temp] > doubleAtt->Upper())
						{
							doubleAtt->setUpper(values[temp]);
						}
						if (values[temp] < doubleAtt->Lower())
						{
							doubleAtt->setLower(values[temp]);
						}
						temp += 1;
					}

					doubleAtt->setValueList(values);
					this->_doubleData.push_back(doubleAtt);
				}

				else if ((columnType == SQL_CHAR) || (columnType == SQL_VARCHAR) || (columnType == SQL_LONGVARCHAR) || (columnType < 0))
				{
					PureStringAttInfo *stringAtt = new PureStringAttInfo();
					stringAtt->type = Type.MULTI_CAT;
					stringAtt->attID = it;
					stringAtt->attName = pCur->getColumn(it)->szName;

					string* values = new string[rowCount];
					int temp = 0;
					bool BRC;
					for (BRC = pCur->first() ; BRC ; BRC=pCur->next())
					{
						values[temp] = pCur->getChar(it);
						temp += 1;
					}
					stringAtt->setValueList(values,rowCount);
					this->_stringData.push_back(stringAtt);
				}
				
				else if ((columnType == SQL_DATE) || (columnType == SQL_DATETIME) || (columnType == SQL_TYPE_DATE) || (columnType == SQL_TIMESTAMP))
				{
					PureIntAttInfo *intAtt = new PureIntAttInfo();
					intAtt->type=Type.TYPE_DATE;
					intAtt->attID = it;

					long int* values = new long int[rowCount];
					int temp = 0;
					bool BRC;

					intAtt->setLower((int)(pow(2.0,31.0) - 1));
					intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);

					for (BRC=pCur->first() ; BRC ; BRC=pCur->next())
					{
						try{
							char buffer[100];							

							char* year = itoa(pCur->getDate(it)->iYear,buffer,10);
							char* yrString = (char*)malloc(strlen(year)+1);
							yrString = strcpy(yrString,year);
							
							char* month = itoa(pCur->getDate(it)->iMonth,buffer,10);
							char* monthString = (char*)malloc(strlen(month) + 1);
							monthString = strcpy(monthString,month); 

							char* paddingMonthString = NULL;
							if (strlen(monthString) < 2)
							{
								char* temp = itoa(0,buffer,10);
								paddingMonthString = (char*)malloc(strlen(temp) + 1);
								paddingMonthString = strcpy(paddingMonthString,temp);
							}
							
							char* day = itoa(pCur->getDate(it)->iDay,buffer,10);
							char* dayString = (char*)malloc(strlen(day)+1);
							dayString = strcpy(dayString,day);

							char* paddingDayString = NULL;
							if (strlen(dayString) < 2)
							{
								char* temp = itoa(0,buffer,10);
								paddingDayString = (char*)malloc(strlen(temp) + 1);
								paddingDayString = strcpy(paddingDayString,temp);
							}

							size_t memSize = 0;
							memSize += strlen(yrString);
							if(paddingMonthString != NULL) {
								memSize += strlen(paddingMonthString);
							}
							memSize += strlen(monthString);
							if(paddingDayString != NULL) {
								memSize += strlen(paddingDayString);
							}
							memSize += strlen(dayString);
							char* dateString = (char*)malloc(memSize + 1);

							dateString = strcpy(dateString,yrString);
							if (paddingMonthString != NULL)
							{
								dateString = strcat(dateString,paddingMonthString);
							}							
							dateString = strcat(dateString,monthString);
							if (paddingDayString != NULL)
							{
								dateString = strcat(dateString,paddingDayString);
							}
							dateString = strcat(dateString,dayString);
							values[temp] = (long int)atol(dateString);

							if (values[temp] > intAtt->Upper())
							{
								intAtt->setUpper(values[temp]);
							}
							if (values[temp] < intAtt->Lower())
							{
								intAtt->setLower(values[temp]);
							}

							temp += 1;
						}

						catch(std::exception &ex){
							//TODO add logging details here.
						}
						
					}
					intAtt->setValueList(values);
					this->_intData.push_back(intAtt);
				}
			}
			
			

			return true;
		}
		catch(std::exception &ex){
			return false;
		}
	}

	CGOdbcStmt* DBQueryExecutionInfo::DBQueryExecution::DBStatementPtr(){
		return this->_stmtPtr;
	}

	DBQueryExecutionInfo::DBQueryExecution::~DBQueryExecution(){
		//cout<<"Destructor calls"<<endl;
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