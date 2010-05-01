package ActionClasses.VisualTreeElements
{
	import flash.display.Graphics;
	import mx.graphics.RectangularDropShadow;
	import mx.skins.RectangularBorder;
	
	public class RootElementBorderSkin extends RectangularBorder
	{
		private var dropShadow:RectangularDropShadow;
		
		public function RootElementBorderSkin()
		{
		}
		
		override protected function updateDisplayList(unscaledWidth:Number, unscaledHeight:Number):void 
	    {
	       super.updateDisplayList(unscaledWidth, unscaledHeight);
	
	       var cornerRadius:Number = getStyle("cornerRadius");
	       var backgroundColor:int = getStyle("backgroundColor");
	       var backgroundAlpha:Number = getStyle("backgroundAlpha");
	       graphics.clear();
	
	       // Background
	       drawRoundRect(0, 0, unscaledWidth, unscaledHeight, 
	           {tl: 0, tr:cornerRadius, bl: cornerRadius, br: 0}, 
	           backgroundColor, backgroundAlpha);
	
	       // Shadow
	       if (!dropShadow)
	          dropShadow = new RectangularDropShadow();
	
	       dropShadow.distance = 8;
	       dropShadow.angle = 45;
	       dropShadow.color = 0;
	       dropShadow.alpha = 0.4;
	       dropShadow.tlRadius = 0;
	       dropShadow.trRadius = cornerRadius;
	       dropShadow.blRadius = cornerRadius;
	       dropShadow.brRadius = 0;
	       dropShadow.drawShadow(graphics, 0, 0, unscaledWidth, unscaledHeight);
	    }

	}
}