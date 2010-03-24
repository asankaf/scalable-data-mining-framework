// ActionScript file
import flash.events.Event;

import mx.controls.Alert;

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
	//lbl.text=  xstart.text;
}