#pragma once
#include <boost/exception/all.hpp>
#include <exception> 
#include <string> 
#include <iostream> 

using namespace boost;
using namespace std;

/************************************************************************/
/*    Class  :SEEDMinerExceptions.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:18.04.2010 18:48:51
/*    Author :SEEDMiner
/*    Subj   :Exception class for the SEEDMiner framework
/*    Version: 
/************************************************************************/

/** Structure used to register an exception message in an exception*/
typedef boost::error_info<struct tag_error_message,std::string> error_message;

/** Structure used to register an exception code in an exception*/
typedef boost::error_info<struct tag_error_code,std::string> error_code;

/*///////////////////////////////////////////////////////////////////////
class: base_exception
Description: Base Class of All SEEDMiner Exception classes
///////////////////////////////////////////////////////////////////////*/
struct base_exception : virtual std::exception, virtual boost::exception{};

/** Base Exception class for the SMDataExtraction*/
struct sm_extraction_exception : virtual base_exception {};

/**	DBConnection Exception*/
struct error_db_connection : virtual sm_extraction_exception {};

/**	DBQueryParsing Exception*/
struct error_db_query_execution : virtual sm_extraction_exception {};

/** DBUnhandled Data type Exception*/
struct error_db_unhandled_datatype : virtual sm_extraction_exception {};

/** Error Binding Raw Data Exception*/
struct error_binding_pure_data : virtual sm_extraction_exception {};

struct error_odbc_reg_access : virtual sm_extraction_exception {};

struct error_csv_file_reader : virtual sm_extraction_exception {};

struct error_pure_data_conversion : virtual sm_extraction_exception {};

struct error_vector_out_of_range : virtual sm_extraction_exception {};

struct error_encoding_int_atts : virtual sm_extraction_exception {};

struct error_encoding_string_atts : virtual sm_extraction_exception {};

struct error_encoding_double_atts : virtual sm_extraction_exception {};

struct error_object_null : virtual sm_extraction_exception {};

struct error_folder_not_exist : virtual sm_extraction_exception {};

struct error_saving_encoded_data : virtual sm_extraction_exception {};

struct error_loading_encoded_data : virtual sm_extraction_exception {};