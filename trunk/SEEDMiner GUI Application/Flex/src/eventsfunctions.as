// ActionScript file
import ActionClasses.ActionObject;
import ActionClasses.CSVDataSource;

import flash.events.Event;
import flash.events.MouseEvent;

import mx.controls.Alert;
import mx.controls.Image;
import mx.core.DragSource;
import mx.events.DragEvent;
import mx.managers.DragManager;


public var actionObj:ActionObject;
public var correctionX:Number;
public var correctionY:Number;

private function mouseDownHandler(event:MouseEvent):void 
{
	correctionX=event.localX;
	correctionY=event.localY;
	
    var dragInitiator:Image=Image(event.currentTarget);
    var ds:DragSource = new DragSource();
    ds.addData(dragInitiator, "img");

    //TODO add code for other action objects
    var csvDataSource:ActionObject = new CSVDataSource();
    csvDataSource.image=dragInitiator;
    actionObj=csvDataSource;

    var imageProxy:Image = new Image();
    imageProxy.source = dragInitiator.source;
             
    DragManager.doDrag(dragInitiator, ds, event, imageProxy, 0, 0, 0.80);
}

private function dragEnterHandler(event:DragEvent):void {
    if (event.dragSource.hasFormat("img"))
    {
        DragManager.acceptDragDrop(Canvas(event.currentTarget));
    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
        DragManager.acceptDragDrop(Canvas(event.currentTarget));
    }
}

private function dragDropHandler(event:DragEvent):void {
	if (event.dragSource.hasFormat("img"))
    {
    	actionObj.imageX=event.localX-correctionX;
    	actionObj.imageY=event.localY-correctionY;
    	drawingcanvas.addChild(actionObj.image);
    	actionObj=null;
    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
    	var obj:CSVDataSource = CSVDataSource(event.dragSource.dataForFormat("actionObj"));
    	obj.image.x=event.localX-obj.correctionX;
    	obj.image.y=event.localY-obj.correctionY;
    }
}

private function dragOverHandler(event:DragEvent):void {
	if (event.dragSource.hasFormat("img"))
    {
    	DragManager.showFeedback(DragManager.LINK);
    }
    else if (event.dragSource.hasFormat("actionObj"))
    {
    	DragManager.showFeedback(DragManager.LINK);
    }
}

private function myEventHandler(e:Event):void {
    Alert.show("The button '" + e.currentTarget.id + "' was clicked.");
}

private function draw(e:Event):void {

	var line:Shape = new Shape();
	
	line.graphics.lineStyle(2,0xFF0000, .75);
	
	line.graphics.drawRect(0, 0, xslideval.value,yslideval.value);
	
	var mySprite:Sprite = new Sprite();
	mySprite.graphics.beginFill(0xFFCC00,.2);
	mySprite.graphics.drawCircle(200, 300, xslideval.value);
	
	drawingcanvas.rawChildren.addChild(line);
	drawingcanvas.rawChildren.addChild(mySprite);
}

var num:Number=0;

private function testEvent(e:Event):void{
	lble.text = (num++).toString();
}
