// FlexCtrl.cpp: implementation of the CFlexCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlexCtrl.h"
#include "FlexObject.h"
#include "ShockwaveFlash.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::map<int,CFlexCallBack*> CFlexCtrl::objCallBack;
int CFlexCtrl::flashCount = 0;

CFlexCtrl::CFlexCtrl(int _flashIndex,CShockwaveFlash* Flash, CFlexCallBack *callBack)
{
	flashId = flashCount;
	flashIndex = _flashIndex;
	++flashCount;
	objCallBack[flashId] = callBack;
	m_Flash = Flash;
}

CFlexCtrl::~CFlexCtrl()
{

}

CASWork* CFlexCtrl::Clone()
{
	return (CASWork*) (new CFlexCtrl(flashIndex,m_Flash,objCallBack[flashId]));
}

void CFlexCtrl::EventCallBack(CASObject& obj, CFlexBridge* pBridge){
	CASObject oRootObj;
    pBridge->Root(oRootObj);

	CFlexEvent *evt = new CFlexEvent();
	evt->itemId     = CFlexCtrl::GetStringValue(obj.GetObjectMap()["itemId"]);
	evt->itemType   = CFlexCtrl::GetStringValue(obj.GetObjectMap()["itemType"]);
	evt->eventType  = CFlexCtrl::GetStringValue(obj.GetObjectMap()["eventType"]);
	evt->flashIndex = CFlexCtrl::GetIntegerValue(obj.GetObjectMap()["flashIndex"]);
	int flashID      = CFlexCtrl::GetIntegerValue(obj.GetObjectMap()["flashId"]);

	evt->procedure      = CFlexCtrl::GetStringValue(obj.GetObjectMap()["procedure"]);
	evt->procedurePara      = CFlexCtrl::GetStringValue(obj.GetObjectMap()["procedurePara"]);

	CFlexObject fobj(CFlexCtrl::objCallBack[flashID],evt->flashIndex);

	bool called = false;
	// Call default callback.

	
	// Calls for Button
	if(evt->itemType == "Button")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonActivate(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonClick(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonDeactivate(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonDoubleClick(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonKeyFocusChange(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonTabIndexChange(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonFocusOut(evt,evt->itemId);
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexButtonFocusIn(evt,evt->itemId);
			called =true;
		}
	}

	// Calls for Check Box
	if(evt->itemType == "CheckBox")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxActivate(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxChange(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxClick(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxDeactivate(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxDoubleClick(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxKeyFocusChange(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxTabIndexChange(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxFocusOut(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckBoxFocusIn(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
	}

	// Calls for Radio Button
	if(evt->itemType == "RadioButton")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonActivate(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonChange(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonClick(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonDeactivate(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonDoubleClick(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonKeyFocusChange(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonTabIndexChange(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonFocusOut(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexRadioButtonFocusIn(evt,evt->itemId,fobj.GetPropertyBoolean(evt->itemId,"selected"));
			called =true;
		}
	}

	// Calls for Numeric Stepper.
	if(evt->itemType == "NumericStepper")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperActivate(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperChange(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperClick(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "dataChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperDataChange(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperDeactivate(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperDoubleClick(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperKeyFocusChange(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperTabIndexChange(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperFocusIn(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexNumericStepperFocusOut(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId,"value"));
			called =true;
		}
	}

	// Calls for Text Input
	if(evt->itemType == "TextInput" || evt->itemType == "TextArea")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputActivate(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputChange(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputClick(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "dataChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputDataChange(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputDeactivate(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputDoubleClick(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
		called =true;
		}
		if(evt->eventType == "enter"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputEnter(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputKeyFocusChange(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputTabIndexChange(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputFocusIn(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexTextInputFocusOut(evt,evt->itemId, fobj.GetPropertyString(evt->itemId, "text"));
			called =true;
		}
	}

	// Calls for Sliders
	if(evt->itemType == "VSlider" || evt->itemType == "HSlider" )
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderActivate(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderDeactivate(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderClick(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderChange(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderChange(evt,evt->itemId,fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderKeyFocusChange(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderTabIndexChange(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderFocusIn(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexSliderFocusOut(evt,evt->itemId, fobj.GetPropertyDouble(evt->itemId, "value"));
			called =true;
		}
	}
	
	// Calls for Combo Box.
	if(evt->itemType == "ComboBox")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxActivate(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxDeactivate(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "enter"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxEnter(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxDoubleClick(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "dataChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxDataChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxKeyFocusChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxTabIndexChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxChange(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxFocusIn(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexComboBoxFocusOut(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
	}
	
	// Calls for List
	if(evt->itemType == "List" || evt->itemType == "HorizontalList")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListActivate(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListDeactivate(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "itemClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListItemClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "itemDoubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListItemDoubleClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListDoubleClick(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "dataChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListDataChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListKeyFocusChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListTabIndexChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListChange(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListFocusIn(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexListFocusOut(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
	}
	
	// Calls for Data Grid
	if(evt->itemType == "DataGrid")
	{
		if(evt->eventType == "activate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridActivate(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "deactivate"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridDeactivate(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "itemClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridItemClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "itemDoubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridItemDoubleClick(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "doubleClick"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridDoubleClick(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "dataChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridDataChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "keyFocusChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridKeyFocusChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "tabIndexChange"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridTabIndexChange(evt,evt->itemId, fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "change"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridChange(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "focusIn"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridFocusIn(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
		if(evt->eventType == "focusOut"){
			CFlexCtrl::objCallBack[flashID]->OnFlexDataGridFocusOut(evt,evt->itemId,fobj.GetPropertyInteger(evt->itemId, "selectedIndex"));
			called =true;
		}
	}

	if(evt->itemType == "CartesianChart")
	{
		CFlexCtrl::objCallBack[flashID]->OnFlexCartesianChartItemClick(evt, evt->itemId, CFlexCtrl::GetStringValue(obj.GetObjectMap()["seriesName"]), CFlexCtrl::GetDoubleValue(obj.GetObjectMap()["xField"]), CFlexCtrl::GetDoubleValue(obj.GetObjectMap()["yField"]));
	}
	if(evt->itemType == "PieChart")
	{
		CFlexCtrl::objCallBack[flashID]->OnFlexPieChartItemClick(evt,evt->itemId,CFlexCtrl::GetStringValue(obj.GetObjectMap()["seriesName"]),CFlexCtrl::GetStringValue(obj.GetObjectMap()["field"]), CFlexCtrl::GetDoubleValue(obj.GetObjectMap()["value"]));
		called =true;
	}
	
	// Calls for Alert
	if(evt->itemType == "Alert")
	{
		if(evt->eventType == "alertRespond"){
			CFlexCtrl::objCallBack[flashID]->OnFlexAlertRespond(evt,CFlexCtrl::GetStringValue(obj.GetObjectMap()["respond"]), CFlexCtrl::GetIntegerValue(obj.GetObjectMap()["__alertId"]));
			called =true;
		}
	}

	// Calls for Menu
	if(evt->itemType == "Menu")
	{
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexMenuClick(evt,CFlexCtrl::GetStringValue(obj.GetObjectMap()["clickedItem"]));
			called =true;
		}
	}
	if(evt->itemType == "BoardSquare")
	{
		if(evt->eventType == "click"){
			CFlexCtrl::objCallBack[flashID]->OnFlexCheckerSquareClick(evt, CFlexCtrl::GetIntegerValue(obj.GetObjectMap()["clickX"]), CFlexCtrl::GetIntegerValue(obj.GetObjectMap()["clickY"]));
			called =true;
		}
	}	
	if(!called){
			CFlexCtrl::objCallBack[flashID]->OnFlexEvent(evt);
	}
	delete evt;

}

void CFlexCtrl::Worker()
{
    CFlexBridge* pBridge = (CFlexBridge*)m_pBridge;    
    CASObject oRootObj;
    pBridge->Root(oRootObj);

	objCallBack[flashId]->root[flashIndex] = oRootObj;
    if ( oRootObj.GetInstance() )
    {
		
        try
        {		
            oRootObj.Call("__setCallBackFunction", flashId,flashIndex, EventCallBack);
			oRootObj.Call("__showProgress", "initializing");
			oRootObj.Call("__autoAssignEvents");
			objCallBack[flashId]->OnFlexInit(flashIndex);
			oRootObj.Call("__hideProgress");
        }
        catch ( CBridgeException& oErrBridge )
        {
            LOG("Bridge Exception Caught: \"" << oErrBridge.what() << "\"");            
        }
    }
    
}

CString CFlexCtrl::GetStringValue(CASObject obj)
{
	CASValue cv;
	obj.GetValue(cv);
	if (obj.GetType() == ASOBJECT_STRING)
	{
		return cv.m_sVal->c_str();
	}
	else if(obj.GetType() == ASOBJECT_INT)
	{
		int iVal = GetIntegerValue(obj);
		CString csRet;
		csRet.Format("%d",iVal);
		return csRet;
	}
	else if(obj.GetType() == ASOBJECT_NUMBER)
	{
		int iVal = GetDoubleValue(obj);
		CString csRet;
		csRet.Format("%f",iVal);
		return csRet;
	}
	else if(obj.GetType() == ASOBJECT_BOOLEAN)
	{
		int bVal = GetBooleanValue(obj);
		CString csRet;
		if(bVal)
			csRet = "true";
		else
			csRet = "false";
		return csRet;
	}
	else
	{
		return "";
	}
	
}

int CFlexCtrl::GetIntegerValue(CASObject obj)
{
	CASValue cv;
	obj.GetValue(cv);
	if (obj.GetType() == ASOBJECT_INT)
	{
		return (*(cv.m_iVal));
	} 
	else if(obj.GetType() == ASOBJECT_NUMBER)
	{
		return (int)(*(cv.m_nVal));
	}else{
		return -1;
	}
}

double CFlexCtrl::GetDoubleValue(CASObject obj)
{
	CASValue cv;
	obj.GetValue(cv);
	if(obj.GetType() == ASOBJECT_NUMBER)
	{
		return (*(cv.m_nVal));
	}else{
		return -1;
	}
}

bool CFlexCtrl::GetBooleanValue(CASObject obj)
{
	CASValue cv;
	obj.GetValue(cv);
	if(obj.GetType() == ASOBJECT_BOOLEAN)
	{
		return (*(cv.m_bVal));
	}else{
		return false;
	}
}

std::vector<CASObject> CFlexCtrl::GetArrayValue(CASObject obj)
{
	CASValue cv;
	obj.GetValue(cv);
	if(obj.GetType() == ASOBJECT_ARRAY)
	{
		return (*(cv.m_aVal));
	}else{
		return (std::vector<CASObject>)NULL;
	}
}


LRESULT CFlexCtrl::OnFlexCallback(WPARAM wParam, LPARAM lParam)
{
    std::string* msg = (std::string*)wParam;
    if ( msg == NULL )
    {
        LOG("FlexCallback got a msg with NULL wparam");
        return -1;
    }
    LOGDEBUG("FlexCallback got " << msg->c_str());
	
    CShockwaveFlash* pShockwave = NULL;
	
    int iFlashID = (int)lParam;
	
    pShockwave = (CShockwaveFlash*)CFlashIDManager::GetFlashPtr(iFlashID);
	
    if ( pShockwave != NULL )
	{		
		try
		{
			/* make response to async call back into flex */
			pShockwave->CallFunction(msg->c_str());
	    }
		catch(...)
	    {
		    LOG("Shockwave.CallFunction threw exception!");
		}
	}
    delete msg;
    return 0;
}

LRESULT CFlexCtrl::OnFlexCallbackRet(WPARAM wParam, LPARAM lParam)
{
    std::string* msg = (std::string*)wParam;
    if ( msg == NULL )
    {
        LOG("FlexCallback got a msg with NULL wparam");
        return -1;
    }
    LOGDEBUG("FlexCallback got " << msg->c_str());
	
    CShockwaveFlash* pShockwave = NULL;
	
    CASResponse* pASResp = (CASResponse*)lParam;
    
	
    pShockwave = (CShockwaveFlash*)CFlashIDManager::GetFlashPtr(pASResp->m_iFlashID);
	
    if ( pShockwave != NULL )
	{
		try
		{
			std::string resFn = "<invoke name=\"dude\"><arguments>"+pShockwave->CallFunction(msg->c_str()) + "</arguments></invoke>";
			CFlexCallQueue::AddResponse(pASResp->m_uiID, resFn);
	    }
		catch(...)
		{
			LOG("m_Shockwave.CallFunction threw exception!");
	    }
	}
    delete msg;
    return 0;
}

void CFlexCtrl::OnFlashCallShockwaveflash(LPCTSTR request, CShockwaveFlash* m_Flash) 
{
	int iFlashID = CFlashIDManager::GetIDFlashPtr((void*)m_Flash);    
	CFlexCtrl::FlashCallHandler(iFlashID, request);	
}

void CFlexCtrl::FlashCallHandler(int iFlashID, LPCTSTR request)
{
    CASRequest oRequest;
    oRequest.m_iFlashID = iFlashID;
    oRequest.m_sRequest = request;
    LOGDEBUG("Got request " << oRequest.m_sRequest );
    CFlexCallQueue::AddCall(oRequest);
}

int CFlexCtrl::RegisterDefaultCallbacks(CShockwaveFlash* Flash,CFlexCallBack* callBack)
{
	static int i_fid = 0;
	++i_fid;
	CBridgeManager::RegisterDefaultCallbacks(new CFlexCtrl(i_fid,Flash,callBack),CFlashIDManager::AddFlashPtr((void *)Flash));
	return i_fid;
}

CString CFlexCtrl::GetAbsolutePath(CString relativePath)
{
	relativePath.Replace("/","\\");
	CString returnPath ="";
	char buff[500];
	GetModuleFileName(NULL, buff, 500);
	CString path = buff;
	int count = 1;
	while(relativePath.Find("..\\") == 0)
	{
		++count;
		relativePath = relativePath.Right(relativePath.GetLength() - 3);
	}
	for(int i = 0; i < count; i++)
	{
		path = path.Left(path.ReverseFind('\\'));
		
	}
	returnPath.Format("%s\\%s",path,relativePath);
	return returnPath;
}