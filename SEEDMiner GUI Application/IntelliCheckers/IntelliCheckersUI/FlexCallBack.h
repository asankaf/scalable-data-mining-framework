// FlexCallBack.h: interface for the CFlexCallBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEXCALLBACK_H__885EE019_6B30_4575_B8E3_FACA658B4E1D__INCLUDED_)
#define AFX_FLEXCALLBACK_H__885EE019_6B30_4575_B8E3_FACA658B4E1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable : 4786)
#include "FlexEvent.h"
//#include "FlexCtrl.h"
#endif // _MSC_VER > 1000

class CFlexCallBack  
{
public:
	CFlexCallBack();

	virtual ~CFlexCallBack();

	//Callback Methods
	virtual void OnFlexEvent(CFlexEvent *evt){};
	virtual void OnFlexInit(int flashIndex){};
	
	//Text Input controller
	virtual void OnFlexTextInputActivate(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputChange(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputClick(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputDataChange(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputDeactivate(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputDoubleClick(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputEnter(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputKeyFocusChange(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputTabIndexChange(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputFocusIn(CFlexEvent *evt, CString controller, CString text){};
	virtual void OnFlexTextInputFocusOut(CFlexEvent *evt, CString controller, CString text){};

	//Button controller
	virtual void OnFlexButtonActivate(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonClick(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonDeactivate(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonDoubleClick(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonKeyFocusChange(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonTabIndexChange(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonFocusIn(CFlexEvent *evt, CString controller){};
	virtual void OnFlexButtonFocusOut(CFlexEvent *evt, CString controller){};

	//Check Box controller
	virtual void OnFlexCheckBoxActivate(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxChange(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxClick(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxDeactivate(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxDoubleClick(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxKeyFocusChange(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxTabIndexChange(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxFocusIn(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexCheckBoxFocusOut(CFlexEvent *evt, CString controller, bool val){};

	//RadioButton controller
	virtual void OnFlexRadioButtonActivate(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonChange(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonClick(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonDeactivate(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonDoubleClick(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonKeyFocusChange(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonTabIndexChange(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonFocusIn(CFlexEvent *evt, CString controller, bool val){};
	virtual void OnFlexRadioButtonFocusOut(CFlexEvent *evt, CString controller, bool val){};

	//Numeric Stepper controller
	virtual void OnFlexNumericStepperActivate(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperClick(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperDataChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperDeactivate(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperDoubleClick(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperKeyFocusChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperTabIndexChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperFocusIn(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexNumericStepperFocusOut(CFlexEvent *evt, CString controller, double val){};

	//Slider controller
	virtual void OnFlexSliderActivate(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderClick(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderDoubleClick(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderDeactivate(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderKeyFocusChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderTabIndexChange(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderFocusIn(CFlexEvent *evt, CString controller, double val){};
	virtual void OnFlexSliderFocusOut(CFlexEvent *evt, CString controller, double val){};
	
	//Combo Box controller
	virtual void OnFlexComboBoxActivate(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxDataChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxEnter(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxDoubleClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxDeactivate(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxKeyFocusChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxTabIndexChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxFocusIn(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexComboBoxFocusOut(CFlexEvent *evt, CString controller, int index){};
	
	//List controller
	virtual void OnFlexListActivate(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListDoubleClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListDataChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListItemClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListItemDoubleClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListDeactivate(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListKeyFocusChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListTabIndexChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListFocusIn(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexListFocusOut(CFlexEvent *evt, CString controller, int index){};
	
	//DataGrid controller
	virtual void OnFlexDataGridActivate(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridDoubleClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridDataChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridItemClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridItemDoubleClick(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridDeactivate(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridKeyFocusChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridTabIndexChange(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridFocusIn(CFlexEvent *evt, CString controller, int index){};
	virtual void OnFlexDataGridFocusOut(CFlexEvent *evt, CString controller, int index){};
	
	//charts
	virtual void OnFlexPieChartItemClick(CFlexEvent *evt, CString controller, CString seriesName, CString field, double dVal){};
	virtual void OnFlexCartesianChartItemClick(CFlexEvent *evt, CString controller, CString seriesName, double dXVal, double dYVal){};
	
	//Alert
	virtual void OnFlexAlertRespond(CFlexEvent *evt, CString respond, int alertId){};
	virtual void OnFlexMenuClick(CFlexEvent *evt, CString item){};
	virtual void OnFlexCheckerSquareClick(CFlexEvent *evt, int x, int y){};
	//Root object
	std::map<int,CASObject> root;
private:
	bool initCalled;
};



#endif // !defined(AFX_FLEXCALLBACK_H__885EE019_6B30_4575_B8E3_FACA658B4E1D__INCLUDED_)
