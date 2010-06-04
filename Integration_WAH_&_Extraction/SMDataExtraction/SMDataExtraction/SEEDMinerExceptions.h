#pragma once
#include <boost/exception/all.hpp>
#include <exception> 
#include <string> 
#include <iostream> 

using namespace boost;
using namespace std;

/*///////////////////////////////////////////////////////////////////////
	class: base_exception
	Description: Base Class of All SEEDMiner Exception classes
///////////////////////////////////////////////////////////////////////*/

typedef boost::error_info<struct tag_error_message,std::string> error_message;
typedef boost::error_info<struct tag_error_code,std::string> error_code;

struct base_exception : virtual std::exception, virtual boost::exception
{

};

struct error_db_connection : virtual base_exception {

};