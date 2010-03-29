// FlexObject.cpp: implementation of the CFlexObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlexObject.h"
#include "FlexCtrl.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString  CFlexObject::FLEX_EVT_CLICK			 = "click";
CString  CFlexObject::FLEX_EVT_ACTIVATE          = "activate";
CString  CFlexObject::FLEX_EVT_DEACTIVATE        = "deactivate";
CString  CFlexObject::FLEX_EVT_DOUBLE_CLICK      = "doubleClick";
CString  CFlexObject::FLEX_EVT_KEY_FOCUS_CHANGE  = "keyFocusChange";
CString  CFlexObject::FLEX_EVT_TAB_INDEX_CHANGE  = "tabIndexChange";
CString  CFlexObject::FLEX_EVT_CHANGE            = "change";
CString  CFlexObject::FLEX_EVT_DATA_CHANGE       = "dataChange";
CString  CFlexObject::FLEX_EVT_ITEM_CLICK        = "itemClick";
CString  CFlexObject::FLEX_EVT_ENTER_KEY_PRESSED = "enter";

int CFlexObject::FLEX_ALERT_OK_ONLY               = 0;
int CFlexObject::FLEX_ALERT_OK_CANCEL             = 1;
int CFlexObject::FLEX_ALERT_YES_NO                = 2;
int CFlexObject::FLEX_ALERT_YES_NO_CANCEL         = 3;

CFlexObject::CFlexObject(CFlexCallBack* flexCallback, int flashIndex)
{
	root = flexCallback->root[flashIndex];
}
CFlexObject::CFlexObject()
{

}
CFlexObject::~CFlexObject()
{

}

CString CFlexObject::convertSet(CString item)
{
	CString cs;
	cs.Format ("set%s", item);
	return cs;
}

CString CFlexObject::convertGet(CString item)
{
	CString cs;
	cs.Format ("get%s", item);
	return cs;
}

CASObject CFlexObject::CallFunction(CString func, CASObject arg1)
{
	return root.Call(func, arg1);
}

CASObject CFlexObject::CallFunction(CString func, CASObject arg1, CASObject arg2)
{
	return root.Call(func, arg1, arg2);
}

CASObject CFlexObject::CallFunction(CString func, CASObject arg1, CASObject arg2, CASObject arg3)
{	
	return root.Call(func, arg1, arg2, arg3);
}

CASObject CFlexObject::CallFunction(CString func, CASObject arg1, CASObject arg2, CASObject arg3, CASObject arg4)
{
	return root.Call(func, arg1, arg2, arg3, arg4);
}

void CFlexObject::AddEventListener(CString itemId, CString eventType)
{
	if (eventType == "doubleClick")
	{
		root.Call(convertGet(itemId)).Call("setdoubleClickEnabled",true);
	}
	CallFunction ("__addListener",(CASObject)itemId, (CASObject)eventType);
}

void CFlexObject::SetProperty(CString itemId, CString prop, CASObject val)
{
	root.Call(convertGet(itemId)).Call(convertSet(prop),val);
}


CASObject CFlexObject::GetProperty(CString itemId, CString prop)
{
	return root.Call(convertGet(itemId)).Call(convertGet(prop));
}

CString CFlexObject::GetPropertyString(CString itemId, CString prop)
{
	return CFlexCtrl::GetStringValue(GetProperty(itemId, prop));
}

int CFlexObject::GetPropertyInteger(CString itemId, CString prop)
{
	return CFlexCtrl::GetIntegerValue(GetProperty(itemId, prop));
}

double CFlexObject::GetPropertyDouble(CString itemId, CString prop)
{
	return CFlexCtrl::GetDoubleValue(GetProperty(itemId, prop));
}

bool CFlexObject::GetPropertyBoolean(CString itemId, CString prop)
{
	return CFlexCtrl::GetBooleanValue(GetProperty(itemId, prop));
}

void CFlexObject::SetFocus(CString itemId)
{
	root.Call(convertGet(itemId)).Call("setFocus");
}

void CFlexObject::SetEnabled(CString itemId, bool status)
{
	root.Call(convertGet(itemId)).Call("setenabled",status);
}

void CFlexObject::SetText(CString itemId, CString text)
{
	SetProperty(itemId,"text",(CASObject)text);
}

void CFlexObject::SetLabel(CString itemId, CString text)
{
	SetProperty(itemId,"label",(CASObject)text);
}

void CFlexObject::SetValue(CString itemId, int val)
{
	SetProperty(itemId,"value",val);
}

void CFlexObject::SetValue(CString itemId, double val)
{
	SetProperty(itemId,"value",val);
}

void CFlexObject::SetSelected(CString itemId, bool val)
{
	SetProperty(itemId,"selected",val);
}

void CFlexObject::SetMaximum(CString itemId, double val)
{
	SetProperty(itemId,"maximum",val);
}

void CFlexObject::SetMinimum(CString itemId, double val)
{
	SetProperty(itemId,"minimum",val);
}

void CFlexObject::SetStepSize(CString itemId, double val)
{
	SetProperty(itemId,"stepSize",val);
}

void CFlexObject::SetVisible(CString itemId, bool val)
{
	SetProperty(itemId,"visible",val);
}

void CFlexObject::SetToolTip(CString itemId, CString text)
{
	SetProperty(itemId,"toolTip",(CASObject)text);
}

CString CFlexObject::GetText(CString itemId)
{
	return GetPropertyString(itemId,"text");
}

CString CFlexObject::GetLabel(CString itemId)
{
	return GetPropertyString(itemId,"label");
}

int CFlexObject::GetValueInteger(CString itemId)
{
	return GetPropertyInteger(itemId,"value");
}

double CFlexObject::GetValueDouble(CString itemId)
{
	return GetPropertyDouble(itemId,"value");
}

bool CFlexObject::GetSelected(CString itemId)
{
	return GetPropertyBoolean(itemId,"selected");
}

double CFlexObject::GetMaximum(CString itemId)
{
	return GetPropertyDouble(itemId,"maximum");
}

double CFlexObject::GetMinimum(CString itemId)
{
	return GetPropertyDouble(itemId,"minimum");
}

double CFlexObject::GetStepSize(CString itemId)
{
	return GetPropertyDouble(itemId,"stepSize");
}

bool CFlexObject::GetVisible(CString itemId)
{
	return GetPropertyBoolean(itemId,"visible");
}

CString CFlexObject::GetToolTip(CString itemId)
{
	return GetPropertyString(itemId,"toolTip");
}

void CFlexObject::AddToDataProvider(CString itemId, std::vector<CASObject> dataArray)
{
    root.Call(convertGet(itemId)).Call("setdataProvider", dataArray);
}

void CFlexObject::AddItem(CString itemId, CASObject Item)
{
	std::vector<CASObject> vct;
	vct.push_back(Item);
	AddItems (itemId, vct);
}

void CFlexObject::AddItems(CString itemId, std::vector<CASObject> Items)
{
	root.Call ("__addToDataProvider",(CASObject)itemId, (CASObject)Items, -1);
}

void CFlexObject::AddItemsAt(CString itemId, std::vector<CASObject> Items,int index)
{
	root.Call ("__addToDataProvider",(CASObject)itemId, (CASObject)Items, index);
}

void CFlexObject::AddItemAt(CString itemId, CASObject Item,int index)
{
	std::vector<CASObject> vct;
	vct.push_back(Item);
	AddItemsAt (itemId, vct,index);
}

void CFlexObject::ReplaceItemAt(CString itemId,int index, CASObject newItem)
{
	root.Call("__replaceItemOfDataProviderByIndex", (CASObject)itemId,index,newItem);
}

void CFlexObject::ReplaceItem(CString itemId, CASObject oldItem,CASObject newItem)
{
	root.Call("__replaceItemOfDataProviderByObject", (CASObject)itemId, oldItem,newItem);
}

void CFlexObject::RemoveItemAt(CString itemId,int index)
{
	root.Call ("__removeFromDataProviderByIndex", (CASObject)itemId, index);
}


void CFlexObject::RemoveItem(CString itemId,CASObject Item)
{
	root.Call ("__removeFromDataProviderByObject", (CASObject)itemId, Item);
}

int CFlexObject::GetItemIndex(CString itemId,CASObject Item)
{	
	return CFlexCtrl::GetIntegerValue(root.Call("__getItemIndex", (CASObject)itemId, Item));
}

std::map<std::string,CString> CFlexObject::GetGridItemAt(CString itemId,int index)
{	
	std::map<std::string, CString> ret;
	CASObject dataPr = root.Call(convertGet(itemId)).Call("getdataProvider").Call("getItemAt",index);
	if (dataPr.GetType() == ASOBJECT_OBJECT)
	{
		CASObject tmpObj = root.Call(convertGet(itemId)).Call("getcolumns");
		CASValue cv,dat,ant;
		tmpObj.GetValue(cv);
		std::vector<CASObject> arr = *cv.m_aVal;		
		std::map<std::string,CASObject> myMap;
		dataPr.GetValue(dat);
		myMap = *(dat.m_oVal);
		for (int i=0;i<arr.size();i++)
		{
			CString cs = CFlexCtrl::GetStringValue(arr[i].Call("getdataField"));
			ret[(std::string)cs] = CFlexCtrl::GetStringValue(myMap[(std::string)cs]);	
		}
		return ret;
	}else
		return ret;	
}

CString CFlexObject::GetItemAt(CString itemId,int index)
{
	CASObject dataPr = root.Call(convertGet(itemId)).Call("getdataProvider").Call("getItemAt",index);
	if (dataPr.GetType() != ASOBJECT_OBJECT) 
		return CFlexCtrl::GetStringValue(dataPr);
	else 
		return "Error" ;
}

void CFlexObject::Alert(CString message, CString title, int buttons, int alertId)
{
	CallFunction("__showAlert", (CASObject)message, (CASObject)title, (CASObject)buttons, alertId);
}

void CFlexObject::Alert(CString message, CString title, int alertId)
{
	Alert(message, title, CFlexObject::FLEX_ALERT_OK_ONLY, alertId);
}

void CFlexObject::Alert(CString message, int alertId)
{
	Alert(message, "", CFlexObject::FLEX_ALERT_OK_ONLY,alertId);
}


CString CFlexObject::GetSelectedOfRadioButtonGroup(CString itemId)
{
	return CFlexCtrl::GetStringValue(root.Call(convertGet(itemId)).Call("getselectedValue"));
}

void CFlexObject::RemoveAll(CString itemId)
{
	AddItem(itemId, "");
	root.Call(convertGet(itemId)).Call("getdataProvider").Call("removeAll");
}

void CFlexObject::AddDataGridColumn(CString itemId, CString columnId, CString columnName)
{
	root.Call("__addColumn", (CASObject)itemId, (CASObject)columnId, (CASObject)columnName);
}

void CFlexObject::RemoveDataGridColumn(CString itemId, CString columnId)
{
	root.Call("__removeColumn", (CASObject)itemId, (CASObject)columnId);
}

void CFlexObject::RenameDataGridColumn(CString itemId, CString columnId, CString columnName)
{
	root.Call("__renameColumn", (CASObject)itemId, (CASObject)columnId, (CASObject)columnName);
}

void CFlexObject::AddDGItem(CString itemId, std::vector<CASObject> item)
{
	root.Call("__addToGridDataProvider",(CASObject)itemId,item, -1);
}

void CFlexObject::AddDGItemAt(CString itemId, std::vector<CASObject> item, int index)
{
	root.Call("__addToGridDataProvider",(CASObject)itemId,item, index);
}

void CFlexObject::RemoveDGItem(CString itemId, std::vector<CASObject> item)
{
	root.Call("__removeFromDataGridDataProviderByObject", (CASObject)itemId, (CASObject)item);
}

void CFlexObject::ReplaceDGItemAt(CString itemId, std::vector<CASObject> item, int index)
{
	root.Call("__replaceGridItemOfDataProviderByIndex",(CASObject)itemId,index,item);
}

void CFlexObject::RemoveDGItemAt(CString itemId, int index)
{
	RemoveItemAt(itemId, index);
}
void CFlexObject::ReplaceDGItem(CString itemId, std::vector<CASObject> oldItem, std::vector<CASObject> newItem)
{
	root.Call("__replaceItemOfDataGridDataProviderByObject",(CASObject)itemId,oldItem,newItem);
}

//++++++++++++++++++++++ CHART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void CFlexObject::ChartSetHorizontalAxis(CString sItemID, CString sCategoryField, CString sDisplayName, CString sTitle)
{
	root.Call("__setHorizontalAxis", (CASObject)sItemID, (CASObject)sCategoryField, (CASObject)sDisplayName, (CASObject)sTitle);
}

void CFlexObject::ChartAddPieSeries(CString sItemID, CString sSeriesIdentifier, CString sDisplayName)
{
	root.Call("__addPieSeries", (CASObject)sItemID, (CASObject)sDisplayName, (CASObject)sSeriesIdentifier);
}

void CFlexObject::ChartRenameSeries(CString itemID,CString sSeriesIdentifier,CString sDisplayName)
{
	root.Call("__renameSeries", (CASObject)itemID, (CASObject)sSeriesIdentifier,(CASObject)sDisplayName);
}

void CFlexObject::ChartRemoveSeries(CString sItemID, CString sSeriesIdentifier)
{
	root.Call("__removeSeries", (CASObject)sItemID,(CASObject)sSeriesIdentifier);
}

void CFlexObject::ChartRemoveAllSeries(CString itemID)
{
	root.Call("__removeAllSeries", (CASObject)itemID);
}

void CFlexObject::ChartAddPieDataPoint(CString sItemID, CString sSeries, CString sCategory, double iVal, CString sLabel)
{
	root.Call("__addPieDataPoint", (CASObject)sItemID,(CASObject)sSeries, (CASObject)sCategory, iVal, (CASObject)sLabel);
}

void CFlexObject::ChartClearAllDataPoints(CString sItemID)
{
	root.Call("__clearAllDataPoints", (CASObject)sItemID);
}

void CFlexObject::ChartAddSlideEffecttoSeries(CString itemID, CString seriesIdentifier,double duration, CString direction)
{
	root.Call("__addSlideEffecttoSeries", (CASObject)itemID, (CASObject)seriesIdentifier,duration,(CASObject)direction);
}

void CFlexObject::ChartAddZoomEffecttoSeries(CString itemID, CString seriesIdentifier,double duration, CString hrFocus, CString vrFocus)
{
	root.Call("__addZoomEffecttoSeries", (CASObject)itemID, (CASObject)seriesIdentifier,duration,(CASObject)hrFocus,(CASObject)vrFocus);
}

void CFlexObject::ChartAddInterpolateEffecttoSeries(CString itemID, CString seriesIdentifier,double duration, CString direction)
{
	root.Call("__addInterpolateEffecttoSeries",(CASObject)itemID, (CASObject)seriesIdentifier, duration, (CASObject)direction);
}

void CFlexObject::ChartShowLegend(CString itemID)
{
	root.Call("__showLegend", (CASObject)itemID);
}

void CFlexObject::ChartHideLegend(CString itemID)
{
	root.Call("__hideLegend", (CASObject)itemID);
}

void CFlexObject::ChartShowDataTip(CString itemID)
{
	root.Call("__showDataTip", (CASObject)itemID);
}

void CFlexObject::ChartHideDataTip(CString itemID)
{
	root.Call("__hideDataTip", (CASObject)itemID);
}

void CFlexObject::ChartShowAllDataTips(CString itemID)
{
	root.Call("__showAllDataTips", (CASObject)itemID);
}

void CFlexObject::ChartHideAllDataTips(CString itemID)
{
	root.Call("__hideAllDataTips", (CASObject)itemID);
}


//+++++++++++++++++++++++ NEW +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void CFlexObject::ChartAddSeries(CString sItemID, int iSeriesType, CString sSeriesIdentifier, 
								 CString sDisplayName, int iColor, double dThickness, double dAlpha, 
								 CString sForm)
{
	std::vector<CASObject> vctTemp;
	vctTemp.push_back(iColor);
	vctTemp.push_back(dAlpha);
	vctTemp.push_back(dThickness);
	vctTemp.push_back((CASObject)sForm);
	if(iColor == -1)
	{
		vctTemp.push_back(0);
	}else{
		if(dThickness == -1 && dAlpha != -1){
			vctTemp.push_back(2);
		}
		if(dThickness != -1 && dAlpha == -1)
		{
			vctTemp.push_back(3);
		}
		if(dThickness != -1 && dAlpha != -1)
		{
			vctTemp.push_back(4);
		}
		if(dThickness == -1 && dAlpha == -1)
		{
			vctTemp.push_back(1);
		}
	}
	
	root.Call("__addSeries", (CASObject)sItemID, (CASObject)iSeriesType, (CASObject)sSeriesIdentifier, (CASObject)sDisplayName, vctTemp);
}

void CFlexObject::ChartAddDataPoint(CString sItemID, CString sSeriesIdentifier, double dXVal, double dYVal)
{
	buffChartID.push_back((CASObject)sItemID);
	buffChartSID.push_back((CASObject)sSeriesIdentifier);
	buffChartXVal.push_back(dXVal);
	buffChartYVal.push_back(dYVal);
}

void CFlexObject::ChartUpdateCharts()
{
	int iSize = buffChartID.size();
	int iCount = iSize / 100;
	int iBalance = iSize % 100;
	std::vector<CASObject> vctTemp1;
	std::vector<CASObject> vctTemp2;
	std::vector<CASObject> vctTemp3;
	std::vector<CASObject> vctTemp4;
	for(int i = 0; i < iCount; i++)
	{
		vctTemp1.clear();
		vctTemp2.clear();
		vctTemp3.clear();
		vctTemp4.clear();
		for(int j = 0; j < 100; j++){
			vctTemp1.push_back(buffChartID[i*100 + j]);
			vctTemp2.push_back(buffChartSID[i*100 + j]);
			vctTemp3.push_back(buffChartXVal[i*100 + j]);
			vctTemp4.push_back(buffChartYVal[i*100 + j]);
		}
		root.Call("__addDataPoints",vctTemp1, vctTemp2, vctTemp3, vctTemp4, false);
	}
	vctTemp1.clear();
	vctTemp2.clear();
	vctTemp3.clear();
	vctTemp4.clear();
	for (int k = 0; k < iBalance; k++)
	{
		vctTemp1.push_back(buffChartID[iCount*100 + k]);
		vctTemp2.push_back(buffChartSID[iCount*100 + k]);
		vctTemp3.push_back(buffChartXVal[iCount*100 + k]);
		vctTemp4.push_back(buffChartYVal[iCount*100 + k]);
	}
	//AfxMessageBox(CFlexCtrl::GetStringValue(vctTemp4[4]));
	root.Call("__addDataPoints",vctTemp1, vctTemp2, vctTemp3, vctTemp4,true);
	vctTemp1.clear();
	vctTemp2.clear();
	vctTemp3.clear();
	vctTemp4.clear();
	buffChartID.clear();
	buffChartSID.clear();
	buffChartXVal.clear();
	buffChartYVal.clear();
}

void CFlexObject::ChartShowAxisLabelHr(CString sItemID, bool bShow)
{
	root.Call("__showHideChartHrAxisLabels", (CASObject)sItemID, bShow);
}
void CFlexObject::ChartShowAxisLabelVr(CString sItemID, bool bShow)
{
	root.Call("__showHideChartVrAxisLabels", (CASObject)sItemID, bShow);
}
void CFlexObject::ChartShowAxisHr(CString sItemID, bool bShow)
{
	root.Call("__showHideChartHrAxis", (CASObject)sItemID, bShow);
}
void CFlexObject::ChartShowAxisVr(CString sItemID, bool bShow)
{
	root.Call("__showHideChartVrAxis", (CASObject)sItemID, bShow);
}

void CFlexObject::ChartSetGrid(CString sItemID, CString sType,int iColor, double dThickness, double dAlpha)
{
	root.Call("__setChartGrid", (CASObject)sItemID, (CASObject)sType, iColor, dThickness,dAlpha);
}

void CFlexObject::ChartClearDataPointBuffer()
{
	buffChartID.clear();
	buffChartSID.clear();
	buffChartXVal.clear();
	buffChartYVal.clear();
}

void CFlexObject::ChartSetAxisLabelHr(CString sItemID, CString sTitle, CString sDisplayName)
{
	root.Call("__setHorizontalAxisLabel", (CASObject)sItemID, (CASObject)sTitle, (CASObject)sDisplayName);
}

void CFlexObject::ChartSetAxisLabelVr(CString sItemID, CString sTitle, CString sDisplayName)
{
	root.Call("__setVerticalAxisLabel", (CASObject)sItemID, (CASObject)sTitle,(CASObject)sDisplayName);
}