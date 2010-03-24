// FlexCtrl.h: interface for the CFlexCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEXCTRL_H__F0B36538_0C68_4DD2_85F5_576F17BD14E4__INCLUDED_)
#define AFX_FLEXCTRL_H__F0B36538_0C68_4DD2_85F5_576F17BD14E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable : 4786)
#include "ASWork.h"
#include "FlexCallBack.h"
#include "FlexObject.h"
#include "shockwaveflash.h"
#endif // _MSC_VER > 1000
/*
 * This is the Flex controlling class.
 * All Callbacks from Flex are come here.
 * Many support functions has implemented here.
 */	
class CFlexCtrl: public CASWork 
{

public:
   /*
	* Return the absolute path of a given relative path of some file from
	* the path of the application is running.
	*/
	static CString GetAbsolutePath(CString relativePath);

   /*
    * Return String value of a CASObject.
	*/
	static CString GetStringValue(CASObject obj);

   /*
    * Return Integer value of a CASObject.
	*/
	static int	   GetIntegerValue(CASObject obj);

   /*
    * Return Double value of a CASObject.
	*/
	static double  GetDoubleValue(CASObject obj);

   /*
    * Return Boolean value of a CASObject.
	*/
	static bool    GetBooleanValue(CASObject obj);

   /*
    * Return Array value of a CASObject.
	* This return std::vector<CASObject> object.
	*/
	static std::vector<CASObject> GetArrayValue(CASObject obj);

   /*
    * All event callbacks are come here from flex.
	* This is the router.
    */
	static void    EventCallBack(CASObject& obj, CFlexBridge* pBridge);

	static LRESULT OnFlexCallback(WPARAM wParam, LPARAM lParam);
	static LRESULT OnFlexCallbackRet(WPARAM wParam, LPARAM lParam);
	static void    OnFlashCallShockwaveflash(LPCTSTR request, CShockwaveFlash* m_Flash) ;
	static void    FlashCallHandler(int iFlashID, LPCTSTR request);
	static int     RegisterDefaultCallbacks(CShockwaveFlash* m_Flash,CFlexCallBack* callBack);

	CFlexCtrl(int _flashIndex,CShockwaveFlash* Flash, CFlexCallBack *callBack);

	virtual ~CFlexCtrl();
	virtual CASWork* Clone();

	static std::map<int,CFlexCallBack*> objCallBack;
	static int flashCount;

protected:
	virtual void Worker();
	int flashId;
	int flashIndex;
	CShockwaveFlash* m_Flash;
};

#endif // !defined(AFX_FLEXCTRL_H__F0B36538_0C68_4DD2_85F5_576F17BD14E4__INCLUDED_)
