#include "stdhead.h"
#include <stdexcept>
#include "boost\exception\exception.hpp"
class CGOdbcEx : virtual std::exception{
public:
	typedef enum
	{
		eTypeIsIncompatible = 1,
		eIndexOutOfRange,
		eFieldNotFound,
		eAllParameterAlreadyBound,
	}UsageCode;

	CGOdbcEx(bool bUsage, long lCode, const char *szMsg = 0);
	bool isUsage();
	long getCode();
	const char *getMsg();
protected:
	bool m_bUsage;
	long m_lCode;
	_bstr_t m_sMsg;

};