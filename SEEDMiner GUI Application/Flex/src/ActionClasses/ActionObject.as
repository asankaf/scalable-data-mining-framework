package ActionClasses
{
	import mx.controls.Image;
	
	
	public interface ActionObject
	{
		function type():Number;
		
		function get image():Image;
		
		function set image(img:Image):void;
		
		function get imageX():Number;
		
		function set imageX(num:Number):void;
		
		function get imageY():Number;
		
		function set imageY(num:Number):void;

	}
}