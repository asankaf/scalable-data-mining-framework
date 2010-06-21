// ActionScript file
import ActionClasses.ActionObject;
import ActionClasses.ActionObjectParent;
import ActionClasses.AlgorithmApriory;
import ActionClasses.AlgorithmClassification;
import ActionClasses.AlgorithmNaiveBayes;
import ActionClasses.CSVDataSource;
import ActionClasses.ClassificationDom;
import ActionClasses.DiscretizeFilter;
import ActionClasses.DrawingEvent;
import ActionClasses.FilterResample;
import ActionClasses.GenerateGraphicalTree;
import ActionClasses.MsSqlDataSource;
import ActionClasses.MySQLDataSource;
import ActionClasses.Path;
import ActionClasses.RangeSplitter;
import ActionClasses.TextViewer;
import ActionClasses.TreeViewer;
import ActionClasses.Util;
import ActionClasses.WAHCompression;
import ActionClasses.WAHCompression2;
import ActionClasses.XML_Loader_;

import com.dncompute.graphics.ArrowStyle;
import com.dncompute.graphics.GraphicsUtil;

import flash.display.Shape;
import flash.events.Event;
import flash.events.MouseEvent;
import flash.utils.Dictionary;

import mx.charts.series.LineSeries;
import mx.collections.ArrayCollection;
import mx.containers.Canvas;
import mx.controls.Image;
import mx.controls.ProgressBar;
import mx.core.DragSource;
import mx.core.IFlexDisplayObject;
import mx.events.DragEvent;
import mx.managers.DragManager;
import mx.managers.PopUpManager;
import mx.utils.ObjectProxy;

import seedminer.AprioryConfigPopUp;
import seedminer.ControlPanel;
import seedminer.ExecutingMarkPopUp;
import seedminer.GraphViewObject;
import seedminer.GraphViewPop;
import seedminer.LoopConfigure;
import seedminer.MsSQLDataSourcesSelectPopUp;
import seedminer.ProgressBarComponent;
import seedminer.SaveFlowButtonObject;
import seedminer.SavedFlow;
import seedminer.Sink;
import seedminer.SpacePopUp;
import seedminer.TimePopUp;
import seedminer.XMLLoaderConfigPopUp;


include "bridge/FlexVCBridge.as";

private var actionObj:ActionObject;
private var correctionX:Number;
private var correctionY:Number;

private var actionObjectsOnCanvas:Dictionary = new Dictionary();
private var actionObjectSequence:Array = new Array();
private var arrowsOnCanvas:Array = new Array();

private var tempLine:Shape;
private var arrowColour:uint=0x919191;
private var fillColour:uint=0xdad8d8;
private static var ok:Boolean=false;
private var procedurePara:String;
private var algoPara:String;
private var DONE:String="Done";
private var EXECUTING:String="Executing Flow";
private var timeStampsOnCanvas:Dictionary = new Dictionary();
private var timeStamps:Array = new Array();
private var controlPanel:ControlPanel;
private var sink:Sink;
private var loopConfig:LoopConfigure;
private var rememberGraphName:String;
private var c:int=0;

private var mysqlObject:MySQLDataSource;
private var mssqlObject:MsSqlDataSource;
private var executingMask:ExecutingMarkPopUp;
private var progress_Bar:ProgressBar;
private var sequenceNumber:int=0;
private var rowsToLoad:int=10000;
private var compress_unit_KB:String="KB";
private var small_compress_unit_B:String="B";
private var compress_unit_MB:String="MB";

private var flowID:int=0;
private var savedFlows:ArrayCollection=new ArrayCollection();


public function startUp(event:Event):void
{
	
}
public function createControlPanel(event:Event):void
{
	controlPanel=ControlPanel(PopUpManager.createPopUp(this, ControlPanel , false));
	controlPanel.x=this.drawingcanvas.x+6;
	controlPanel.y=this.drawingcanvas.y+70;
	controlPanel.executeButton.addEventListener(MouseEvent.CLICK,executeFlow);
	controlPanel.saveButton.addEventListener(MouseEvent.CLICK,saveCurrentFlow);
	controlPanel.clearCanvasButton.addEventListener(MouseEvent.CLICK,clearCanvas);
	controlPanel.createLoopButton.addEventListener(MouseEvent.CLICK,showLoopConfigureWindow);
	controlPanel.measureTimeButton.addEventListener(MouseEvent.CLICK,measureTime);
	
	sink=Sink(PopUpManager.createPopUp(this, Sink , false));
	sink.canvas_Main=canvasmain;
	sink.clearSavedGraphs.addEventListener(MouseEvent.CLICK,sink.clearGraphs);
	sink.clearSavedFlaws.addEventListener(MouseEvent.CLICK,sink.clearFlows);
	sink.comparegraphs.addEventListener(MouseEvent.CLICK,sink.compareAllGraphsInOneGraph);
	sink.x=this.drawingcanvas.width-sink.width-8;
	sink.y=this.drawingcanvas.y+70;
}

private function saveCurrentFlow(event:MouseEvent):void
{
	var actionObjects:Array = new Array();
    for (var actionObj:Object in actionObjectsOnCanvas)
    {
        actionObjects.push(actionObj);
    }
    var numberOfActionObjects:int = actionObjects.length;
    
	if(0<numberOfActionObjects)
	{
		var saveFlow:SavedFlow=new SavedFlow(actionObjectsOnCanvas,actionObjectSequence,arrowsOnCanvas);
		var saveFlowButton:SaveFlowButtonObject=new SaveFlowButtonObject();
		saveFlowButton.label=getNameOfSavingFlow(actionObjectsOnCanvas);
		saveFlowButton.addEventListener(MouseEvent.CLICK,recreateFlow);
		saveFlowButton.flow=saveFlow;
		saveFlowButton.flowID=flowID;
		sink.saveFlaws_box.addChild(saveFlowButton);
		flowID++;
	}
}

private function getNameOfSavingFlow(actionObjectsOnCanvas_:Dictionary):String
{
	var nameOfFlow:String="";
	var actionObjects:Array = new Array();
    for (var actionObj:Object in actionObjectsOnCanvas_)
    {
        actionObjects.push(actionObj);
    }
    var numberOfActionObjects:int = actionObjects.length;
    
    for(var j:int=0;j<numberOfActionObjects;j++)
	{
		var Obj:ActionObject=ActionObject(actionObjectsOnCanvas_[actionObjects[j]]);
		if(Obj.type()==ActionObjectParent.CSV_DATASOURCE)
		{			
			nameOfFlow+="csv";		
		}
		else if(Obj.type()==ActionObjectParent.MySQL_DATASOURCE)
		{					
			nameOfFlow+="mysql";
		}
		else if(Obj.type()==ActionObjectParent.XML_LOADER)
		{			
			nameOfFlow+="xml";
		}
		else if(Obj.type()==ActionObjectParent.MSSQL_DATASOURCE)
		{
			nameOfFlow+="mssql";
		}
		else if(Obj.type()==ActionObjectParent.FILTER_RESAMPLE)
		{
			nameOfFlow+="removeNull";
		}	
		else if(Obj.type()==ActionObjectParent.DESCRITIZE)
		{
			nameOfFlow+="descritize";
		}
		else if(Obj.type()==ActionObjectParent.RANGESPLITE)
		{
			nameOfFlow+="rangesplite";
		}
		else if(Obj.type()==ActionObjectParent.WAH_COMPRESSTION)
		{
			nameOfFlow+="wah";
		}
		else if(Obj.type()==ActionObjectParent.WAH_COMPRESSTION_2)
		{
			nameOfFlow+="ewah";
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_APRIORY)
		{					
			nameOfFlow+="apriory";
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_CLASSIFICATION)
		{
			nameOfFlow+="classification";
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_NAIVEBAYES)
		{
			nameOfFlow+="naiveBayes";
		}
		else if(Obj.type()==ActionObjectParent.TEXT_VIEWER)
		{
			nameOfFlow+="text";
		}
		else if(Obj.type()==ActionObjectParent.TREE_VIEWER)
		{
			nameOfFlow+="tree";
		}	
		if(j+1!=numberOfActionObjects)
		{
			nameOfFlow+="->";
		}
	}
	return nameOfFlow;
}

private function recreateFlow(event:MouseEvent):void
{
	clearCanvas(null);
	var sf:SavedFlow=SaveFlowButtonObject(event.currentTarget).flow;
	//trace(SaveFlowButtonObject(event.currentTarget).flow);
	createFlow(sf);
}

private function measureTime(event:MouseEvent):void
{
	if(controlPanel.loopFlaw==true)
	{
		loopConfig.hide();
		controlPanel.toggleLoop();
		PopUpManager.removePopUp(loopConfig as IFlexDisplayObject);
	}
	controlPanel.toggleMeasureTime();
}

private function showLoopConfigureWindow(event:MouseEvent):void
{
	if(controlPanel.measuretime==true)
	{
		controlPanel.toggleMeasureTime();
	}
	
	if(loopConfig==null)
	{
		loopConfig=LoopConfigure(PopUpManager.createPopUp(drawingcanvas, LoopConfigure , false));
		loopConfig.show();
		controlPanel.toggleLoop();
		loopConfig.width=drawingcanvas.width;
		loopConfig.x=drawingcanvas.x;
		loopConfig.y=drawingcanvas.y;
	}
	else
	{
		if(!loopConfig.runInALoop)
		{
			loopConfig.show();
			controlPanel.toggleLoop();
			PopUpManager.addPopUp(loopConfig as IFlexDisplayObject,drawingcanvas);
		}
		else if(loopConfig.runInALoop)
		{
			loopConfig.hide();
			controlPanel.toggleLoop();
			PopUpManager.removePopUp(loopConfig as IFlexDisplayObject);
		}
	}
	
}

private function addTimeStamp(actionObject:ActionObjectParent,timeValue:String):void
{
	var timeStamp:TimePopUp;
	if(actionObject.timeStamp==null)
	{
		timeStamp=new TimePopUp();
		actionObject.addTimeStamp(timeStamp);
	}
	else
	{
		timeStamp=actionObject.timeStamp;
	}	
	timeStamp.myLable.text=timeValue;
}

private function addSpaceStamps(actionObject:ActionObjectParent,spaceValues:String):void
{
	var spaceStamp:SpacePopUp;
	if(actionObject.spaceStamps==null)
	{
		spaceStamp=new SpacePopUp();
		actionObject.addSpaceStamp(spaceStamp);
	}
	else
	{
		spaceStamp=actionObject.spaceStamps;
	}	
	var strs:Array=spaceValues.split("^^");
	var numbers_:int=String(strs[0]).length;
	if(8<numbers_)
	{
		spaceStamp.space_compression.text=(int(parseInt(strs[0])/(1024*1024))).toString()+" "+compress_unit_MB+" → "+(int(parseInt(strs[1])/(1024*1024))).toString()+" "+compress_unit_MB;
	}
	else if(4<numbers_)
	{
		spaceStamp.space_compression.text=(int(parseInt(strs[0])/1024)).toString()+" "+compress_unit_KB+" → "+(int(parseInt(strs[1])/1024)).toString()+" "+compress_unit_KB;
	}
	else
	{
		spaceStamp.space_compression.text=strs[0]+" "+small_compress_unit_B+" → "+strs[1]+" "+small_compress_unit_B;
	}
}

public function cplusPluseCallBackFunction(str:String):void
{	
	//Alert.show(str);
	var compressData:String="";
	var strs:Array=str.split("^&&");
	if(1<strs.length)
	{
		str="";
		compressData=strs[1];
		for(var mm:int;mm<strs.length;mm++)
		{
			if(mm==1)
			{
				continue;
			}
			str+=strs[mm];
		}
		for(var jj:int=0;jj<actionObjectSequence.length;jj++)
		{
			var actionObject_:ActionObjectParent=ActionObjectParent(actionObjectsOnCanvas[actionObjectSequence[jj]]);
			if(actionObject_.type()==ActionObjectParent.WAH_COMPRESSTION || actionObject_.type()==ActionObjectParent.WAH_COMPRESSTION_2)
			{
				addSpaceStamps(actionObject_,compressData);				
			}
		}		
	}
	//Alert.show(str);
	var strings:Array=str.split("##");//get view
	var view:String=strings[0];
	
	var strings1:Array=strings[1].split("$$");//get time stamps
	if(1<strings1.length)
	{
		var timeInfo:String=strings1[1];
		var strings2:Array=timeInfo.split("@@");
		if(1<strings2.length)
		{
			var procedure:Array=String(strings2[0]).split("->");
			 
			///////////////////////////////////////time//////////////////////////////
			for(var i:int=0;i<procedure.length;i++)
			{
				var currentProcedure:String=procedure[i];
				for(var j:int=0;j<actionObjectSequence.length;j++)
				{
					var actionObject:ActionObjectParent=ActionObjectParent(actionObjectsOnCanvas[actionObjectSequence[j]]);
					if(actionObject.type()==ActionObjectParent.CSV_DATASOURCE && currentProcedure=="csv")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.MySQL_DATASOURCE && currentProcedure=="mysql")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.XML_LOADER && currentProcedure=="xml")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.MSSQL_DATASOURCE && currentProcedure=="mssql")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.FILTER_RESAMPLE && currentProcedure=="removeNull")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.DESCRITIZE && currentProcedure=="descritize")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.RANGESPLITE && currentProcedure=="rangesplite")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.WAH_COMPRESSTION && currentProcedure=="wah")
					{
						addTimeStamp(actionObject,strings2[i+1]);					
					}
					else if(actionObject.type()==ActionObjectParent.WAH_COMPRESSTION_2 && currentProcedure=="ewah")
					{
						addTimeStamp(actionObject,strings2[i+1]);					
					}
					else if(actionObject.type()==ActionObjectParent.ALGORITHM_APRIORY && currentProcedure=="apriory")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}				
					else if(actionObject.type()==ActionObjectParent.ALGORITHM_CLASSIFICATION && currentProcedure=="classification")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}
					else if(actionObject.type()==ActionObjectParent.ALGORITHM_NAIVEBAYES && currentProcedure=="naiveBayes")
					{
						addTimeStamp(actionObject,strings2[i+1]);
					}					
				}
			}	
		}
		
	}
	else
	{
		for(var k:int=0;k<actionObjectSequence.length;k++)
		{
			var actionObject__:ActionObjectParent=ActionObjectParent(actionObjectsOnCanvas[actionObjectSequence[k]]);
			actionObject__.removeTimeStamp();
		}				
	}
	
	///////////////////////////////////////view////////////////////////////////////
	if(view=="textViewer")
	{
		var data:String=strings1[0];
		var textPopUp:TEXTViewPopUp=TEXTViewPopUp(PopUpManager.createPopUp(this, TEXTViewPopUp , true));
		var startindex:int=String(data).search("\n");
		textPopUp.textViewerTextArea.text=data;
		var point1:Point = new Point();
		point1.x=0;
	    point1.y=0;                
	    textPopUp.x=canvasmain.width/2-textPopUp.width/2;
	    textPopUp.y=canvasmain.height/2-textPopUp.height/2;
	}
	
	else if(view=="treeViewer")
	{
		
		var treeString:String=strings1[0];
		var dom:ClassificationDom=new ClassificationDom(treeString);
		
	    var treePopUp:TreeViewPopUp=TreeViewPopUp(PopUpManager.createPopUp(this, TreeViewPopUp , true));
	    var point1:Point = new Point();
		point1.x=0;
	    point1.y=0;                
	    treePopUp.x=canvasmain.width/2-treePopUp.width/2;
	    treePopUp.y=canvasmain.height/2-treePopUp.height/2;
	    var genTree:GenerateGraphicalTree=new GenerateGraphicalTree(dom,treePopUp);
	}	
	else if(view=="mysqlDataSourcesList")
	{
		var sqlsourcelist:String=strings1[0];		
		var dataSourcesList:XMLList= new XMLList(sqlsourcelist);
				
		MySqlDataSourcesSelectPopUp(mysqlObject.config).dataSoucesCol.dataField="name";
		MySqlDataSourcesSelectPopUp(mysqlObject.config).dg.dataProvider=dataSourcesList;
	}
	else if(view=="mssqlDataSourcesList")
	{
		var mssqlsourcelist:String=strings1[0];		
		var msdataSourcesList:XMLList= new XMLList(mssqlsourcelist);
				
		MsSQLDataSourcesSelectPopUp(mssqlObject.config).dataSoucesCol.dataField="name";
		MsSQLDataSourcesSelectPopUp(mssqlObject.config).dg.dataProvider=msdataSourcesList;
	}
	else if(view=="graph")
	{
		var graphData:String=strings1[0];
		var dataXMLList:XMLList= new XMLList(graphData);	
		var a:Array = xmlListToObjectArray(dataXMLList.children());
        var ac:ArrayCollection = new ArrayCollection(a);
        var displayCleanName:String=getGraphName(rememberGraphName);
        addGraphToSink(dataXMLList.children(),rememberGraphName);
        var graphPopUp:GraphViewPop=GraphViewPop(PopUpManager.createPopUp(this, GraphViewPop , true));
        graphPopUp.dataCollection=ac;
        
        var mySeries:Array=new Array();
        var series1:LineSeries = new LineSeries();
        series1.yField=rememberGraphName;        
        series1.displayName=displayCleanName;
        sink.graphNames.push(rememberGraphName);
        mySeries.push(series1);
		graphPopUp.graphLineChart.series= mySeries;
        
        var point1:Point = new Point();
        point1.x=0;
	    point1.y=0;                
	    graphPopUp.x=canvasmain.width/2-graphPopUp.width/2;
	    graphPopUp.y=canvasmain.height/2-graphPopUp.height/2;
	}
	else if(view=="errorProcedure")
	{
		showError("Invalid flaw! Please create a new valid flaw to execute...");
		clearCanvas(new MouseEvent(Event.CHANGE));
	}
	else if(view=="exceptionThrown")
	{
		showError("Exception occured!"+strings[1]);
		//clearCanvas(new MouseEvent(Event.CHANGE));
	}
	else if(view=="nullView")
	{
	}
	
	showStatus(DONE);
}

private function getGraphName(GraphName:String):String
{
	var strs:Array=GraphName.split("_");
	var newName:String="";
	for(var i:int=0;i<strs.length-1;i++)
	{
		newName+=strs[i];
		if(strs.length-2 != i)
		{
			newName+="_";
		}		
	}
	return newName;
}

private function addGraphToSink(xmlList:XMLList,GraphName:String):void
{
	sink.graphCount++;
	var graphButton:GraphViewObject=new GraphViewObject();
	graphButton.addEventListener(MouseEvent.CLICK,graphButton.drawThisGraph);
	graphButton.canvas_Main=canvasmain;
	graphButton.graphName=GraphName;
	var a:Array = xmlListToObjectArray(xmlList);
	var ac:ArrayCollection = new ArrayCollection(a);
	graphButton.graph=ac;
	graphButton.label=" : "+getGraphName(GraphName);
	sink.saveGraphs.addChild(graphButton);
	sink.graphs.addItem(xmlList);
}

private function xmlListToObjectArray(xmlList:XMLList):Array
{
    var a:Array = new Array();
    for each(var xml:XML in xmlList)
    {
        var attributes:XMLList = xml.attributes();
        var o:Object = new Object();
        for each (var attribute:XML in attributes)
        {
            var nodeName:String = attribute.name().toString();
            if(nodeName!="datasize")
            {
            	rememberGraphName=nodeName; 	
            }
            var value:*;          
            value = attribute.toString();
                
            o[nodeName] = value;
        }
        
        a.push(new ObjectProxy(o));
    }
    sequenceNumber++;
    return a;
}

private function showError(str:String):void
{
	var errorpop:ErrorPopUp=ErrorPopUp(PopUpManager.createPopUp(this, ErrorPopUp , false));
	errorpop.message.text=str;
	var point1:Point = new Point();
	point1.x=0;
    point1.y=0;                
    errorpop.x=canvasmain.width/2-errorpop.width/2;
    errorpop.y=canvasmain.height/2-errorpop.height/2;
}

private function executeFlow(event:Event):void
{
	if(1<actionObjectSequence.length)
	{
		showStatus(EXECUTING);
		
		var ret:Object = new Object();
		

		ret["flashId"] = __flashPlayerId;
		ret["flashIndex"] = __flashPlayerIndex;
		var iid:Array = event.currentTarget.toString().split(".");
		var itemId:String = iid[iid.length - 1];
		ret["itemId"] = itemId;
		ret["itemType"] = "Button";
		ret["eventType"] = event.type.toString();
		
		if(getCurrentProcedure()==null)//validate procedure
		{
			showStatus("Done");
			return;
		}
			
		ret["procedure"] = getCurrentProcedure();
		ret["procedurePara"] = procedurePara;
		ret["algoPara"] = algoPara;
		
		ret["measureTime"] = controlPanel.measuretime.toString();
		if(!controlPanel.loopFlaw)
		{
			ret["runInALoop"] = controlPanel.loopFlaw.toString()+"@@"+"1"+"@@"+"1000";
		}
		else if(controlPanel.loopFlaw)
		{
			ret["runInALoop"] = controlPanel.loopFlaw.toString()+"@@"+loopConfig.loopCount.text+"@@"+loopConfig.increment.text;
		}	
	
		__callBackFunction.call(fabridge,ret);
		//var str:String="treeViewer##outlook = sunny\n|   humidity = high: no (3.0)\n|   humidity = normal: yes (2.0)\noutlook = overcast: yes (4.0)\noutlook = rainy\n|   windy = TRUE: no (2.0)\n|   windy = FALSE: yes (3.0)";
		//var str:String="treeViewer##1 = 0\n|   2 = 0: 0 (186/1)\n|   2 = 1\n|   |   0 = 0: 0 (4)\n|   |   0 = 1: 1 (3)\n|   2 = 2: 0 (61)\n1 = 1\n|   0 = 0\n|   |   1 = 0\n|   |   |   0 = 0: 0 (7)\n|   |   |   0 = 1\n|   |   |   |   0 = 0: 1 (49/1)\n|   |   |   |   0 = 1: 0 (3)\n|   |   |   |   0 = 2: 1 (0)\n|   |   1 = 1: 0 (39/1)\n|   |   1 = 2: 0 (14)\n|   0 = 1: 2 (9/1)";
		//var str:String="treeViewer##children = 0\n|   save_act = NO: YES (48)\n|   save_act = YES: NO (240)\nchildren = 1: YES (144)\nchildren = 2\n|   car = NO: YES (48)\n|   car = YES: NO (96)\nchildren = 3: NO (96)";
		//var str:String="treeViewer##petalwidth <= 0.6: Iris-setosa (50.0)\npetalwidth > 0.6\n|   petalwidth <= 1.7\n|   |   petallength <= 4.9: Iris-versicolor (48.0/1.0)\n|   |   petallength > 4.9\n|   |   |   petalwidth <= 1.5: Iris-virginica (3.0)\n|   |   |   petalwidth > 1.5: Iris-versicolor (3.0/1.0)\n|   petalwidth > 1.7: Iris-virginica (46.0/1.0)";
		//var str:String="noView##petalwidth <= 0.6: 6.0/1.0)";
		//var str:String="textViewer##asdfsasdf\nasdf\n$$mysql->text@@50ms@@10ms";
		//var str:String="textViewer##asdfsasdf\nasdf";
		//var str:String="nullView##^&&3000^^5000^&&";
		//var str:String="treeViewer##plant = :  (5)/nplant = lt-normal: gt-norm (8320/468)/nplant = normal: norm (1635/39)/nplant = plant-stand: precip (39)$$csv->classification->tree@@1 s@@0 s";
		/*if(c==0)
		{
			c++;
			//var str:String='graph##<items><item month=\"1000\" csv=\"10\" /><item month=\"2000\" csv=\"4\" /><item month="3000" csv=".7" /><item month="4000" csv="85" /><item month="5000" csv="14" /></items>';
			var str:String="graph##<items><item datasize=\"100\" csv_text=\"213\"/><item datasize=\"200\" csv_text=\"540\"/><item datasize=\"300\" csv_text=\"587\"/><item datasize=\"400\" csv_text=\"0\"/><item datasize=\"500\" csv_text=\"0\"/><item datasize=\"600\" csv_text=\"220\"/><item datasize=\"700\" csv_text=\"0\"/><item datasize=\"800\" csv_text=\"0\"/><item datasize=\"900\" csv_text=\"20\"/><item datasize=\"1000\" csv_text=\"40\"/></items>";
		}
		else
		{
			c=0;
			var str:String='graph##<items><item datasize=\"1000\" mysql=\"60\" /><item datasize=\"2000\" mysql=\"50\" /><item datasize="3000" mysql=".7" /><item datasize="4000" mysql="2.3" /><item datasize="5000" mysql="3.1" /></items>';
		}*/
		//var str:String="nullView##^&&3000^^5000^&&";
		/*var str:String="treeViewer##int-discolor = absent\n|   plant-stand = lt-normal: herbicide-injury (12)\n|   plant-stand = normal\n|   |   leaf-malf = absent: cyst-nematode (20)\n|   |   leaf-malf = present: 2-4-d-injury (17)\nint-discolor = black: charcoal-rot (30)\nint-discolor = brown: brown-stem-rot (64)\nint-discolor = none\n|   leafspot-size = dna\n"+
"|   |   canker-lesion = brown\n|   |   |   leaves = abnorm\n|   |   |   |   fruit-spots = absent: anthracnose (5)\n|   |   |   |   fruit-spots = brown-w/blk-specks: anthracnose (10)\n|   |   |   |   fruit-spots = colored: diaporthe-stem-canker (0)\n|   |   |   |   fruit-spots = dna: diaporthe-stem-canker (21/1)\n"+
"|   |   |   leaves = norm: rhizoctonia-root-rot (29)\n|   |   canker-lesion = dk-brown-blk\n|   |   |   plant-growth = abnorm\n|   |   |   |   plant-stand = lt-normal\n|   |   |   |   |   fruiting-bodies = absent: phytophthora-rot (128)\n|   |   |   |   |   fruiting-bodies = present: anthracnose (4)\n|   |   |   |   plant-stand = normal: anthracnose (8)\n|   |   |   plant-growth = norm: anthracnose (37)\n|   |   canker-lesion = dna\n|   |   |   plant-growth = abnorm: diaporthe-stem-canker (20)\n|   |   |   plant-growth = norm\n|   |   |   |   leaves = abnorm: powdery-mildew (30)\n"+
"|   |   |   |   leaves = norm: diaporthe-pod-&-stem-blight (21)\n|   |   canker-lesion = tan: purple-seed-stain (14)\n|   leafspot-size = gt-1/8\n|   |   leaf-mild = absent\n|   |   |   fruit-pods = diseased\n|   |   |   |   external-decay = absent\n|   |   |   |   |   canker-lesion = brown: brown-spot (2)\n|   |   |   |   |   canker-lesion = dk-brown-blk: frog-eye-leaf-spot (2)\n|   |   |   |   |   canker-lesion = dna: frog-eye-leaf-spot (2)\n|   |   |   |   |   canker-lesion = tan: frog-eye-leaf-spot (0)\n|   |   |   |   external-decay = firm-and-dry: frog-eye-leaf-spot (86)\n|   |   |   |   external-decay = watery: frog-eye-leaf-spot (0)\n|   |   |   fruit-pods = dna: brown-spot (0)\n|   |   |   fruit-pods = few-present: brown-spot (0)\n|   |   |   fruit-pods = norm\n"+
"|   |   |   |   leaf-malf = absent\n|   |   |   |   |   fruiting-bodies = absent\n|   |   |   |   |   |   date = ?: alternarialeaf-spot (0)\n|   |   |   |   |   |   date = april: brown-spot (7)\n|   |   |   |   |   |   date = august\n|   |   |   |   |   |   |   plant-growth = abnorm: frog-eye-leaf-spot (3)\n|   |   |   |   |   |   |   plant-growth = norm\n|   |   |   |   |   |   |   |   leaf-shread = absent\n|   |   |   |   |   |   |   |   |   seed = abnorm: alternarialeaf-spot (3)\n|   |   |   |   |   |   |   |   |   seed = norm\n|   |   |   |   |   |   |   |   |   |   hail = no: frog-eye-leaf-spot (3)\n|   |   |   |   |   |   |   |   |   |   hail = yes\n|   |   |   |   |   |   |   |   |   |   |   plant-stand = lt-normal: alternarialeaf-spot (12/2)\n|   |   |   |   |   |   |   |   |   |   |   plant-stand = normal\n|   |   |   |   |   |   |   |   |   |   |   |   seed-tmt = fungicide: frog-eye-leaf-spot (9)\n|   |   |   |   |   |   |   |   |   |   |   |   seed-tmt = none\n|   |   |   |   |   |   |   |   |   |   |   |   |   germination = 80-89\n"+
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   crop-hist = diff-lst-year: alternarialeaf-spot (0)\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   crop-hist = same-lst-sev-yrs: alternarialeaf-spot (2)\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   crop-hist = same-lst-two-yrs\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   area-damaged = ?: alternarialeaf-spot (0)\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   area-damaged = low-areas: alternarialeaf-spot (0)\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   area-damaged = scattered: frog-eye-leaf-spot (2)\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   area-damaged = upper-areas: alternarialeaf-spot (3)\n|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   area-damaged = whole-field: alternarialeaf-spot (0)\n"+
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   crop-hist = same-lst-yr: frog-eye-leaf-spot (2)\n|   |   |   |   |   |   |   |   |   |   |   |   |   germination = 90-100: alternarialeaf-spot (4)\n|   |   |   |   |   |   |   |   |   |   |   |   |   germination = lt-80: alternarialeaf-spot (2)\n|   |   |   |   |   |   |   |   |   |   |   |   seed-tmt = other: frog-eye-leaf-spot (0)\n|   |   |   |   |   |   |   |   leaf-shread = present: alternarialeaf-spot (4)\n|   |   |   |   |   |   date = july\n|   |   |   |   |   |   |   precip = gt-norm\n|   |   |   |   |   |   |   |   area-damaged = ?: frog-eye-leaf-spot (0)\n|   |   |   |   |   |   |   |   area-damaged = low-areas: brown-spot (3/1)\n|   |   |   |   |   |   |   |   area-damaged = scattered\n|   |   |   |   |   |   |   |   |   crop-hist = diff-lst-year: alternarialeaf-spot (5/1)\n|   |   |   |   |   |   |   |   |   crop-hist = same-lst-sev-yrs: frog-eye-leaf-spot (2)\n|   |   |   |   |   |   |   |   |   crop-hist = same-lst-two-yrs: frog-eye-leaf-spot (1)\n|   |   |   |   |   |   |   |   |   crop-hist = same-lst-yr: frog-eye-leaf-spot (1)\n|   |   |   |   |   |   |   |   area-damaged = upper-areas: frog-eye-leaf-spot (3)\n|   |   |   |   |   |   |   |   area-damaged = whole-field: brown-spot (2)\n|   |   |   |   |   |   |   precip = lt-norm: phyllosticta-leaf-spot (2)\n|   |   |   |   |   |   |   precip = norm: phyllosticta-leaf-spot (4)\n|   |   |   |   |   |   date = june\n|   |   |   |   |   |   |   precip = gt-norm: brown-spot (31)\n|   |   |   |   |   |   |   precip = lt-norm: phyllosticta-leaf-spot (6)\n|   |   |   |   |   |   |   precip = norm\n"+
"|   |   |   |   |   |   |   |   hail = no: brown-spot (3)\n|   |   |   |   |   |   |   |   hail = yes: phyllosticta-leaf-spot (4/1)\n|   |   |   |   |   |   date = may: brown-spot (36/1)\n|   |   |   |   |   |   date = october: alternarialeaf-spot (44/1)\n|   |   |   |   |   |   date = september\n|   |   |   |   |   |   |   stem = abnorm: frog-eye-leaf-spot (3)\n|   |   |   |   |   |   |   stem = norm\n|   |   |   |   |   |   |   |   temp = gt-norm: alternarialeaf-spot (37)\n|   |   |   |   |   |   |   |   temp = lt-norm: alternarialeaf-spot (0)\n|   |   |   |   |   |   |   |   temp = norm\n|   |   |   |   |   |   |   |   |   leaf-shread = absent\n|   |   |   |   |   |   |   |   |   |   crop-hist = diff-lst-year: alternarialeaf-spot (5)\n|   |   |   |   |   |   |   |   |   |   crop-hist = same-lst-sev-yrs: frog-eye-leaf-spot (4)\n|   |   |   |   |   |   |   |   |   |   crop-hist = same-lst-two-yrs: alternarialeaf-spot (4)\n|   |   |   |   |   |   |   |   |   |   crop-hist = same-lst-yr\n|   |   |   |   |   |   |   |   |   |   |   germination = 80-89: alternarialeaf-spot (1)\n|   |   |   |   |   |   |   |   |   |   |   germination = 90-100: frog-eye-leaf-spot (2)\n|   |   |   |   |   |   |   |   |   |   |   germination = lt-80: frog-eye-leaf-spot (2)\n|   |   |   |   |   |   |   |   |   leaf-shread = present: alternarialeaf-spot (8)\n|   |   |   |   |   fruiting-bodies = present: brown-spot (49)\n|   |   |   |   leaf-malf = present: phyllosticta-leaf-spot (14)\n|   |   leaf-mild = lower-surf: downy-mildew (30)\n|   |   leaf-mild = upper-surf: brown-spot (0)\n|   leafspot-size = lt-1/8\n|   |   canker-lesion = brown: bacterial-blight (0)\n|   |   canker-lesion = dk-brown-blk: bacterial-blight (0)\n|   |   canker-lesion = dna\n|   |   |   leafspots-marg = dna: bacterial-blight (0)\n|   |   |   leafspots-marg = no-w-s-marg: bacterial-pustule (26)\n|   |   |   leafspots-marg = w-s-marg\n|   |   |   |   seed-size = lt-norm: bacterial-pustule (3)\n|   |   |   |   seed-size = norm: bacterial-blight (31/1)\n|   |   canker-lesion = tan: purple-seed-stain (16)";
		//getCurrentProcedure();
		//var str:String="<mysqlsource><name>source1</name></mysqlsource><mysqlsource><name>source2</name></mysqlsource>";
		cplusPluseCallBackFunction(str);*/
	
	}	
}

private function getCurrentProcedure():String
{
	var procedure:String="";
	procedurePara="";
	for(var i:int=0;i<actionObjectSequence.length;i++)
	{
		var Obj:ActionObjectParent=ActionObjectParent(actionObjectsOnCanvas[actionObjectSequence[i]]);
		if(Obj.type()==ActionObjectParent.CSV_DATASOURCE)
		{
			if(CSVConfigPopUp(Obj.config).location.text=="")
			{
				showError("Path not configured!\nEnter the path of the CSV file...");
				return null;
			}
			else
			{
				procedure+="csv";
				procedurePara=CSVConfigPopUp(Obj.config).location.text.toString()+"@@"+CSVConfigPopUp(Obj.config).csv_data_size.text.toString();
			}			
		}
		else if(Obj.type()==ActionObjectParent.MySQL_DATASOURCE)
		{			
			if(MySqlDataSourcesSelectPopUp(Obj.config).dg.selectedIndex<0 && MySqlDataSourcesSelectPopUp(Obj.config).mysql_query.text=="" && MySqlDataSourcesSelectPopUp(Obj.config).mysql_data_size.text=="")
			{
				showError("Query not configured!\nEnter the query for MYSQL data source...");
				return null;
			}
			else
			{
				procedure+="mysql";
				var st:Array=MySqlDataSourcesSelectPopUp(Obj.config).dg.selectedItem.toString().split("<name>");
				var stt:Array=String(st[1]).split("</name>");
				procedurePara=String(stt[0])+"@@"+MySqlDataSourcesSelectPopUp(Obj.config).mysql_query.text.toString()+"@@"+MySqlDataSourcesSelectPopUp(Obj.config).mysql_data_size.text.toString();
			}
		}
		else if(Obj.type()==ActionObjectParent.XML_LOADER)
		{
			if(XMLLoaderConfigPopUp(Obj.config).xml_metadata_location.text=="" && XMLLoaderConfigPopUp(Obj.config).xml_data_location.text=="" && XMLLoaderConfigPopUp(Obj.config).xml_data_source.text=="")
			{
				showError("Paths not configured!\nEnter the paths of the XML files...");
				return null;
			}
			else
			{
				procedure+="xml";
				procedurePara=XMLLoaderConfigPopUp(Obj.config).xml_metadata_location.text.toString()+"@@"+XMLLoaderConfigPopUp(Obj.config).xml_data_location.text.toString()+"@@"+XMLLoaderConfigPopUp(Obj.config).xml_data_source.text.toString()+"@@"+XMLLoaderConfigPopUp(Obj.config).xml_data_size.text.toString();
			}
		}
		else if(Obj.type()==ActionObjectParent.MSSQL_DATASOURCE)
		{
			if(MsSQLDataSourcesSelectPopUp(Obj.config).dg.selectedIndex<0 && MsSQLDataSourcesSelectPopUp(Obj.config).mssql_query.text=="" && MsSQLDataSourcesSelectPopUp(Obj.config).mssql_data_size.text=="")
			{
				showError("Query not configured!\nEnter the query for MSSQL data source...");
				return null;
			}
			else
			{
				procedure+="mssql";
				var sa:Array=MsSQLDataSourcesSelectPopUp(Obj.config).dg.selectedItem.toString().split("<name>");
				var saa:Array=String(sa[1]).split("</name>");
				procedurePara=String(saa[0])+"@@"+MsSQLDataSourcesSelectPopUp(Obj.config).mssql_query.text.toString()+"@@"+MsSQLDataSourcesSelectPopUp(Obj.config).mssql_data_size.text.toString();
			}
		}
		else if(Obj.type()==ActionObjectParent.FILTER_RESAMPLE)
		{
			procedure+="removeNull";
		}	
		else if(Obj.type()==ActionObjectParent.DESCRITIZE)
		{
			procedure+="descritize";
		}
		else if(Obj.type()==ActionObjectParent.RANGESPLITE)
		{
			procedure+="rangesplite";
		}
		else if(Obj.type()==ActionObjectParent.WAH_COMPRESSTION)
		{
			procedure+="wah";
		}
		else if(Obj.type()==ActionObjectParent.WAH_COMPRESSTION_2)
		{
			procedure+="ewah";
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_APRIORY)
		{					
			procedure+="apriory";
			algoPara=AprioryConfigPopUp(Obj.config).support_combo.selectedLabel+"@@"+AprioryConfigPopUp(Obj.config).confidence_combo.selectedLabel+"@@"+AprioryConfigPopUp(Obj.config).number_of_rules_input.text;			
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_CLASSIFICATION)
		{
			procedure+="classification";
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_NAIVEBAYES)
		{
			procedure+="naiveBayes";
		}
		else if(Obj.type()==ActionObjectParent.TEXT_VIEWER)
		{
			procedure+="text";
		}
		else if(Obj.type()==ActionObjectParent.TREE_VIEWER)
		{
			procedure+="tree";
		}	
		if(i+1!=actionObjectSequence.length)
		{
			procedure+="->";
		}
		
	}
	return procedure;
}

private function showStatus(status:String):void 
{	 
	 if(status!=DONE)
	 {
	 	if(executingMask==null)
		{
			executingMask=ExecutingMarkPopUp(PopUpManager.createPopUp(canvasmain, ExecutingMarkPopUp , true));
			//executingMask.y=canvasmain.height-50;
			//executingMask.x=canvasmain.width-executingMask.width;
			progress_Bar=new ProgressBarComponent();
			progress_Bar.x=60;
			progress_Bar.y=250;
			executingMask.addChild(progress_Bar);			
			PopUpManager.centerPopUp(executingMask);
		}
		else
		{
			PopUpManager.addPopUp(executingMask,canvasmain,true);
			PopUpManager.centerPopUp(executingMask);
		}	 	
	 	progress_Bar.indeterminate=true;
	 }
	 else if(progress_Bar!=null && status==DONE)
	 {
	 	PopUpManager.removePopUp(executingMask);
	 	progress_Bar.indeterminate=false;
	 }
	 if(progress_Bar!=null)
	 {
    	progress_Bar.label= status;
	 }
}

private function createFlow(flow:SavedFlow):void
{
	var _actionObjectsOnCanvas:Dictionary=flow.actionObjectsOnCanvas;
	var _actionObjectSequence:Array=flow.actionObjectSequence;
	var _arrowsOnCanvas:Array=flow.arrowsOnCanvas;
	
    var actionObjects:Array = new Array();
    for (var actionObj:Object in _actionObjectsOnCanvas)
    {
        actionObjects.push(actionObj);
    }
    var numberOfActionObjects:int = actionObjects.length;
    
    for(var j:int=0;j<numberOfActionObjects;j++)
	{
		var id:String=actionObjects[j];
		drawingcanvas.addChild(ActionObject(_actionObjectsOnCanvas[id]).vbox);
	}
	
	for(var k:int=0;k<_actionObjectSequence.length-1;k++)
    {
    	var shotestPath:Path=Util.getShortestPath(ActionObject(_actionObjectsOnCanvas[_actionObjectSequence[k]]),ActionObject(_actionObjectsOnCanvas[_actionObjectSequence[k+1]]));
		var line:Shape = getArrow(1,arrowColour, 1,shotestPath.startX,shotestPath.startY,shotestPath.endX,shotestPath.endY);
		drawingcanvas.rawChildren.addChild(line);
		_arrowsOnCanvas.push(line);
    }
    actionObjectsOnCanvas=_actionObjectsOnCanvas;
    actionObjectSequence=_actionObjectSequence;
    arrowsOnCanvas=_arrowsOnCanvas;
}

private function clearCanvas(event:MouseEvent):void 
{
	var actionObjects:Array = new Array();
    for (var actionObj:Object in actionObjectsOnCanvas)
    {
        actionObjects.push(actionObj);
    }
    var numberOfActionObjects:int = actionObjects.length;
    
	if(0<numberOfActionObjects)
	{
		var numberOfArrows:int = arrowsOnCanvas.length;
		for(var i:int=0;i<numberOfArrows;i++)
		{
			drawingcanvas.rawChildren.removeChild(Shape(arrowsOnCanvas.pop()));
		}
		
		var seq:int = actionObjectSequence.length;
		//for(var k:int=0;k<seq;k++)
		//{
		//	actionObjectSequence.pop();
		//}
		
		for(var j:int=0;j<numberOfActionObjects;j++)
		{
			var id:String=String(actionObjects.pop());
			drawingcanvas.removeChild(ActionObject(actionObjectsOnCanvas[id]).vbox);
			//delete actionObjectsOnCanvas[id];
		}
	}
	actionObjectsOnCanvas=new Dictionary();
	actionObjectSequence=new Array();
	arrowsOnCanvas=new Array();
}

private function mouseDownHandler(event:MouseEvent):void 
{
	correctionX=event.localX;
	correctionY=event.localY;
	
    var dragInitiator:Image=Image(event.currentTarget);
    var ds:DragSource = new DragSource();
    ds.addData(dragInitiator, "img");

    //TODO add code for other action objects
    if(dragInitiator.id=="CSV_DataSource")
    {
    	var csvDataSource:ActionObject = new CSVDataSource();
    	csvDataSource.image=dragInitiator;
    	actionObj=csvDataSource;	
    }
    else if(dragInitiator.id=="MySQL_DataSource")
    {
    	var mysqlDataSource:ActionObject = new MySQLDataSource();
    	mysqlDataSource.image=dragInitiator;
    	actionObj=mysqlDataSource;	
    }
    else if(dragInitiator.id=="ALGORITHM_Apriory")
    {
    	var algorithmApriory:ActionObject = new AlgorithmApriory();
    	algorithmApriory.image=dragInitiator;
    	actionObj=algorithmApriory;
    }
    else if(dragInitiator.id=="Text_Viewer")
    {
    	var textViewer:ActionObject = new TextViewer();
    	textViewer.image=dragInitiator;
    	actionObj=textViewer;	
    }
    else if(dragInitiator.id=="Tree_Viewer")
    {
    	var treeViewer:ActionObject = new TreeViewer();
    	treeViewer.image=dragInitiator;
    	actionObj=treeViewer;	
    }
    else if(dragInitiator.id=="ALGORITHM_Clasification")
    {
    	var algorithmClassification:ActionObject = new AlgorithmClassification();
    	algorithmClassification.image=dragInitiator;
    	actionObj=algorithmClassification;	
    }
    else if(dragInitiator.id=="NULL_REMOVE")
    {
    	var filterResample:ActionObject = new FilterResample();
    	filterResample.image=dragInitiator;
    	actionObj=filterResample;	
    }
    else if(dragInitiator.id=="WAH_Compression")
    {
    	var wahcompression:ActionObject = new WAHCompression();
    	wahcompression.image=dragInitiator;
    	actionObj=wahcompression;	
    }
    else if(dragInitiator.id=="WAH_Compression2")
    {
    	var wahcompression2:ActionObject = new WAHCompression2();
    	wahcompression2.image=dragInitiator;
    	actionObj=wahcompression2;	
    }
    else if(dragInitiator.id=="XML_Loader")
    {
    	var xmlloader:ActionObject=new XML_Loader_();
    	xmlloader.image=dragInitiator;
    	actionObj=xmlloader;	
    }
    else if(dragInitiator.id=="DISCRETIZE")
    {
    	var discretizeFilter:DiscretizeFilter=new DiscretizeFilter();
    	discretizeFilter.image=dragInitiator;
    	actionObj=discretizeFilter;	
    }
    else if(dragInitiator.id=="RANGE_SPLIT")
    {
    	var rangeSplitter:RangeSplitter=new RangeSplitter();
    	rangeSplitter.image=dragInitiator;
    	actionObj=rangeSplitter;	
    }
    else if(dragInitiator.id=="MSSQL_DataSource")
    {
    	var mssqldatasource:MsSqlDataSource=new MsSqlDataSource();
    	mssqldatasource.image=dragInitiator;
    	actionObj=mssqldatasource;	
    }
    else if(dragInitiator.id=="ALGORITHM_NAIVEBAYES")
    {
    	var naivebayes:AlgorithmNaiveBayes=new AlgorithmNaiveBayes();
    	naivebayes.image=dragInitiator;
    	actionObj=naivebayes;	
    }

    var imageProxy:Image = new Image();
    imageProxy.source = dragInitiator.source;
             
    DragManager.doDrag(dragInitiator, ds, event, imageProxy, 0, 0, 0.80);
}

private function dragEnterHandler(event:DragEvent):void 
{
    if (event.dragSource.hasFormat("img"))
    {
        DragManager.acceptDragDrop(Canvas(event.currentTarget));
    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
        DragManager.acceptDragDrop(Canvas(event.currentTarget));
    }
}

private function dragDropHandler(event:DragEvent):void
{ 
	if (event.dragSource.hasFormat("img"))
    {
    	actionObj.vboxX=event.localX-correctionX-((actionObj.vbox.width/2)-(actionObj.image.measuredWidth/2));
    	actionObj.vboxY=event.localY-correctionY-(actionObj.vbox.height-actionObj.image.measuredHeight-actionObj.label.height)/2;
    	//add objects to canvas
    	drawingcanvas.addChild(actionObj.vbox);
    	//add objects to collection
    	actionObjectsOnCanvas[actionObj.id]=actionObj;
    	
    	if(actionObj.type()==ActionObjectParent.MySQL_DATASOURCE)
    	{
    		var sqlListX:int=actionObj.vboxX+actionObj.vbox.width;
    		var sqlListY:int=drawingcanvas.y+actionObj.vboxY+actionObj.vbox.height-40;
    		var mysqlsourcesPopUp:MySqlDataSourcesSelectPopUp=MySqlDataSourcesSelectPopUp(PopUpManager.createPopUp(this, MySqlDataSourcesSelectPopUp , false));
    		actionObj.configObj=mysqlsourcesPopUp; 
    		mysqlObject=MySQLDataSource(actionObj);        
		    mysqlsourcesPopUp.x=sqlListX;
		    mysqlsourcesPopUp.y=sqlListY;
			var itemId:String = actionObj.id;
    		getDataSourceList(itemId);
    	}
    	else if(actionObj.type()==ActionObjectParent.MSSQL_DATASOURCE)
    	{
    		var mssqlListX:int=actionObj.vboxX+actionObj.vbox.width;
    		var mssqlListY:int=drawingcanvas.y+actionObj.vboxY+actionObj.vbox.height-40;
    		var mssqlsourcesPopUp:MsSQLDataSourcesSelectPopUp=MsSQLDataSourcesSelectPopUp(PopUpManager.createPopUp(this, MsSQLDataSourcesSelectPopUp , false));
    		actionObj.configObj=mssqlsourcesPopUp; 
    		mssqlObject=MsSqlDataSource(actionObj);        
		    mssqlsourcesPopUp.x=mssqlListX;
		    mssqlsourcesPopUp.y=mssqlListY;
			var msitemId:String = actionObj.id;
    		getMSDataSourceList(msitemId);
    	}
    	else if(actionObj.type()==ActionObjectParent.CSV_DATASOURCE)
    	{
    		var csvPathEnterPopUp:CSVConfigPopUp=CSVConfigPopUp(PopUpManager.createPopUp(this, CSVConfigPopUp , false));
    		actionObj.configObj=csvPathEnterPopUp;               
            csvPathEnterPopUp.x=((drawingcanvas.x+drawingcanvas.width)<(actionObj.vboxX+csvPathEnterPopUp.width))?(drawingcanvas.x+drawingcanvas.width-csvPathEnterPopUp.width):(drawingcanvas.x+actionObj.vboxX);;
            csvPathEnterPopUp.y=drawingcanvas.y+actionObj.vboxY+(actionObj.vbox.height/4)*3;
    	}
    	else if(actionObj.type()==ActionObjectParent.XML_LOADER)
    	{
    		var xmlPathsEnterPopUp:XMLLoaderConfigPopUp=XMLLoaderConfigPopUp(PopUpManager.createPopUp(this, XMLLoaderConfigPopUp , false));
    		actionObj.configObj=xmlPathsEnterPopUp;               
            xmlPathsEnterPopUp.x=((drawingcanvas.x+drawingcanvas.width)<(actionObj.vboxX+xmlPathsEnterPopUp.width))?(drawingcanvas.x+drawingcanvas.width-xmlPathsEnterPopUp.width):(drawingcanvas.x+actionObj.vboxX);
            xmlPathsEnterPopUp.y=drawingcanvas.y+actionObj.vboxY+(actionObj.vbox.height/4)*3;
    	}
    	else if(actionObj.type()==ActionObjectParent.ALGORITHM_APRIORY)
    	{
    		var aprioryConfigPopUp:AprioryConfigPopUp=AprioryConfigPopUp(PopUpManager.createPopUp(this, AprioryConfigPopUp , false));
    		actionObj.configObj=aprioryConfigPopUp;               
            aprioryConfigPopUp.x=((drawingcanvas.x+drawingcanvas.width)<(actionObj.vboxX+aprioryConfigPopUp.width))?(drawingcanvas.x+drawingcanvas.width-aprioryConfigPopUp.width):(drawingcanvas.x+actionObj.vboxX);
            aprioryConfigPopUp.y=drawingcanvas.y+actionObj.vboxY+(actionObj.vbox.height/4)*3;
    	}
    	
    	actionObj=null;

    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
    	var obj:ActionObject = ActionObject(event.dragSource.dataForFormat("actionObj"));
    	obj.vboxX=event.localX-obj.correctionX;
    	obj.vboxY=event.localY-obj.correctionY;
    	updateArrows(actionObjectSequence);
    }
}

private function getDataSourceList(itemId:String):void 
{
	//trace("call");
	var ret:Object = new Object();
	ret["flashId"] = __flashPlayerId;
	ret["flashIndex"] = __flashPlayerIndex;
	ret["itemId"] = itemId;
	ret["itemType"] = "Button";
	ret["eventType"] = "click";
	ret["procedure"] = "getMySqlDataSourceList";
	__callBackFunction.call(fabridge,ret);
	//var str:String="mysqlDataSourcesList##<mysqlsource><name>source1</name></mysqlsource><mysqlsource><name>source2</name></mysqlsource>";
	//cplusPluseCallBackFunction(str);
}

private function getMSDataSourceList(itemId:String):void 
{
	//trace("call");
	var ret:Object = new Object();
	ret["flashId"] = __flashPlayerId;
	ret["flashIndex"] = __flashPlayerIndex;
	ret["itemId"] = itemId;
	ret["itemType"] = "Button";
	ret["eventType"] = "click";
	ret["procedure"] = "getMsSqlDataSourceList";
	__callBackFunction.call(fabridge,ret);
	//var str:String="mssqlDataSourcesList##<mssqlsource><name>Mssource1</name></mssqlsource><mssqlsource><name>Mssource2</name></mssqlsource>";
	//cplusPluseCallBackFunction(str);
}

private function dragOverHandler(event:DragEvent):void 
{
	if (event.dragSource.hasFormat("img"))
    {
    	DragManager.showFeedback(DragManager.COPY);
    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
    	DragManager.showFeedback(DragManager.LINK);
    }
}

private function mouseMoveHandler(event:MouseEvent):void 
{
	if(ActionObjectParent.arrowDrawingStatus==ActionObjectParent.ARROW_DRAWING && !(ActionObjectParent.actionObjectClickStatus==ActionObjectParent.ACTIONOBJECT_DOUBLE_CLICKED))
	{		
		var actobj:ActionObject=ActionObject(actionObjectsOnCanvas[ActionObjectParent.drawingEvent.startId]);
		var img:Image=ActionObject(actionObjectsOnCanvas[ActionObjectParent.drawingEvent.startId]).image;
		var sourceX:int = actobj.vboxX+img.x+img.width;
		var sourceY:int = actobj.vboxY+img.y+img.height/2;
		var line:Shape = getArrow(1,arrowColour, .9,sourceX,sourceY,Canvas(event.currentTarget).contentMouseX,Canvas(event.currentTarget).contentMouseY);
		if(tempLine!=null)
		{
			drawingcanvas.rawChildren.removeChild(tempLine);
		}
		drawingcanvas.rawChildren.addChild(line);
		tempLine = line;
	}
	else if(ActionObjectParent.arrowDrawingStatus==ActionObjectParent.ARROW_COMPLETE&& !(ActionObjectParent.actionObjectClickStatus==ActionObjectParent.ACTIONOBJECT_DOUBLE_CLICKED))
	{
		if(tempLine!=null)
		{
			drawingcanvas.rawChildren.removeChild(tempLine);
			tempLine=null;
		}
		addToActionObjectSequence(ActionObjectParent.drawingEvent);
		updateArrows(actionObjectSequence);
		ActionObjectParent.drawingEvent=null;
		ActionObjectParent.arrowDrawingStatus=ActionObjectParent.NO_DRAWING;
	}
}

private function mouseClickHandler(event:MouseEvent):void
{
	if(tempLine!=null&&ActionObjectParent.arrowDrawingStatus==ActionObjectParent.ARROW_DRAWING)
	{
		drawingcanvas.rawChildren.removeChild(tempLine);
		tempLine=null;
		ActionObjectParent.drawingEvent=null;
		ActionObjectParent.arrowDrawingStatus=ActionObjectParent.NO_DRAWING;
	}
}

private function getArrow(thickness:int,colour:uint,apha:int,sourceX:int,sourceY:int,destinationX:int,destinationY:int):Shape
{
	var line:Shape = new Shape();
	line.graphics.lineStyle(thickness,colour,apha);
	line.graphics.beginFill(fillColour);
	var style:ArrowStyle = new ArrowStyle();
	style.headLength = 22;
	style.headWidth = 15;
	style.shaftPosition = 0.25;
	style.shaftThickness = 2;
	style.edgeControlPosition = 0.35;
	style.edgeControlSize = 0.05;
	GraphicsUtil.drawArrow(line.graphics,new Point(sourceX,sourceY),new Point(destinationX,destinationY),style);
	return line;
}

private function updateArrows(seq:Array):void 
{
	//delete old arrows
	var numberOfArrows:int = arrowsOnCanvas.length;
	for(var i:int=0;i<numberOfArrows;i++)
	{
		drawingcanvas.rawChildren.removeChild(Shape(arrowsOnCanvas.pop()));
	}
	//add new arrows
    for(var j:int=0;j<seq.length-1;j++)
    {
    	drawArrow(seq[j],seq[j+1]);
    }
}

private function drawArrow(source:String,destination:String):void
{
	var shotestPath:Path=Util.getShortestPath(ActionObject(actionObjectsOnCanvas[source]),ActionObject(actionObjectsOnCanvas[destination]));
	var line:Shape = getArrow(1,arrowColour, 1,shotestPath.startX,shotestPath.startY,shotestPath.endX,shotestPath.endY);
	drawingcanvas.rawChildren.addChild(line);
	arrowsOnCanvas.push(line);
}

private function addToActionObjectSequence(drawingEvent:DrawingEvent):void 
{
    if(actionObjectSequence.length==0)
    {
    	actionObjectSequence.push(drawingEvent.startId);
    	actionObjectSequence.push(drawingEvent.destinationId);
    }
    else if(drawingEvent.startId==actionObjectSequence[actionObjectSequence.length-1])
    {
    	actionObjectSequence.push(drawingEvent.destinationId);
    }
    else if(drawingEvent.destinationId==actionObjectSequence[0])
    {
    	actionObjectSequence.unshift(drawingEvent.destinationId);
    }
}
