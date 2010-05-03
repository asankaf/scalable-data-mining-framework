package ActionClasses.VisualTreeElements
{
	import flash.display.Shape;
	import flash.geom.Point;
	
	import mx.controls.Label;
	
	public class LinkElement
	{
		public var line:Shape;
		public var lable:Label;
		
		public function LinkElement(text:String,start:Point,end:Point)
		{
			line = new Shape();
			line.graphics.lineStyle(1, 0xffd0b4, .75);
			line.graphics.beginFill(0xCE9DF1);
			line.graphics.moveTo(start.x, start.y);
			line.graphics.lineTo(end.x, end.y);
			
			lable=new Label();
			lable.alpha=1;
			lable.setStyle("fontSize",11);
			lable.setStyle("color","#ffba91");
			lable.text=text;
			lable.width=text.length*Element.linkLableCharacterWidth;
			lable.height=Element.rootElementHeight;
			
			var correctionX:int=(end.x-start.x)/2;
			var correctionY:int=(end.y-start.y)/2;
			if(0<correctionX)
			{
				correctionX+=Element.rootElementHeight;
			}
			var lableX:int=start.x+correctionX;
			var lableY:int=start.y+correctionY;

			lable.x=lableX;
			lable.y=lableY;
		}

	}
}