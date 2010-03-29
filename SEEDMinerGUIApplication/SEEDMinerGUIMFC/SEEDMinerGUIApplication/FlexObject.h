// FlexObject.h: interface for the CFlexObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEXOBJECT_H__74AEF654_C7E0_47CC_AB4B_62EF4B340A09__INCLUDED_)
#define AFX_FLEXOBJECT_H__74AEF654_C7E0_47CC_AB4B_62EF4B340A09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "FlexCallBack.h"
#include "ASObject.h"
#endif // _MSC_VER > 1000


class CFlexObject  
{
public:
	static CString  FLEX_EVT_ACTIVATE;
	static CString  FLEX_EVT_CLICK;
	static CString  FLEX_EVT_DEACTIVATE;
	static CString  FLEX_EVT_DOUBLE_CLICK;
	static CString  FLEX_EVT_KEY_FOCUS_CHANGE;
	static CString  FLEX_EVT_TAB_INDEX_CHANGE;
	static CString  FLEX_EVT_CHANGE;
	static CString  FLEX_EVT_DATA_CHANGE;
	static CString  FLEX_EVT_ITEM_CLICK;
	static CString  FLEX_EVT_ENTER_KEY_PRESSED;

	static int FLEX_ALERT_OK_ONLY;
	static int FLEX_ALERT_OK_CANCEL;
	static int FLEX_ALERT_YES_NO;
	static int FLEX_ALERT_YES_NO_CANCEL;
	CFlexObject(CFlexCallBack* flexCallback, int flashIndex);
	CFlexObject();
	virtual ~CFlexObject();
	//Control Methods
	void      AddEventListener(CString itemId, CString eventType);
	CASObject CallFunction(CString func, CASObject arg1);
	CASObject CallFunction(CString func, CASObject arg1, CASObject arg2);
	CASObject CallFunction(CString func, CASObject arg1, CASObject arg2, CASObject arg3);
	CASObject CallFunction(CString func, CASObject arg1, CASObject arg2, CASObject arg3, CASObject arg4);

	void      Alert(CString message, CString title, int buttons, int alertId);
	void      Alert(CString message, CString title, int alertId);
	void      Alert(CString message, int alertId);

	void      SetProperty(CString itemId, CString prop, CASObject val);
	CASObject GetProperty(CString itemId, CString prop);
	CString   GetPropertyString(CString itemId, CString prop);
	int       GetPropertyInteger(CString itemId, CString prop);
	double    GetPropertyDouble(CString itemId, CString prop);
	bool      GetPropertyBoolean(CString itemId, CString prop);

	void      SetFocus(CString itemId);
	void      SetEnabled(CString itemId,bool status = true);
	void      SetText(CString itemId, CString text);
	void      SetLabel(CString itemId, CString text);
	void      SetValue(CString itemId, int val);
	void      SetValue(CString itemId, double val);
	void      SetSelected(CString itemId, bool val);
	void      SetMaximum(CString itemId, double val);
	void      SetMinimum(CString itemId, double val);
	void      SetStepSize(CString itemId, double val);
	void      SetVisible(CString itemId, bool val);
	void      SetToolTip(CString itemId, CString text);

	CString   GetText(CString itemId);
	CString   GetLabel(CString itemId);
	int       GetValueInteger(CString itemId);
	double    GetValueDouble(CString itemId);
	bool      GetSelected(CString itemId);
	double    GetMaximum(CString itemId);
	double    GetMinimum(CString itemId);
	double    GetStepSize(CString itemId);
	bool      GetVisible(CString itemId);
	CString   GetToolTip(CString itemId);
	

	void		AddToDataProvider(CString itemId, std::vector<CASObject> vArgArgs);
	void		AddDataGridColumn(CString itemId, CString columnId, CString columnName);
	void		RemoveDataGridColumn(CString itemId, CString columnId);
	void		RenameDataGridColumn(CString itemId, CString columnId, CString columnName);
	
	void AddDGItem(CString itemId, std::vector<CASObject> item);
	void AddDGItemAt(CString itemId, std::vector<CASObject> item, int index);
	void RemoveDGItem(CString itemId, std::vector<CASObject> item);
	void RemoveDGItemAt(CString itemId, int index);
	void ReplaceDGItemAt(CString itemId, std::vector<CASObject> item, int index);
	void ReplaceDGItem(CString itemId, std::vector<CASObject> oldItem, std::vector<CASObject> newItem);

	//add a Item to a Combo Box,List,Horizontal List
	void		AddItem(CString itemId, CASObject Item);			
	//add a Set of Items to a Combo Box,List,Horizontal List or a DataGrid
	void		AddItems(CString itemId, std::vector<CASObject> Items);
				
	//Insert a Set of Item to a Combo Box,List,Horizontal List or a DataGrid at a specific Index - zero based index
	void		AddItemsAt(CString itemId, std::vector<CASObject> Items,int index);
				
	//add a Item to a Combo Box,List,Horizontal List at a specific Index - zero based index
	void		AddItemAt(CString itemId, CASObject Item,int index);
				
	//Replace a Item in a Combo Box,List,Horizontal List using zero based Index
	void		ReplaceItemAt(CString itemId,int index, CASObject newItem);
				
	//Replace a Item in a DataGrid using zero based Index
	void		ReplaceItem(CString itemId, CASObject oldItem,CASObject newItem);
				
	//Remove a Item in a Combo Box,List,Horizontal List or a DataGrid using zero based Index
	void		RemoveItemAt(CString itemId, int index);

	void		RemoveAll(CString itemId);
				
	//Remove a Item in a Combo Box,List,Horizontal List 
	void		RemoveItem(CString itemId, CASObject Item);
				
	//Get the Index of a Items' first appearance in a Combo Box,List,Horizontal List except DataGrids
	int 		GetItemIndex(CString itemId, CASObject Item);
				
	//Get a Item in a Data Grid using zero based Index
	std::map<std::string,CString> GetGridItemAt(CString itemId,int index);
				
	//Get a Item in a Combo Box,List,Horizontal List using zero based Index
	CString		GetItemAt(CString itemId,int index);

	CString     GetSelectedOfRadioButtonGroup(CString itemId);

	void ChartAddSeries(CString sItemID, int iSeriesType, CString sSeriesIdentifier, CString sDisplayName, int iColor, double dThickness, double dAlpha, CString sForm);
	void ChartAddDataPoint(CString sItemID, CString sSeriesIdentifier, double dXVal, double dYVal);
	void ChartUpdateCharts();
	void ChartShowAxisLabelHr(CString sItemID, bool bShow);
	void ChartShowAxisLabelVr(CString sItemID, bool bShow);
	void ChartShowAxisHr(CString sItemID, bool bShow);
	void ChartShowAxisVr(CString sItemID, bool bShow);
	void ChartSetGrid(CString sItemID, CString sType,int iColor, double dThickness, double dAlpha);
	void ChartClearDataPointBuffer();
	void ChartSetAxisLabelHr(CString sItemID, CString sTitle, CString sDisplayName);
	void ChartSetAxisLabelVr(CString sItemID, CString sTitle, CString sDisplayName);

	void ChartSetHorizontalAxis(CString sItemID, CString sCategoryField, CString sDisplayName, CString sTitle);
	void ChartAddPieSeries(CString sItemID, CString sSeriesIdentifier, CString sDisplayName);
	void ChartRenameSeries(CString itemID,CString sSeriesIdentifier,CString sDisplayName);
	void ChartRemoveSeries(CString sItemID, CString sSeriesIdentifier);
	void ChartRemoveAllSeries(CString itemID);
	void ChartAddPieDataPoint(CString sItemID, CString sSeries, CString sCategory, double iVal, CString sLabel);
	void ChartClearAllDataPoints(CString sItemID);
	void ChartAddSlideEffecttoSeries(CString itemID, CString seriesIdentifier,double duration, CString direction);
	void ChartAddZoomEffecttoSeries(CString itemID, CString seriesIdentifier,double duration, CString hrFocus, CString vrFocus);
	void ChartAddInterpolateEffecttoSeries(CString itemID, CString seriesIdentifier,double duration, CString direction);
	void ChartShowLegend(CString itemID);
	void ChartHideLegend(CString itemID);
	void ChartShowDataTip(CString itemID);
	void ChartHideDataTip(CString itemID);
	void ChartShowAllDataTips(CString itemID);
	void ChartHideAllDataTips(CString itemID);


	CASObject root;
private:
	CString convertSet(CString item);
	CString convertGet(CString item);

	std::vector<CASObject> buffChartID;
	std::vector<CASObject> buffChartSID;
	std::vector<CASObject> buffChartXVal;
	std::vector<CASObject> buffChartYVal;

};

#endif // !defined(AFX_FLEXOBJECT_H__74AEF654_C7E0_47CC_AB4B_62EF4B340A09__INCLUDED_)

