// ActionScript file
import ActionClasses.ActionObject;
import ActionClasses.ActionObjectParent;
import ActionClasses.AlgorithmApriory;
import ActionClasses.AlgorithmClassification;
import ActionClasses.CSVDataSource;
import ActionClasses.ClassificationDom;
import ActionClasses.DrawingEvent;
import ActionClasses.FilterResample;
import ActionClasses.GenerateGraphicalTree;
import ActionClasses.MySQLDataSource;
import ActionClasses.Path;
import ActionClasses.TextViewer;
import ActionClasses.TreeViewer;
import ActionClasses.Util;
import ActionClasses.VisualTreeElements.Element;
import ActionClasses.WAHCompression;
import ActionClasses.WAHCompression2;

import com.dncompute.graphics.ArrowStyle;
import com.dncompute.graphics.GraphicsUtil;

import flash.display.Shape;
import flash.events.Event;
import flash.events.MouseEvent;

import mx.containers.Canvas;
import mx.controls.Image;
import mx.core.DragSource;
import mx.events.DragEvent;
import mx.managers.DragManager;

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
private var DONE:String="Done";
private var EXECUTING:String="Executing Flaw";

private var sqlListX:int;
private var sqlListY:int;


public function startUp(event:Event):void
{
	
}

public function cplusPluseCallBackFunction(str:String):void
{
	var strings:Array=str.split("##");
	var view:String=strings[0];
	
	if(view=="textViewer")
	{
		var data:String=strings[1];
		var textPopUp:TEXTViewPopUp=TEXTViewPopUp(PopUpManager.createPopUp(this, TEXTViewPopUp , false));
		var startindex:int=String(data).search("\n");
		textPopUp.textViewerTextArea.width=Element.estimateStringPixelLength(data.slice(0,startindex));
		textPopUp.textViewerTextArea.text=data;
		var point1:Point = new Point();
		point1.x=0;
	    point1.y=0;                
	    textPopUp.x=canvasmain.width/2-textPopUp.width/2;
	    textPopUp.y=canvasmain.height/2-textPopUp.height/2;
	}
	
	else if(view=="treeViewer")
	{
		
		var treeString:String=strings[1];
		var dom:ClassificationDom=new ClassificationDom(treeString);
		
	    var treePopUp:TreeViewPopUp=TreeViewPopUp(PopUpManager.createPopUp(this, TreeViewPopUp , false));
	    var point1:Point = new Point();
		point1.x=0;
	    point1.y=0;                
	    treePopUp.x=canvasmain.width/2-treePopUp.width/2;
	    treePopUp.y=canvasmain.height/2-treePopUp.height/2;
	    var genTree:GenerateGraphicalTree=new GenerateGraphicalTree(dom,treePopUp);
	}	
	else if(view=="sqlDataSourcesList")
	{
		var sqlsourcelist:String=strings[1];
		
		var dataSourcesList:XMLList= new XMLList(sqlsourcelist);
		var mysqlsources:MySqlDataSourcesSelectPopUp=MySqlDataSourcesSelectPopUp(PopUpManager.createPopUp(this, MySqlDataSourcesSelectPopUp , false));
		mysqlsources.dataSoucesCol.dataField="name";
		mysqlsources.dg.dataProvider=dataSourcesList;
		var point1:Point = new Point();
		point1.x=0;
	    point1.y=0;                
	    mysqlsources.x=sqlListX;
	    mysqlsources.y=toptabs.height+sqlListY;
	}
	else if(view=="noView")
	{
		showError("Invalid flaw! Please create a new valid flaw to execute...");
		clearCanvas(new MouseEvent(Event.CHANGE));
	}
	
	showStatus(DONE);
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
	
		//__callBackFunction.call(fabridge,ret);
		//var str:String="treeViewer##outlook = sunny\n|   humidity = high: no (3.0)\n|   humidity = normal: yes (2.0)\noutlook = overcast: yes (4.0)\noutlook = rainy\n|   windy = TRUE: no (2.0)\n|   windy = FALSE: yes (3.0)";
		//var str:String="treeViewer##1 = 0\n|   2 = 0: 0 (186/1)\n|   2 = 1\n|   |   0 = 0: 0 (4)\n|   |   0 = 1: 1 (3)\n|   2 = 2: 0 (61)\n1 = 1\n|   0 = 0\n|   |   1 = 0\n|   |   |   0 = 0: 0 (7)\n|   |   |   0 = 1\n|   |   |   |   0 = 0: 1 (49/1)\n|   |   |   |   0 = 1: 0 (3)\n|   |   |   |   0 = 2: 1 (0)\n|   |   1 = 1: 0 (39/1)\n|   |   1 = 2: 0 (14)\n|   0 = 1: 2 (9/1)";
		//var str:String="treeViewer##children = 0\n|   save_act = NO: YES (48)\n|   save_act = YES: NO (240)\nchildren = 1: YES (144)\nchildren = 2\n|   car = NO: YES (48)\n|   car = YES: NO (96)\nchildren = 3: NO (96)";
		//var str:String="treeViewer##petalwidth <= 0.6: Iris-setosa (50.0)\npetalwidth > 0.6\n|   petalwidth <= 1.7\n|   |   petallength <= 4.9: Iris-versicolor (48.0/1.0)\n|   |   petallength > 4.9\n|   |   |   petalwidth <= 1.5: Iris-virginica (3.0)\n|   |   |   petalwidth > 1.5: Iris-versicolor (3.0/1.0)\n|   petalwidth > 1.7: Iris-virginica (46.0/1.0)";
		var str:String="noView##petalwidth <= 0.6: 6.0/1.0)";
		cplusPluseCallBackFunction(str);
	
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
			if(Obj.config==null)
			{
				showError("Path not configured!\nDoubleClick the 'CSV DataSource' to configure path!");
				return null;
			}
			else
			{
				procedure+="csv";
				procedurePara=CSVConfigPopUp(Obj.config).location.text.toString();
			}
			
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_APRIORY)
		{
			procedure+="apriory";
		}
		else if(Obj.type()==ActionObjectParent.TEXT_VIEWER)
		{
			procedure+="text";
		}
		else if(Obj.type()==ActionObjectParent.TREE_VIEWER)
		{
			procedure+="tree";
		}
		else if(Obj.type()==ActionObjectParent.ALGORITHM_CLASSIFICATION)
		{
			procedure+="classification";
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
	 	exe.label="Executing";
	 	exe.enabled=false;
	 	progressBar.indeterminate=true;
	 }
	 else
	 {
	 	exe.label="Execute Flow";
	 	exe.enabled=true;
	 	progressBar.indeterminate=false;
	 }
     progressBar.label= status;
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
		for(var k:int=0;k<seq;k++)
		{
			actionObjectSequence.pop();
		}
		
		for(var j:int=0;j<numberOfActionObjects;j++)
		{
			var id:String=String(actionObjects.pop());
			drawingcanvas.removeChild(ActionObject(actionObjectsOnCanvas[id]).vbox);
			delete actionObjectsOnCanvas[id];
		}
	}
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
    else if(dragInitiator.id=="FILTER_Resample")
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
    		sqlListX=actionObj.vboxX+actionObj.vbox.width;
    		sqlListY=actionObj.vboxY+actionObj.vbox.height-40;
    		getDataSourceList();
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

private function getDataSourceList():void 
{
	var ret:Object = new Object();
	ret["flashId"] = __flashPlayerId;
	ret["flashIndex"] = __flashPlayerIndex;
	ret["itemId"] = "sql";
	ret["itemType"] = "Button";
	ret["eventType"] = "click";
	ret["procedure"] = "getMySqlDataSourceList";
	__callBackFunction.call(fabridge,ret);
	//var str:String="sqlDataSourcesList##<mysqlsource><name>source1</name></mysqlsource><mysqlsource><name>source2</name></mysqlsource>";
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
