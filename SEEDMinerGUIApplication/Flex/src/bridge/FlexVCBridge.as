import flash.events.Event;

import mx.charts.AxisRenderer;
import mx.charts.CategoryAxis;
import mx.charts.ColumnChart;
import mx.charts.GridLines;
import mx.charts.Legend;
import mx.charts.LineChart;
import mx.charts.PieChart;
import mx.charts.chartClasses.CartesianChart;
import mx.charts.chartClasses.ChartBase;
import mx.charts.effects.SeriesInterpolate;
import mx.charts.effects.SeriesSlide;
import mx.charts.effects.SeriesZoom;
import mx.charts.events.ChartItemEvent;
import mx.charts.series.AreaSeries;
import mx.charts.series.BarSeries;
import mx.charts.series.ColumnSeries;
import mx.charts.series.LineSeries;
import mx.charts.series.PieSeries;
import mx.collections.ArrayCollection;
import mx.containers.Canvas;
import mx.containers.Panel;
import mx.controls.Alert;
import mx.controls.Button;
import mx.controls.CheckBox;
import mx.controls.ComboBox;
import mx.controls.DataGrid;
import mx.controls.HSlider;
import mx.controls.HorizontalList;
import mx.controls.List;
import mx.controls.MenuBar;
import mx.controls.NumericStepper;
import mx.controls.ProgressBar;
import mx.controls.RadioButton;
import mx.controls.TextArea;
import mx.controls.TextInput;
import mx.controls.VSlider;
import mx.controls.dataGridClasses.DataGridColumn;
import mx.core.Container;
import mx.events.CloseEvent;
import mx.events.MenuEvent;
import mx.graphics.SolidColor;
import mx.graphics.Stroke;
import mx.managers.PopUpManager;

	public var __callBackFunction:Function;
	var __flashPlayerId:int;
	var __flashPlayerIndex:int;
	var __alertId:Array;
	var __children:Object = new Object;
	var __childrenArray:Array = new Array;
	var __zcItemID:Array = new Array;
	var __zcSeries:Array = new Array;
	var __zcXField:Array = new Array;
	var __zcYField:Array = new Array;
	
	var __cnvsProg:Canvas = new Canvas;
	var __pnlProg:Panel = new Panel;
	var __prgProg:ProgressBar = new ProgressBar;
	var __isProgShow:Boolean = false;
	public var __explodingArray:Array = new Array();
		public function __assignEvents():void{
			if(__childrenArray.length == 0)
				__loadChildren();
			var a:Array = __childrenArray;
			for(var i:int = 0; i < a.length; i++){
				a[i].doubleClickEnabled = true;
				if(a[i] is Button){
					//a[i].addEventListener("click",__onEvent);
				}
				if(a[i] is CheckBox){
					a[i].addEventListener("change",__onEvent);
				}
				if(a[i] is RadioButton){
					a[i].addEventListener("change",__onEvent);
				}
				if(a[i] is NumericStepper){
					a[i].addEventListener("change",__onEvent);
				}
				if(a[i] is TextInput || a[i] is TextArea){
					a[i].addEventListener("enter",__onEvent);
					a[i].addEventListener("focusOut",__onEvent);
				}
				if(a[i] is VSlider || a[i] is HSlider){
					a[i].addEventListener("change",__onEvent);
				}
				if(a[i] is ComboBox){
					a[i].addEventListener("change",__onEvent);
				}
				if(a[i] is HorizontalList || a[i] is List ){
					a[i].addEventListener("change",__onEvent);
					a[i].addEventListener("doubleClick",__onEvent);
				}
				if(a[i] is DataGrid){
					a[i].addEventListener("itemClick",__onEvent);
					a[i].addEventListener("doubleClick",__onEvent);
				}
				if(a[i] is ChartBase){
					a[i].addEventListener("itemClick",__onEvent);
				}
				if(a[i] is MenuBar){
					a[i].addEventListener("itemClick",__handleMenuClick);
				}
			}
		}
		
		public function __autoAssignEvents():void{
			__assignEvents();
		}
		
		public function __loadChildren():void{
			__childrenArray = __getChildernArray(this);
			for(var i:int = 0; i < __childrenArray.length; i++){
				__children[__childrenArray[i].id.toString()] = __childrenArray[i];
			}
		}
		
		public function __setCallBackFunction(flashId:int,flashIndex:int, callbackfn:Function):void
		{
			__flashPlayerId = flashId;
			__flashPlayerIndex = flashIndex;
			__callBackFunction = callbackfn;
			__loadChildren();
			__pnlProg.width = 160;
			__pnlProg.height = 50;
			__pnlProg.setStyle("cornerRadius",10);
			__pnlProg.setStyle("cornerRadius",10); 
			__pnlProg.setStyle("roundedBottomCorners",true); 
			__pnlProg.setStyle("borderStyle","solid"); 
			__pnlProg.setStyle("borderAlpha",0); 
			__pnlProg.setStyle("backgroundAlpha",0.5);
			__cnvsProg.addChild(__pnlProg);
			__prgProg.x = 10;
			__prgProg.y = 10;
			__prgProg.width = 140;
			__prgProg.height = 30;
			__prgProg.indeterminate = true;
			__cnvsProg.addChild(__prgProg);
		}
		public function __showProgress(text:String):void{
			if(!__isProgShow){
				__prgProg.label = text;
				__isProgShow = true;
				__cnvsProg.x = width/2 - 80;
				__cnvsProg.y = height/2 - 80;
				PopUpManager.addPopUp(__cnvsProg,this,true);
			}
		}
		
		public function __hideProgress():void{
			if(__isProgShow){
				__isProgShow = false;
				PopUpManager.removePopUp(__cnvsProg);
			}
		}
		
		public function __addListener(item:String, event:String):void
		{
			__getChildByName(item).addEventListener(event,__onEvent);
		} 
		
		public function __onEvent(event:Event):void{
			var ret:Object = new Object();
			var iid:Array = event.currentTarget.toString().split(".");
			var itemId:String = iid[iid.length - 1];
			//var itemId:String = event.target.id;
			var itemType:String = event.currentTarget.toLocaleString().replace("[object ","").replace("]","");
			ret["flashId"] = __flashPlayerId;
			ret["flashIndex"] = __flashPlayerIndex;
			ret["itemId"] = itemId;
			ret["itemType"] = itemType;
			ret["eventType"] = event.type.toString();
			if(event.currentTarget is PieChart)
			{
				ret["itemType"] = "PieChart";
				var evt:ChartItemEvent = (event as ChartItemEvent);
				ret["seriesName"] = evt.hitData.chartItem.element.name;
				ret["value"] = evt.hitData.chartItem.item[ret["seriesName"]];
				ret["field"] = evt.hitData.chartItem.item["__data__"];
			}
			if(event.currentTarget is CartesianChart)
			{
				ret["itemType"] = "CartesianChart";
				var evt:ChartItemEvent = (event as ChartItemEvent);
				ret["xField"] = evt.hitData.chartItem.item["__X__"];
				ret["seriesName"] = evt.hitData.chartItem.element.name;
				ret["yField"] = evt.hitData.chartItem.item[ret["seriesName"]];
			}
			__callBackFunction.call(this,ret);
		}
		
		public function __addColumn(item:String, colId:String, colName:String):void{//colId:String, colName:String
			var dataGridObject:Object = (__getChildByName( item) as DataGrid);
	 		var dgc:DataGridColumn = new DataGridColumn(colName);
	 		dgc.dataField = colId;
	 		dgc.editable = true;
	 		dgc.sortable = false;
            var cols:Array = dataGridObject.columns;
            cols.push(dgc);
           	dataGridObject.columns = cols;
		}
		public function __removeColumn(item:String, colId:String):void{//colId:String, colName:String
			var dataGridObject = (__getChildByName( item) as DataGrid);
            var cols:Array = dataGridObject.columns;
            var colsNew:Array = new Array();
            var dgc:DataGridColumn;
            while(cols.length > 0){
            	dgc = cols.pop();
            	if(dgc.dataField != colId)
            		colsNew.push(dgc);
            }
            while(colsNew.length > 0){
            	cols.push(colsNew.pop());
            }
           	dataGridObject.columns = cols;
		}
		public function __removeAllColumns(item:String):void{
			var dataGridObject = (__getChildByName( item) as DataGrid);
            var colsNew:Array = new Array();
           	dataGridObject.columns = colsNew;
		}
		public function __renameColumn(item:String, colId:String, colName:String):void{
			var dataGridObject = (__getChildByName( item) as DataGrid);
			var cols:Array = dataGridObject.columns;
            var colsNew:Array = new Array();
            var dgc:DataGridColumn;
            while(cols.length > 0){
            	dgc = cols.pop();
            	if(dgc.dataField == colId)
            		dgc.headerText = colName;
            	colsNew.push(dgc);
            }
            while(colsNew.length > 0){
            	cols.push(colsNew.pop());
            }
           	dataGridObject.columns = cols;
		}
		public function __showAlert(message:String, title:String, options:int, ___alertId:int):void{
			if(options == 0)
				Alert.show(message,title,Alert.OK, this, __alertClickHandler);
			if(options == 1)
				Alert.show(message,title,Alert.OK|Alert.CANCEL, this, __alertClickHandler);
			if(options == 2)
				Alert.show(message,title,Alert.YES|Alert.NO, this, __alertClickHandler);
			if(options == 3)
				Alert.show(message,title,Alert.YES|Alert.NO|Alert.CANCEL ,this, __alertClickHandler);
			if(__alertId == null)
				__alertId = new Array();
			__alertId.push(___alertId);
		}
        
            // Event handler function for displaying the selected Alert button.
        public function __alertClickHandler(event:CloseEvent):void {
          	var reply:String;
            if (event.detail==Alert.YES)
                reply = "yes";
            if (event.detail==Alert.NO)
                reply = "no";
            if (event.detail==Alert.CANCEL)
                reply = "cancel";
            if (event.detail==Alert.OK)
                reply = "ok";
			var ret:Object = new Object();
			ret["flashId"] = __flashPlayerId;
			ret["flashIndex"] = __flashPlayerIndex;
			ret["itemId"] = "Alert";
			ret["itemType"] = "Alert";
			ret["eventType"] = "alertRespond";
			ret["__alertId"] = (__alertId.pop() as int);
			ret["respond"] = reply;
			__callBackFunction.call(this,ret);
        }
        
        public function __addToGridDataProvider(item:String, _dataOfDataProvider:Object, index:int):void{

        	var dataOfDataProvider:Array = (_dataOfDataProvider as Array);

			var dataGridObject:DataGrid = (__getChildByName( item)) as DataGrid;

			if(!dataGridObject.dataProvider)
					dataGridObject.dataProvider = new ArrayCollection();
			var _data:Object = new Object();
			var ccount:int = dataGridObject.columnCount;

			for(var i:int = 0; i<ccount; i++){
				if(dataOfDataProvider.length > i)
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = dataOfDataProvider[i].toString();
				else
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = "";
			} 
			
			if(index == -1){
					(dataGridObject.dataProvider as ArrayCollection).addItem(_data);
			}else{
					(dataGridObject.dataProvider as ArrayCollection).addItemAt(_data,index++);
			}
        }
        
        public function __replaceGridItemOfDataProviderByIndex(item:String, index:int, _dataOfDataProvider:Object):void{
			var dataOfDataProvider:Array = (_dataOfDataProvider as Array);

			var dataGridObject:DataGrid = (__getChildByName( item)) as DataGrid;

			if(!dataGridObject.dataProvider)
					dataGridObject.dataProvider = new ArrayCollection();
			var _data:Object = new Object();
			var ccount:int = dataGridObject.columnCount;

			for(var i:int = 0; i<ccount; i++){
				if(dataOfDataProvider.length > i)
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = dataOfDataProvider[i].toString();
				else
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = "";
			} 
			if(dataGridObject.dataProvider.length > index)
				dataGridObject.dataProvider.setItemAt(_data,index);
		}
        
        public function __addToDataProvider(item:String, _dataOfDataProvider:Object, index:int):void{
			var dataOfDataProvider:Array = (_dataOfDataProvider as Array);
			var dataGridObject:Object = __getChildByName( item);
			if(!dataGridObject.dataProvider)
					dataGridObject.dataProvider = new ArrayCollection();
			if(index == -1){
				for(var i:int = 0; i < dataOfDataProvider.length; ++i){
					(dataGridObject.dataProvider as ArrayCollection).addItem(dataOfDataProvider[i]);
				}
			}else{
				for(var i:int = 0; i < dataOfDataProvider.length; ++i){
					(dataGridObject.dataProvider as ArrayCollection).addItemAt(dataOfDataProvider[i],index++);
				}
			}
		}
		
		
		
		public function __removeFromDataProviderByIndex(item:String, index:int):void{
			var dataObject:Object = __getChildByName( item);
			if(dataObject.dataProvider.length > index)
				dataObject.dataProvider.removeItemAt(index);
		}
		public function __removeFromDataGridDataProviderByObject(item:String, _dataOfDataProvider:Object):void{
			var dataOfDataProvider:Array = (_dataOfDataProvider as Array);

			var dataGridObject:DataGrid = (__getChildByName( item)) as DataGrid;

			if(!dataGridObject.dataProvider)
					dataGridObject.dataProvider = new ArrayCollection();
			var _data:Object = new Object();
			var ccount:int = dataGridObject.columnCount;

			for(var i:int = 0; i<ccount; i++){
				if(dataOfDataProvider.length > i)
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = dataOfDataProvider[i].toString();
				else
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = "";
			} 
			__removeFromDataProviderByObject(item, _data);
		}
		 public function __removeFromDataProviderByObject(item:String, _dataOfDataProvider:Object):void{
		 	var dataObject:Object = __getChildByName( item);
            if(dataObject.className.toString() == "DataGrid"){
            	var dataGridObject:DataGrid = (dataObject as DataGrid);
            	for(var i:int = 0; i < (dataGridObject.dataProvider as ArrayCollection).length; ++i){
            		var ok:Boolean = false;
            		for(var j:int = 0; j < dataGridObject.columnCount; ++j){
            			if(j == 0 || ok)
            				if(_dataOfDataProvider[(dataGridObject.columns[j] as DataGridColumn).dataField.toString()] == ((dataGridObject.dataProvider as ArrayCollection).getItemAt(i))[(dataGridObject.columns[j] as DataGridColumn).dataField.toString()]){
            					ok = true;
            				}else{
            					ok = false;
            				}
            		}
            		if(ok){
            			(dataGridObject.dataProvider as ArrayCollection).removeItemAt(i);
            			--i;
            		}
            	}
            }else{
            	if((dataObject.dataProvider as ArrayCollection).getItemIndex(_dataOfDataProvider) != -1)
            		(dataObject.dataProvider as ArrayCollection).removeItemAt((dataObject.dataProvider as ArrayCollection).getItemIndex(_dataOfDataProvider));           	
            }
		 }
		 
		 public function __replaceItemOfDataGridDataProviderByObject(item:String, _dataOfDataProvider:Object, _newDataOfDataProvider:Object):void{
		 	var dataOfDataProvider:Array = (_dataOfDataProvider as Array);
			var newdataOfDataProvider:Array = (_newDataOfDataProvider as Array);
			
			var dataGridObject:DataGrid = (__getChildByName( item)) as DataGrid;

			if(!dataGridObject.dataProvider)
					dataGridObject.dataProvider = new ArrayCollection();
			var _data:Object = new Object();
			var _data2:Object = new Object();
			var ccount:int = dataGridObject.columnCount;

			for(var i:int = 0; i<ccount; i++){
				if(dataOfDataProvider.length > i){
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = dataOfDataProvider[i].toString();
					_data2[(dataGridObject.columns[i] as DataGridColumn).dataField] = newdataOfDataProvider[i].toString();
				}
				else{
					_data[(dataGridObject.columns[i] as DataGridColumn).dataField] = "";
					_data2[(dataGridObject.columns[i] as DataGridColumn).dataField] = "";
				}
			} 
			__replaceItemOfDataProviderByObject(item,_data, _data2);
		 }
		 
		 public function __replaceItemOfDataProviderByObject(item:String, _dataOfDataProvider:Object, _newDataOfDataProvider:Object):void{
		 	var dataObject:Object = __getChildByName( item);
            if(dataObject.className.toString() == "DataGrid"){
            	var dataGridObject:DataGrid = (dataObject as DataGrid);
            	for(var i:int = 0; i < (dataGridObject.dataProvider as ArrayCollection).length; ++i){
            		var ok:Boolean = false;
            		for(var j:int = 0; j < dataGridObject.columnCount; ++j){
            			if(j == 0 || ok)
            				if(_dataOfDataProvider[(dataGridObject.columns[j] as DataGridColumn).dataField.toString()] == ((dataGridObject.dataProvider as ArrayCollection).getItemAt(i))[(dataGridObject.columns[j] as DataGridColumn).dataField.toString()]){
            					ok = true;
            				}else{
            					ok = false;
            				}
            		}
            		if(ok){
            			(dataGridObject.dataProvider as ArrayCollection).setItemAt(_newDataOfDataProvider,i);
            			--i;
            		}
            	}
            }else{
            	if((dataObject.dataProvider as ArrayCollection).getItemIndex(_dataOfDataProvider) != -1)
            		(dataObject.dataProvider as ArrayCollection).setItemAt(_newDataOfDataProvider,(dataObject.dataProvider as ArrayCollection).getItemIndex(_dataOfDataProvider) ); //.removeItemAt((dataObject.dataProvider as ArrayCollection).getItemIndex(_dataOfDataProvider));           	
            }
		 }
		 
		 public function __replaceItemOfDataProviderByIndex(item:String, index:int, _newDataOfDataProvider:Object):void{
			var dataObject:Object = __getChildByName( item);
			if(dataObject.dataProvider.length > index)
				dataObject.dataProvider.setItemAt(_newDataOfDataProvider,index);
		 }
		 
		
		 public function __getItemIndex(item:String, objectOfDataprovider:Object):int{
		 	var dataObject:Object = __getChildByName( item);
		 	var indexx:int = -1;
		 	if(dataObject.className.toString() == "DataGrid"){
            	var dataGridObject:DataGrid = (dataObject as DataGrid);
            	for(var i:int = 0; i < (dataGridObject.dataProvider as ArrayCollection).length; ++i){
            		var ok:Boolean = false;
            		for(var j:int = 0; j < dataGridObject.columnCount; ++j){
            			if(j == 0 || ok)
            				if(objectOfDataprovider[(dataGridObject.columns[j] as DataGridColumn).dataField.toString()] == ((dataGridObject.dataProvider as ArrayCollection).getItemAt(i))[(dataGridObject.columns[j] as DataGridColumn).dataField.toString()]){
            					ok = true;
            				}else{
            					ok = false;
            				}
            		}
            		if(ok){
            			indexx = i;
            			break;
            		}
            	}
            	return indexx;
            }else{
		 		return (dataObject.dataProvider as ArrayCollection).getItemIndex(objectOfDataprovider);
            }
		 }
		 
		private function __getChildByName(item:String):Object{
			var ret:Object = __children[item];
			if(!ret)
				ret = __getChildByNameX(this, item);
			return ret;
		}
		
		private function __getChildernArray(parent:Object):Array{
			var ret:Array = new Array;
			if(parent is Container){
				var a:Array = parent.getChildren();
				for(var i:int = 0; i < a.length; i++){
					if(a[i].id)
						ret.push(a[i]);
					ret = ret.concat(__getChildernArray(a[i]));
				}
			}
			return ret;
		}
		 
		 
		 private function __getChildByNameX(parent:Object,item:String):Object{
			var ret:Object = parent.getChildByName(item);
			if(!ret)
			{
				var a:Array = parent.getChildren();
				for(var i:int; i < a.length; i++){
					if(a[i].toLocaleString() == "[object Panel]"){
						ret = __getChildByNameX(a[i],item);
						if(ret)
							break;
					}
				}
				
			}
			return ret;
		}
		private function __handleMenuClick(evt:MenuEvent):void {
        	var menuLabel:String = evt.item.@label;
			var ret:Object = new Object();
			ret["flashId"] = __flashPlayerId;
			ret["flashIndex"] = __flashPlayerIndex;
			ret["itemId"] = evt.menuBar.id;
			ret["itemType"] = "Menu";
			ret["eventType"] = "click";
			ret["clickedItem"] = menuLabel;
			__callBackFunction.call(this,ret);
    	}
//**************************************************************************************************    	
//Chart Functions   	
		public function __getAxisLabel(categoryValue:Object, previousCategoryValue:Object, axis:CategoryAxis, categoryItem:Object):String {
			return categoryItem["sibkascatlbl09"];
		}
		//================****=====================================================================
		public function __addPieDataPoint(sItemID:String, sSeries:String, sCategory:String, iVal:Number, sLabel:String = null):void{
			var cbObject:Object = __getChildByName(sItemID);
			var sDataField:String;
			if(!cbObject.dataProvider){
				cbObject.dataProvider = new ArrayCollection();
			} 
			
			var arrDataProvider:ArrayCollection = (cbObject.dataProvider as ArrayCollection);
			
			if(cbObject.className.toString() == "LineChart")
			{
				sDataField = ((cbObject as LineChart).horizontalAxis as CategoryAxis).categoryField.toString();
				var iSize = arrDataProvider.length;
				var bOk:Boolean = false;
				for(var i:int = 0; i < iSize; i++){
					if(arrDataProvider[i][sDataField].toString() == sCategory){
						arrDataProvider[i][sSeries] = iVal;
						if(sLabel)
							arrDataProvider[i]["sibkascatlbl09"] = sLabel;
						bOk = true;
						break;
					}
				}
				if(!bOk){
					var newDataPoint:Object = new Object();
					newDataPoint[sDataField] = sCategory;
					newDataPoint[sSeries] = iVal;
					if(!sLabel)
						newDataPoint["sibkascatlbl09"] = sCategory;
					else
						newDataPoint["sibkascatlbl09"] = sLabel;	
					arrDataProvider.addItem(newDataPoint);
				}
				cbObject.dataProvider = arrDataProvider;					
			}
			if(cbObject.className.toString() == "ColumnChart")
			{
				sDataField = ((cbObject as ColumnChart).horizontalAxis as CategoryAxis).categoryField.toString();
				var iSize = arrDataProvider.length;
				var bOk:Boolean = false;
				for(var i:int = 0; i < iSize; i++){
					if(arrDataProvider[i][sDataField].toString() == sCategory){
						arrDataProvider[i][sSeries] = iVal;
						if(sLabel)
							arrDataProvider[i]["sibkascatlbl09"] = sLabel;
						bOk = true;
						break;
					}
				}
				if(!bOk){
					var newDataPoint:Object = new Object();
					newDataPoint[sDataField] = sCategory;
					newDataPoint[sSeries] = iVal;
					if(!sLabel)
						newDataPoint["sibkascatlbl09"] = sCategory;
					else
						newDataPoint["sibkascatlbl09"] = sLabel;	
					arrDataProvider.addItem(newDataPoint);
				}
				cbObject.dataProvider = arrDataProvider;					
			}
			if(cbObject.className.toString() == "PieChart")
			{
				sDataField = "__data__";
				var iSize = arrDataProvider.length;
				var bOk:Boolean = false;
				for(var i:int = 0; i < iSize; i++){
					if(arrDataProvider[i][sDataField].toString() == sCategory){
						arrDataProvider[i][sSeries] = iVal;
						if(sLabel)
							arrDataProvider[i]["sibkascatlbl09"] = sLabel;
						bOk = true;
						break;
					}
				}
				if(!bOk){
					var newDataPoint:Object = new Object();
					newDataPoint[sDataField] = sCategory;
					newDataPoint[sSeries] = iVal;
					if(!sLabel)
						newDataPoint["sibkascatlbl09"] = sCategory;
					else
						newDataPoint["sibkascatlbl09"] = sLabel;	
					arrDataProvider.addItem(newDataPoint);
				}
				cbObject.dataProvider = arrDataProvider;					
			}
		}
		//============ OK =========================================================================
		public function __clearAllDataPoints(sItemID:String): void{
			var cbObject:Object = __getChildByName(sItemID);
			cbObject.dataProvider = new ArrayCollection();
		}
		
		//============= *** ========================================================================
		public function __addPieSeries(sItemID:String, sDisplayName:String, sSeriesIdentifier:String): void
		{	
			var cbObject:Object = __getChildByName(sItemID);
			var arrSeries:Array = (cbObject.series as Array);
			
			if(cbObject.className.toString() == "ColumnChart")
			{
				var newColumnSeries:ColumnSeries = new ColumnSeries();
				newColumnSeries.displayName = sDisplayName;
				newColumnSeries.yField = sSeriesIdentifier;
				newColumnSeries.name = sSeriesIdentifier;
				newColumnSeries.id = sSeriesIdentifier;
				newColumnSeries.xField = ((cbObject as ColumnChart).horizontalAxis as CategoryAxis).categoryField.toString();
				arrSeries.push(newColumnSeries);
			}
			if(cbObject.className.toString() == "LineChart")
			{
				var newLineSeries:LineSeries = new LineSeries();
				newLineSeries.displayName = sDisplayName;
				newLineSeries.yField = sSeriesIdentifier;
				newLineSeries.name = sSeriesIdentifier;
				newLineSeries.id = sSeriesIdentifier;
				newLineSeries.xField = ((cbObject as LineChart).horizontalAxis as CategoryAxis).categoryField.toString();
				arrSeries.push(newLineSeries);
			}
			if(cbObject.className.toString() == "PieChart")
			{
				var newPieSeries:PieSeries = new PieSeries();
				newPieSeries.displayName = sDisplayName;
				newPieSeries.field = sSeriesIdentifier;
				newPieSeries.nameField = "sibkascatlbl09";
				newPieSeries.name = sSeriesIdentifier;
				newPieSeries.id = sSeriesIdentifier;
				arrSeries.push(newPieSeries);		
			}
			cbObject.series = arrSeries;
		}
		
		//============== OK =======================================================================
		public function __removeSeries(sItemID:String, sSeriesIdentifier:String): void
		{	
			var item:ChartBase = (__getChildByName(sItemID) as ChartBase);
			var seriesArray:Array = (item.series as Array);
			var newSeriesArray:Array = new Array();
			
			if(item.className.toString() == "ColumnChart")
			{		
					var popedCSeries:ColumnSeries;
					for(var i:int=seriesArray.length ; i>0 ; i--)
					{
						popedCSeries = seriesArray.pop();
						if(popedCSeries.yField != sSeriesIdentifier)
						{
							newSeriesArray.push(popedCSeries);
						}			
					}
			}
			if(item.className.toString() == "LineChart")
			{
				for(var i:int=seriesArray.length ; i>0 ; i--)
				{
					var popedSeries:LineSeries;
					popedSeries = seriesArray.pop();
					if(popedSeries.yField != sSeriesIdentifier)
					{
						newSeriesArray.push(popedSeries);
					}			
				}
			}
			if(item.className.toString() == "PieChart")
			{
				for(var i:int=seriesArray.length ; i>0 ; i--)
				{
					var popedPSeries:PieSeries;
					popedPSeries = seriesArray.pop();
					if(popedPSeries.field != sSeriesIdentifier)
					{
						newSeriesArray.push(popedPSeries);
					}			
				}	
			}
			item.series = newSeriesArray.reverse();
		}
		
		//============== OK =======================================================================
		public function __removeAllSeries(itemID:String): void
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);
			var newseriesArray:Array = new Array();
			item.series = newseriesArray;
		}
		
		//================ OK =====================================================================
		public function __renameSeries(itemID:String,sSeriesIdentifier:String,sDisplayName:String): void
		{	
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);
			var seriesArray:Array = (item.series as Array);
			var newSeriesArray:Array = new Array();
			if(item.className.toString() == "ColumnChart")
			{		
					var popedCSeries:ColumnSeries;
					for(var i:int=seriesArray.length ; i>0 ; i--)
					{
						popedCSeries = seriesArray.pop();
						if(popedCSeries.yField == sSeriesIdentifier)
						{
							popedCSeries.displayName = 	sDisplayName;
						}
						newSeriesArray.push(popedCSeries);			
					}
			}
			if(item.className.toString() == "LineChart")
			{
				for(var i:int=seriesArray.length ; i>0 ; i--)
				{
					var popedSeries:LineSeries;
					popedSeries = seriesArray.pop();
					if(popedSeries.yField == sSeriesIdentifier)
					{
						popedSeries.displayName = 	sDisplayName;
					}	
					newSeriesArray.push(popedSeries);		
				}
			}
			if(item.className.toString() == "PieChart")
			{
				for(var i:int=seriesArray.length ; i>0 ; i--)
				{
					var popedPSeries:PieSeries;
					popedPSeries = seriesArray.pop();
					if(popedPSeries.field == sSeriesIdentifier)
					{
						popedPSeries.displayName = 	sDisplayName;
					}	
					newSeriesArray.push(popedPSeries);		
				}	
			}
			item.series = newSeriesArray.reverse();
		}
		
		//=============== OK ======================================================================
		public function __setHorizontalAxis(sItemID:String, sCategoryField:String, sDisplayName:String, sTitle:String):void{
			var cbChart:Object       = __getChildByName(sItemID);
			var catAxis:CategoryAxis = new CategoryAxis();
			catAxis.categoryField    = sCategoryField;
			catAxis.displayName      = sDisplayName;
			catAxis.title            = sTitle;
			catAxis.labelFunction    = __getAxisLabel;
			cbChart.horizontalAxis   = catAxis;
		}
		
		public function __setHorizontalAxisLabel(sItemID:String, sTitle:String, sDisplayName:String):void{
			var cbChart:Object       = __getChildByName(sItemID);
			var axis:Object     	 = cbChart.horizontalAxis;
			axis.displayName 		 = sDisplayName;
			axis.title 				 = sTitle;
			cbChart.horizontalAxis   = axis;
		}
		public function __setVerticalAxisLabel(sItemID:String, sTitle:String, sDisplayName:String):void{
			var cbChart:Object       = __getChildByName(sItemID);
			var axis:Object     	 = cbChart.verticalAxis;
			axis.displayName 		 = sDisplayName;
			axis.title 				 = sTitle;
			cbChart.verticalAxis   = axis;
		}
		
		//=============== OK ======================================================================
		public function __addSlideEffecttoSeries(itemID:String, seriesIdentifier:String,duration:Number, direction:String): void
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);	
			var slideEffect:SeriesSlide = new SeriesSlide();
			
			slideEffect.direction = direction;
			slideEffect.duration = duration;
			slideEffect.elementOffset = 0;
			
			var seriesArray:Array = (item.series as Array);		
//			if(item.className.toString() == "ColumnChart")
//			{
			for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newCol:Object = seriesArray[i-1];
						if(newCol.yField == seriesIdentifier)
						{
							newCol.setStyle("showDataEffect",slideEffect);
							newCol.setStyle("hideDataEffect",slideEffect);						
						}			
					}
//			}
/*			if(item.className.toString() == "LineChart")
			{
				for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newLine:LineSeries = (seriesArray[i-1] as LineSeries);
						if(newLine.yField == seriesIdentifier)
						{
							newLine.setStyle("showDataEffect",slideEffect);
							newLine.setStyle("hideDataEffect",slideEffect);					
						}			
					}
			
			}
			if(item.className.toString() == "PieChart")
			{
				for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newPieSeries:PieSeries = (seriesArray[i-1] as PieSeries);
						if(newPieSeries.field == seriesIdentifier)
						{
							newPieSeries.setStyle("showDataEffect",slideEffect);
							newPieSeries.setStyle("hideDataEffect",slideEffect);						
						}			
					}
			
			}*/
		}
		
		//=============== OK ======================================================================
		public function __addZoomEffecttoSeries(itemID:String, seriesIdentifier:String,duration:Number, hrFocus:String, vrFocus:String): void
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);	
			var zoomEffect:SeriesZoom = new SeriesZoom();
			
			zoomEffect.horizontalFocus = hrFocus;
			zoomEffect.verticalFocus = vrFocus;
			zoomEffect.duration = duration;
			zoomEffect.elementOffset = 0;
			zoomEffect.relativeTo = "chart";
			
			var seriesArray:Array = (item.series as Array);	
				
//			if(item.className.toString() == "ColumnChart")
//			{
			for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newCol:Object = seriesArray[i-1];
						if(newCol.yField == seriesIdentifier)
						{
							newCol.setStyle("showDataEffect",zoomEffect);	
							newCol.setStyle("hideDataEffect",zoomEffect);					
						}			
					}
//			}
/*			if(item.className.toString() == "LineChart")
			{
				for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newLine:LineSeries = (seriesArray[i-1] as LineSeries);
						if(newLine.yField == seriesIdentifier)
						{
							newLine.setStyle("showDataEffect",zoomEffect);
							newLine.setStyle("hideDataEffect",zoomEffect);						
						}			
					}
			
			}
			if(item.className.toString() == "PieChart")
			{
				for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newPieSeries:PieSeries = (seriesArray[i-1] as PieSeries);
						if(newPieSeries.field == seriesIdentifier)
						{
							newPieSeries.setStyle("showDataEffect",zoomEffect);		
							newPieSeries.setStyle("hideDataEffect",zoomEffect);						
						}			
					}
			
			}*/
		}
		//=============== OK ======================================================================
		public function __addInterpolateEffecttoSeries(itemID:String, seriesIdentifier:String,duration:Number, direction:String): void
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);	
			var interpolateEffect:SeriesInterpolate = new SeriesInterpolate();
		
			interpolateEffect.duration = duration;
			interpolateEffect.elementOffset = 0;
	
			
			var seriesArray:Array = (item.series as Array);	
				
//			if(item.className.toString() == "ColumnChart")
//			{
			for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newCol:Object = seriesArray[i-1];
						if(newCol.yField == seriesIdentifier)
						{
							newCol.setStyle("showDataEffect",interpolateEffect);				
						}			
					}
//			}
/*			if(item.className.toString() == "LineChart")
			{
				for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newLine:LineSeries = (seriesArray[i-1] as LineSeries);
						if(newLine.yField == seriesIdentifier)
						{
							newLine.setStyle("showDataEffect",interpolateEffect);					
						}			
					}
			
			}
			if(item.className.toString() == "PieChart")
			{
				for(var i:int=seriesArray .length ; i>0 ; i--)
					{
						var newPieSeries:PieSeries = (seriesArray[i-1] as PieSeries);
						if(newPieSeries.field == seriesIdentifier)
						{
							newPieSeries.setStyle("showDataEffect",interpolateEffect);								
						}			
					}
			
			}*/
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		public function __setChartGrid(sCtrlID:String, sType:String, sColor:int = 0, nWeight:Number = 1, nAlpha:Number = .1):void{
			var cbObject:ChartBase = __getChildByName(sCtrlID) as ChartBase;
			for(var i:int; i < cbObject.backgroundElements.length; i++){
				if(cbObject.backgroundElements[i] is GridLines){
					var _arrtemp:Array = new Array;
					_arrtemp.push(sColor);
					(cbObject.backgroundElements[i] as GridLines).setStyle("direction", sType);
					(cbObject.backgroundElements[i] as GridLines).setStyle("verticalStroke",new Stroke(_arrtemp[0], nWeight, nAlpha));
					(cbObject.backgroundElements[i] as GridLines).setStyle("horizontalStroke",new Stroke(_arrtemp[0], nWeight, nAlpha));
				}
			}
		}
		
		public function __showHideChartHrAxisLabels(sCtrlID:String, bShow:Boolean = true):void{
			(__getChildByName(sCtrlID).horizontalAxisRenderer as AxisRenderer).setStyle("showLabels", bShow);
		}
		
		public function __showHideChartVrAxisLabels(sCtrlID:String, bShow:Boolean = true):void{
			(__getChildByName(sCtrlID).verticalAxisRenderer as AxisRenderer).setStyle("showLabels", bShow);
		}
		
		public function __showHideChartHrAxis(sCtrlID:String, bShow:Boolean):void{
			(__getChildByName(sCtrlID).horizontalAxisRenderer as AxisRenderer).visible = bShow;
		}
		
		public function __showHideChartVrAxis(sCtrlID:String, bShow:Boolean):void{
			(__getChildByName(sCtrlID).verticalAxisRenderer as AxisRenderer).visible = bShow;
		}
		
		//=============== OK ======================================================================
		
		public function __addDataPoints(zItemID:Array, zSeries:Array, zX:Array, zY:Array, invalidateChart:Boolean = false):void{
			__zcItemID = __zcItemID.concat(zItemID);
			__zcSeries = __zcSeries.concat(zSeries);
			__zcXField = __zcXField.concat(zX);
			__zcYField = __zcYField.concat(zY);
			if(invalidateChart){
				var iSize:int = __zcItemID.length;
				for(var i:int = 0; i < iSize; i++){
					__addDataPointss(__zcItemID[i], __zcSeries[i], __zcXField[i], __zcYField[i]);
				}
				__zcItemID = new Array;
				__zcSeries = new Array;
				__zcXField = new Array;
				__zcYField = new Array;
				System.gc();
			}
			
		}
		
		public function __addDataPointss(sItemID:String, sSeries:String, nXField:Number, nYField:Number):void{
			var dataPoint:Object = new Object;
			dataPoint["__X__"] = nXField;
			dataPoint[sSeries] = nYField;
			var cb:ChartBase = (__getChildByName(sItemID) as ChartBase);
			if(!cb.dataProvider)
				cb.dataProvider = new ArrayCollection;
			var arrDP:ArrayCollection = (cb.dataProvider as ArrayCollection);
			arrDP.addItem(dataPoint); 
		}
		
		
		/*
		iSeriesType
			0 = Line
			1 = Column
			2 = Area
			3 = Bar
		args
			0 = Color
			1 = alpha
			2 = line thickness
			3 = line form
			4 = argType
			
			argType:
			 	0 = NoSpecify
			 	1 = Color Only
			 	2 = Color and alpha
			 	3 = Color and thickness
			 	4 = Color, alpha, thickness
		*/
		public function __addSeries(sItemID:String, iSeriesType:int, sSeriesIdentifier:String, sDisplayName:String, args:Array): void{//
			var cbObject:ChartBase = __getChildByName(sItemID) as ChartBase;
			var arrSeries:Array = (cbObject.series as Array);
			
			if(iSeriesType == 0){
				var serNewLineSeries:LineSeries = new LineSeries;
				if(args.length == 5){
					if(args[4] != 0){
						if(args[4] == 1)
							serNewLineSeries.setStyle("lineStroke", new Stroke(args[0]));//));
						if(args[4] == 2)
							serNewLineSeries.setStyle("lineStroke", new Stroke(args[0], 1, args[1]));
						if(args[4] == 3)
							serNewLineSeries.setStyle("lineStroke", new Stroke(args[0], args[2], 1));
						if(args[4] == 4)
							serNewLineSeries.setStyle("lineStroke", new Stroke(args[0], args[2], args[1]));
					}
				}
				if(args.length >= 4)
					serNewLineSeries.setStyle("form", args[3]);
				serNewLineSeries.displayName = sDisplayName;
				serNewLineSeries.yField = sSeriesIdentifier;
				serNewLineSeries.name = sSeriesIdentifier;
				serNewLineSeries.interpolateValues = true;
				serNewLineSeries.xField = "__X__";
				arrSeries.push(serNewLineSeries);
			}
			
			if(iSeriesType == 1){
				var serNewColSeries:ColumnSeries = new ColumnSeries;
				if(args.length == 5){
					if(args[4] != 0){
						if(args[4] == 1 || args[4] == 3)
							serNewColSeries.setStyle("fill", new SolidColor(args[0]));
						if(args[4] == 2 || args[4] == 4)
							serNewColSeries.setStyle("fill", new SolidColor(args[0],args[1]));
			
					}
				}
				serNewColSeries.displayName = sDisplayName;
				serNewColSeries.yField = sSeriesIdentifier;
				serNewColSeries.name = sSeriesIdentifier;
				serNewColSeries.xField = "__X__";
				arrSeries.push(serNewColSeries);
			}
			
			if(iSeriesType == 2){
				var serNewAreaSeries:AreaSeries = new AreaSeries;
				if(args.length == 5){
					if(args[4] != 0){
						if(args[4] == 1 || args[4] == 3)
							serNewAreaSeries.setStyle("areaFill", new SolidColor(args[0]));
						if(args[4] == 2|| args[4] == 4)
							serNewAreaSeries.setStyle("areaFill", new SolidColor(args[0],args[1]));
					}
				}
				serNewAreaSeries.setStyle("form", args[3]);
				serNewAreaSeries.displayName = sDisplayName;
				serNewAreaSeries.yField = sSeriesIdentifier;
				serNewAreaSeries.name = sSeriesIdentifier;
				serNewAreaSeries.xField = "__X__";
				arrSeries.push(serNewAreaSeries);
			}
			
			if(iSeriesType == 3){
				var serNewBarSeries:BarSeries = new BarSeries;
				if(args.length == 5){
					if(args[4] != 0){
						if(args[4] == 1 || args[4] == 3)
							serNewBarSeries.setStyle("fill", new SolidColor(args[0]));
						if(args[4] == 2 || args[4] == 4)
							serNewBarSeries.setStyle("fill", new SolidColor(args[0],args[1]));
					}
				}
				serNewBarSeries.displayName = sDisplayName;
				serNewBarSeries.yField = sSeriesIdentifier;
				serNewBarSeries.name = sSeriesIdentifier;
				serNewBarSeries.xField = "__X__";
				arrSeries.push(serNewBarSeries);
			}
			cbObject.series = arrSeries;
		}
		
		public function __showLegend(itemID:String):void
		{
			var item:Legend = (__getChildByName(itemID) as Legend);
			item.visible = true;			
		}
		public function __hideLegend(itemID:String):void
		{
			var item:Legend = (__getChildByName(itemID) as Legend);
			item.visible = false;				
		}
		public function __showDataTip(itemID:String):void	
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);
			item.showDataTips=true;
		}
		public function __hideDataTip(itemID:String):void	
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);
			item.showDataTips=false;
		}
		public function __showAllDataTips(itemID:String):void	
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);
			item.showAllDataTips=true;
		}
		public function __hideAllDataTips(itemID:String):void	
		{
			var item:ChartBase = (__getChildByName(itemID) as ChartBase);
			item.showAllDataTips=false;
		}
		