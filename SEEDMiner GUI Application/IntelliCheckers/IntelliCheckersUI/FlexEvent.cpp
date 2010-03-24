// FlexEvent.cpp: implementation of the CFlexEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlexEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlexEvent::CFlexEvent()
{
	itemId ="";
	itemType = "";
	eventType = "";
}

CFlexEvent::~CFlexEvent()
{

}
