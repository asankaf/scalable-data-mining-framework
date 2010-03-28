// ActionScript file
import ActionClasses.ActionObject;
import ActionClasses.ActionObjectParent;
import ActionClasses.CSVDataSource;
import ActionClasses.DrawingEvent;
import ActionClasses.MySQLDataSource;

import flash.display.Shape;
import flash.events.Event;
import flash.events.MouseEvent;

import mx.containers.Canvas;
import mx.controls.Alert;
import mx.controls.Image;
import mx.core.DragSource;
import mx.events.DragEvent;
import mx.managers.DragManager;


private var actionObj:ActionObject;
private var correctionX:Number;
private var correctionY:Number;
private var actionObjectsOnCanvas:Dictionary = new Dictionary();
private var actionObjectSequence:Array = new Array();
private var tempLine:Shape; 

public function startUp(event:Event):void
{
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
    	//trace(actionObj.vbox.width);
    	//trace(actionObj.image.measuredWidth);
    	actionObj.vboxY=event.localY-correctionY-(actionObj.vbox.height-actionObj.image.measuredHeight-actionObj.label.height)/2;
    	drawingcanvas.addChild(actionObj.vbox);
    	
    	//add objects to collection
    	actionObjectsOnCanvas[actionObj.id]=actionObj;
    	var ob:ActionObject=ActionObject(actionObjectsOnCanvas[actionObj.id]);
    	//trace("id:"+ob.id+"type:"+ob.type());
    	
    	actionObj=null;
    	
    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
    	var obj:ActionObject = ActionObject(event.dragSource.dataForFormat("actionObj"));
    	obj.vboxX=event.localX-obj.correctionX;
    	//trace(event.localX);
    	obj.vboxY=event.localY-obj.correctionY;
    	//trace(event.localY);
    }
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
	if(ActionObjectParent.arrowDrawingStatus==ActionObjectParent.ARROW_DRAWING)
	{
		//trace("drawing "+ActionObjectParent.drawingEvent.startId+" to "+event.localX+" , "+event.localY);
		
		var line:Shape = new Shape();
		line.graphics.lineStyle(2,0xFF0000, .75);
		line.graphics.beginFill(0x00FF00);
		line.graphics.moveTo(ActionObject(actionObjectsOnCanvas[ActionObjectParent.drawingEvent.startId]).vboxX, ActionObject(actionObjectsOnCanvas[ActionObjectParent.drawingEvent.startId]).vboxY);
		line.graphics.lineTo(Canvas(event.currentTarget).contentMouseX, Canvas(event.currentTarget).contentMouseY);
		if(tempLine!=null)
		{
			drawingcanvas.rawChildren.removeChild(tempLine);
		}
		drawingcanvas.rawChildren.addChild(line);
		tempLine = line;
	}
	else if(ActionObjectParent.arrowDrawingStatus==ActionObjectParent.ARROW_COMPLETE)
	{
		trace("draw complete "+ActionObjectParent.drawingEvent.startId+" to "+ActionObjectParent.drawingEvent.destinationId);
		
		if(tempLine!=null)
		{
			drawingcanvas.rawChildren.removeChild(tempLine);
		}
		addToActionObjectSequence(ActionObjectParent.drawingEvent);
		trace(actionObjectSequence);
		updateArrows(actionObjectSequence);
		ActionObjectParent.drawingEvent=null;
		ActionObjectParent.arrowDrawingStatus=ActionObjectParent.NO_DRAWING;
	}
}

private function updateArrows(seq:Array):void 
{
    for(var i:int=0;i<seq.length-1;i++)
    {
    	drawArrow(seq[i],seq[i+1]);
    }
}

private function drawArrow(source:String,destination:String):void
{
	var line:Shape = new Shape();
	line.graphics.lineStyle(2,0xFF0000, .75);
	line.graphics.beginFill(0x00FF00);
	line.graphics.moveTo(ActionObject(actionObjectsOnCanvas[source]).vboxX, ActionObject(actionObjectsOnCanvas[source]).vboxY);
	line.graphics.lineTo(ActionObject(actionObjectsOnCanvas[destination]).vboxX, ActionObject(actionObjectsOnCanvas[destination]).vboxY);
	//drawingcanvas.rawChildren.removeChild(tempLine);
	drawingcanvas.rawChildren.addChild(line);
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

private function myEventHandler(e:Event):void 
{
    Alert.show("The button '" + e.currentTarget.id + "' was clicked.");
}

private function draw(e:Event):void 
{

	var line:Shape = new Shape();
	
	line.graphics.lineStyle(2,0xFF0000, .75);
	
	//line.graphics.drawRect(0, 0, xslideval.value,yslideval.value);
	
	var mySprite:Sprite = new Sprite();
	mySprite.graphics.beginFill(0xFFCC00,.2);
	//mySprite.graphics.drawCircle(200, 300, xslideval.value);
	
	drawingcanvas.rawChildren.addChild(line);
	drawingcanvas.rawChildren.addChild(mySprite);
}

var num:Number=0;

private function testEvent(e:Event):void
{
	//lble.text = (num++).toString();
}
