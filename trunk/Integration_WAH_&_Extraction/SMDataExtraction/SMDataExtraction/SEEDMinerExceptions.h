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
class base_exception : public std::exception
{
public: 
	base_exception(std::string err_msg) 
		: what_(err_msg) 
	{ 
	} 

	virtual const char *what() const throw() 
	{ 
		return what_.c_str(); 
	} 

private: 
	std::string what_; 
};

class error_db_connection : public base_exception{
public:
	error_db_connection(string db_error) : base_exception(db_error){}
};