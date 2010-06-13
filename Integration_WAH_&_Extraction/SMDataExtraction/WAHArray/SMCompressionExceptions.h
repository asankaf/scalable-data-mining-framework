#pragma  once
#include "SEEDMinerExceptions.h"
#include "boost/exception.hpp"
#include "ExceptionReader.h"
#include "boost/lexical_cast.hpp"
struct compression_exception : virtual base_exception
{
	
compression_exception()
	{
	
	}


	compression_exception(string _exception_code)
	{
		*this<< error_message(ExceptionReader::GetError(_exception_code));
		*this<<error_code(_exception_code);		
	}

	int getErrorLine()
	{
		return *get_error_info<throw_line>(*this);
	}

	string getErrorFile()
	{
		return string(*get_error_info<throw_file>(*this));
	}

	string getErrorMessage()
	{
		return *get_error_info<error_message>(*this);
	}

	string getErrorCode()
	{
		return *get_error_info<error_code>(*this);
	}

	string toString()
	{
		string except = "Error in file ";
		except += getErrorFile() + " line " + boost::lexical_cast<string>(getErrorLine())+"\n";
		except += getErrorCode()+" : "+getErrorMessage();
		return except;
	}
};

struct invalid_compression_type_exception : virtual compression_exception
{
	invalid_compression_type_exception(string _exception_code):compression_exception(_exception_code)
	{

	}

};

struct null_operand_exception : virtual compression_exception
{
	null_operand_exception(string _exception_code):compression_exception(_exception_code)
	{

	}
};

struct incompatible_operands_exception :virtual compression_exception
{
	incompatible_operands_exception(string _exception_code):compression_exception(_exception_code)
	{

	}
};
