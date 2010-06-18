#include "stdafx.h"
#include "CsvConnection.h"
#include "PureStringAttInfo.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include "AttributeType.h"
#include <algorithm>
#include "seedminerexceptions.h"
#include "exceptionreader.h"
#include "ExceptionCodes.h"

using namespace std;

CSVConnectionInfo::CsvConnection::CsvConnection(const char* file_name,const char field_terminator,const char line_terminator,const char enclosure_char):_file_source(file_name),_field_termiator(field_terminator),_line_terminator(line_terminator),_enclosure_char(enclosure_char){
	if(!this->_file_parser.init(this->_file_source))
	{
		error_csv_file_reader ex;
		string fileName(file_name);
		string _what_error = ExceptionReader::GetError(SM1004);
		_what_error += "\nProvided CSV File: " + fileName;
		ex << error_message(_what_error);
		ex << error_code(SM1003);
		BOOST_THROW_EXCEPTION(ex);
	}
	this->_file_parser.set_enclosed_char(this->_enclosure_char,ENCLOSURE_OPTIONAL);
	this->_file_parser.set_field_term_char(this->_field_termiator);
	this->_file_parser.set_line_term_char(this->_line_terminator);
}
CSVConnectionInfo::CsvConnection::CsvConnection(const char* file_name,const char field_terminator,const char line_terminator) :_file_source(file_name),_field_termiator(field_terminator),_line_terminator(line_terminator),_enclosure_char('""'){
	std::fstream *file = new fstream(file_name);
	if (!file_exists(file_name))
	{
		error_csv_file_reader ex;
		string fileName(file_name);
		string _what_error = ExceptionReader::GetError(SM1004);
		_what_error += "\nProvided CSV File: " + fileName;
		ex << error_message(_what_error);
		ex << error_code(SM1003);
		BOOST_THROW_EXCEPTION(ex);
	}
	this->_inputfile = file;
	this->isnamesset = false;
}

CSVConnectionInfo::CsvConnection::~CsvConnection(){

}

ExtractedCsvDTO* CSVConnectionInfo::CsvConnection::extractData(){
	time_t start,end;
	start = clock();
	ExtractedCsvDTO *exCsv = new ExtractedCsvDTO();
	vector<csv_row> rowSets;

	while (this->_file_parser.has_more_rows())
	{
		csv_row row = this->_file_parser.get_row();
		if (row.empty()) break;
		rowSets.push_back(row);
		if (exCsv->AttributeCount() == 0)
		{
			exCsv->AttributeCount(row.size());
		}
	}

	end = clock();
	cout<<"Time to retrieve CSV data : " << (end - start)<<endl;
	exCsv->RowCount(rowSets.size() - 1);
	int attCount = exCsv->AttributeCount();
	int rowCount = exCsv->RowCount();
	PureAttInfo::existanceBitSet.resize(rowCount);
	PureAttInfo::existanceBitSet.flip();

	PureStringAttInfo** stringData = new PureStringAttInfo*[exCsv->AttributeCount()];
	for (int i = 0 ; i < attCount ; i++)
	{		
		PureStringAttInfo* strData = new PureStringAttInfo();
		strData->attID = i;
		strData->attName = rowSets[0][i];

		vector<string> vals;
		vals.resize(rowCount);

		try{
			for (int j = 1 ; j <= rowCount ; j++)
			{
				string val = rowSets[j][i];	
				vals[j - 1] = val;	
				if (strcmp(val.c_str(),"?") == 0)
				{
					PureAttInfo::existanceBitSet[j - 1] = false;
				}
			}
		}
		catch(...){
			error_pure_data_conversion ex;
			ex << error_message(ExceptionReader::GetError(SM1005));
			ex << error_code(SM1005);
			BOOST_THROW_EXCEPTION(ex);
		}

		strData->setValList(vals);
		stringData[i] = strData;
	}

	exCsv->StringData(stringData);

	return exCsv;
}

ExtractedCsvDTO* CSVConnectionInfo::CsvConnection::readCSV(vector<int> readOrder){
	time_t start,end;
	start = clock();
	ExtractedCsvDTO *exCsv = new ExtractedCsvDTO();
	exCsv->Order(readOrder);
	std::vector<std::string> stringArray;
	std::vector<double> doubleArray;
	std::vector<long> longArray;
	long rowCount = 0;
	string field_term;
	field_term = field_term + this->_field_termiator;

	exCsv->AttributeCount(readOrder.size());
	//	declare string buffer
	std::string buffer;
	vector<long> nullRowIDs;
	unsigned long orderSize = readOrder.size();
	//	get line from file
	try
	{
		while(!this->_inputfile->eof()){
			if (!isnamesset)
			{
				//first Line of the Data File is considered as Names of the attributes.
				getline((*this->_inputfile),buffer,this->_line_terminator);
				const char* tempNamesBuffer = buffer.c_str();
				tempNamesBuffer = strtok((char*)tempNamesBuffer,field_term.c_str());
				while (tempNamesBuffer != NULL)
				{
					this->_attNames.push_back(tempNamesBuffer);
					tempNamesBuffer = strtok(NULL,field_term.c_str());
				}
				isnamesset = true;
				delete tempNamesBuffer;
			}

			else
			{

				getline((*this->_inputfile), buffer, this->_line_terminator);
				//	read through buffer searching for commas or end of string. the start of 
				//	each entry is the first element of the string (first word), or the 
				//	position after the last comma encountered (all other words).  the end
				//	of the entry is the last element of the string (last word), or the 
				//	position before the next comma (all other words)
				std::string temp;

				bool nullRow = false;
				unsigned long start=0;
				unsigned long end=0;
				long tempCount = 0;
				const char* tempBuffer = buffer.c_str();
				if (strcmp(tempBuffer,"") == 0) continue;


				tempBuffer = strtok((char*)tempBuffer,field_term.c_str());
				while (tempBuffer != NULL) {

					//	increment end counter
					//end++;


					//	check if position of 'end' in the string a comma, past the end of 
					//	the string, or just another character.  if at a non-comma character,
					//	skip to the start of the next loop
					// 			if (end<buffer.size() && buffer[end]!=',') {
					// 				continue;
					// 			}

					//	assign comma-free token to a temporary string
					//temp.assign(buffer,start,end-start);
					//tempCount++;
					int index = readOrder[(tempCount % orderSize)];
					switch (index)
					{
					case 0:
						{
							longArray.push_back(atoi(tempBuffer));
							if (strcmp(tempBuffer,"?") == 0) nullRow = true;
							break;
						}
					case 1:
						{
							doubleArray.push_back(atof(tempBuffer));
							if (strcmp(tempBuffer,"?") == 0) nullRow = true;
							break;
						}
					case 2:
						{
							stringArray.push_back(tempBuffer);
							if (strcmp(tempBuffer,"?") == 0) nullRow = true;
							break;
						}
					}

					// 				run through each character of the token to determine if it is a 
					// 			//	string, floating-point, or integer, based on number of periods and
					// 			//	presence of non-numeric characters
					// 			int containsPeriod=0;
					// 			int containsAlpha=0;
					// 
					// 			for (long i=0; i<(long)temp.size(); ++i) {
					// 				if (temp[i]=='.') {
					// 					//	token contains a '.' character - either a string or a floating point
					// 					containsPeriod++;
					// 				} else if ( temp[i]<48 || 57<temp[i] ) {
					// 					//	token contains non-numeric characters - string (sorry no hex)
					// 					containsAlpha++;
					// 				}
					// 			}
					// 
					// 			
					// 
					// 			//	based on alphabetical/numerical content and the presence of period/
					// 			//	decimals, determine whether the entry is an integer, floating-point
					// 			//	number, or a string
					// 			if (containsAlpha==0 && containsPeriod==0) {
					// 				longArray.push_back(atoi(temp.c_str()));
					// 				order.push_back(0);
					// 
					// 			} else if (containsPeriod==1 && containsAlpha==0) {
					// 				doubleArray.push_back(atof(temp.c_str()));
					// 				order.push_back(1);
					// 			} else {
					// 				stringArray.push_back(temp);
					// 				order.push_back(2);
					// 			}
					// 

					//	set the start and end to the proper next position
					//end++;
					//start=end;
					tempBuffer = strtok(NULL,field_term.c_str());
					tempCount++;

				}
				if (nullRow)
				{
					nullRowIDs.push_back(rowCount);
					nullRow = false;
				}
				rowCount++;
			}
		}
	}
	catch(...)
	{
		error_csv_file_reader ex;
		ex << error_message(ExceptionReader::GetError(SM1006));
		ex << error_code(ExceptionReader::GetError(SM1006));
		BOOST_THROW_EXCEPTION(ex);
	}
	PureAttInfo::existanceBitSet.resize(rowCount);
	PureAttInfo::existanceBitSet.flip();
	for (int i = 0 ; i < nullRowIDs.size() ; i++)
	{
		PureAttInfo::existanceBitSet[nullRowIDs[i]] = false;
	}
	end = clock();
	cout<<"Time to read the CSV Data : "<<(end - start)<<endl;
	exCsv->RowCount(rowCount);

	try
	{
		//pre-process Int Vector and create PureIntAtts for encoding.
		vector<AttributeProps> intAttProps = getAttributeNames(0,readOrder);
		vector<PureIntAttInfo*> intAtts;
		int int_att_count = (int)(longArray.size() / rowCount);
		int tempIntVec = longArray.size() - int_att_count;
		for (int i = 1 ; i <= int_att_count ; i++)
		{
			PureIntAttInfo *intAtt = new PureIntAttInfo();

			intAtt->type = Type.SIGNED_INT;
			intAtt->attName = intAttProps[i - 1].Name;
			intAtt->attID = intAttProps[i - 1].ID;

			intAtt->setLower((int)(pow(2.0,31.0) - 1));
			intAtt->setUpper((int)((pow(2.0,31.0) - 1)) * -1);
			long int *vals = new long int[rowCount];

			for (int j = (i - 1),k = 0 ; j <= (tempIntVec + i - 1) ; j+= int_att_count)
			{
				long tempVal = longArray[j];
				vals[k++] = tempVal;
				if (tempVal > intAtt->Upper())
				{
					intAtt->setUpper(tempVal);
				}
				if (tempVal < intAtt->Lower())
				{
					intAtt->setLower(tempVal);
				}
			}
			vector<long int> valList(vals,vals + rowCount);	
			intAtt->setValList(valList);
			intAtts.push_back(intAtt);
		}
		exCsv->IntData(intAtts);

		//pre-process Double Vector and create PureDoubleAtts for encoding.
		vector<AttributeProps> doubleAttProps = getAttributeNames(1,readOrder);
		vector<PureDoubleAttInfo*> doubleAtts;
		int double_att_count = (int)(doubleArray.size()/rowCount);
		int tempDoubleVac = doubleArray.size() - double_att_count;

		for (int i = 1 ; i <= double_att_count ; i++)
		{
			PureDoubleAttInfo* doubleAtt = new PureDoubleAttInfo();
			doubleAtt->type = Type.SIGNED_INT;
			doubleAtt->attName = doubleAttProps[i - 1].Name;
			doubleAtt->attID = doubleAttProps[i - 1].ID;

			doubleAtt->setLower(pow(2.0,63.0) - 1);
			doubleAtt->setUpper((pow(2.0,63.0) - 1) * -1);
			double* vals = new double[rowCount];

			for (int j = (i - 1),k = 0 ; j <= (tempDoubleVac + i - 1) ; j+= double_att_count)
			{
				double tempVal = doubleArray[j];
				vals[k++] = tempVal;
				if (tempVal > doubleAtt->Upper())
				{
					doubleAtt->setUpper(tempVal);
				}
				if (tempVal < doubleAtt->Lower())
				{
					doubleAtt->setLower(tempVal);
				}
			}
			vector<double> valList(vals , vals + rowCount);
			doubleAtt->setValList(valList);
			doubleAtts.push_back(doubleAtt);
		}
		exCsv->DoubleData(doubleAtts);

		//pre-process MultiCat Vector and create PureStringAtts for encoding.
		vector<AttributeProps> stringAttProps = getAttributeNames(2,readOrder);
		vector<PureStringAttInfo*> stringAtts;
		int string_att_count = (int)(stringArray.size()/rowCount);
		int tempStringvec = stringArray.size() - string_att_count;

		for (int  i = 1 ; i <= string_att_count ; i++)
		{
			PureStringAttInfo* stringAtt = new PureStringAttInfo();
			stringAtt->type = Type.MULTI_CAT;
			stringAtt->attID = stringAttProps[i-1].ID;
			stringAtt->attName = stringAttProps[i-1].Name;
			string* vals = new string[rowCount];
			for (int j = (i - 1),k = 0 ; j <= (tempStringvec + i - 1) ; j+= string_att_count)
			{
				vals[k++] = stringArray[j];
			}
			vector<string> valList(vals,vals+rowCount);
			stringAtt->setValList(valList);
			stringAtts.push_back(stringAtt);
		}
		exCsv->MultiCatData(stringAtts);
	}
	catch(...)
	{
		error_pure_data_conversion ex;
		ex << error_message(ExceptionReader::GetError(SM1005));
		ex << error_code(SM1005);
		BOOST_THROW_EXCEPTION(ex);
	}
	end = clock();
	cout<<"Total Time to read & process the CSV Data : "<<(end - start)<<endl;
	longArray.clear();
	doubleArray.clear();
	stringArray.clear();
	return exCsv;
}

vector<AttributeProps> CSVConnectionInfo::CsvConnection::getAttributeNames(int attType,vector<int> order){
	vector<AttributeProps> atts;
	
	for (int i = 0 ; i < this->_attNames.size() ; i++)
	{
		if (order[i] == attType)
		{
			AttributeProps att;
			att.ID = i;
			att.Name = this->_attNames[i];
			atts.push_back(att);
		}
	}
	return atts;
}

bool CSVConnectionInfo::CsvConnection::file_exists( const char *fileName )
{
	struct stat info;
	int ret = -1;
	//get the file attributes
	ret = stat(fileName, &info);
	if(ret == 0) 
	{
		//stat() is able to get the file attributes,
		//so the file obviously exists
		return true;
	} 
	else 
	{
		//stat() is not able to get the file attributes,
		//so the file obviously does not exist or
		//more capabilities is required
		return false;
	}
}
