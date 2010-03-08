#include "stdafx.h"
#include "stdhead.h"
#include "SMException.h"
#include <wtypes.h>
#include <winuser.h>
#include <atlconv.h>

HINSTANCE g_hInst;

CGOdbcEx::CGOdbcEx(bool bUsage, long lCode, const char * szMsg)
{
	m_bUsage = bUsage;
	m_lCode = lCode;
	if (bUsage)
	{
		//char szBuff[256] = "";
		LPWSTR szBuff;
		
		LoadString(g_hInst, lCode + 10, (szBuff), 256);
		m_sMsg = szBuff;
	}
	else
		m_sMsg = szMsg;
}

bool CGOdbcEx::isUsage()
{
	return m_bUsage;
}

long CGOdbcEx::getCode()
{
	return m_lCode;
}

const char * CGOdbcEx::getMsg()
{
	return m_sMsg;
}

