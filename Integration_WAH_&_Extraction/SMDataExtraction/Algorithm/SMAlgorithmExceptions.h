#pragma  once
#include "SEEDMinerExceptions.h"
#include "boost/exception.hpp"
#include "ExceptionReader.h"
#include "boost/lexical_cast.hpp"
#include "ExceptionCodes.h"
struct algorithm_exception : virtual base_exception
{

	algorithm_exception()
	{

	}


	algorithm_exception(string _exception_code)
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

struct incompatible_data_type_exception : virtual algorithm_exception
{

	incompatible_data_type_exception(string _exception_code): algorithm_exception(_exception_code)
	{

	}
};

struct empty_data_source_exception:virtual algorithm_exception
{
	empty_data_source_exception(string _exception_code):algorithm_exception(_exception_code)
	{

	}
};

struct invalid_parameter_exception : virtual algorithm_exception
{
	invalid_parameter_exception(string _exception_code) : algorithm_exception(_exception_code)
	{

	}
};

struct division_by_zero_exception : virtual algorithm_exception
{
	division_by_zero_exception(string _exception_code) : algorithm_exception(_exception_code)
	{

	}
};

struct index_out_of_range_exception : virtual algorithm_exception
{
	index_out_of_range_exception(string _exception_code) : algorithm_exception(_exception_code)
	{

	}
};

struct classifier_test_source_exception : virtual algorithm_exception
{
	classifier_test_source_exception(string _exception_code) : algorithm_exception(_exception_code)
	{

	}
};

struct classifier_test_exception : virtual algorithm_exception
{
	classifier_test_exception(string _exception_code) : algorithm_exception(_exception_code)
	{

	}
};