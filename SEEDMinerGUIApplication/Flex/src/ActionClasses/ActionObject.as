package ActionClasses
{
	import mx.containers.VBox;
	import mx.controls.Image;
	import mx.controls.Label;
	
	
	public interface ActionObject
	{
		function type():Number;
		
		function get id():String;
		
		function set id(idval:String):void;
		
		function get image():Image;
		
		function set image(img:Image):void;
		
		function get vbox():VBox;
		
		function get label():Label;
		
		function set labelText(lableTxt:String):void;
		
		function get vboxX():Number;
		
		function set vboxX(num:Number):void;
		
		function get vboxY():Number;
		
		function set vboxY(num:Number):void;
		
		function get correctionX():Number;
		
		function get correctionY():Number;
				
	}
}