// ActionScript file
import ActionClasses.ActionObject;
import ActionClasses.CSVDataSource;
import ActionClasses.MySQLDataSource;

import flash.events.Event;
import flash.events.MouseEvent;

import mx.controls.Alert;
import mx.controls.Image;
import mx.core.DragSource;
import mx.events.DragEvent;
import mx.managers.DragManager;


private var actionObj:ActionObject;
private var correctionX:Number;
private var correctionY:Number;
private var actionObjectsOnCanvas:Dictionary = new Dictionary();

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
	//if(event.currentTarget
	//trace("moving mouce"+num++);
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
