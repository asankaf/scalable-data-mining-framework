package ActionClasses
{
	import mx.controls.Image;

	public class ActionObjectParent implements ActionObject
	{
		protected var imageObj:Image;
		private var imageXposition:Number;
		private var imageYposition:Number;	
		protected var correctionXvalue:Number;
		protected var correctionYvalue:Number;
		protected var arrowDrawing:Boolean=false;
		private var idValue:Number;
		
		public function ActionObjectParent()
		{
		}

		public function type():Number
		{
			return -1;
		}
		
		public function get id():Number
		{
			return idValue;
		}
		
		public function set id(idval:Number):void
		{
			idValue=idval;
		}
		
		public function get image():Image
		{
			return imageObj;
		}
		
		public function set image(img:Image):void
		{
			imageObj.source=img.source;
		}
		
		public function get imageX():Number
		{
			return imageXposition;
		}
		
		public function set imageX(num:Number):void
		{
			imageXposition=num;
			imageObj.x=imageXposition;
		}
		
		public function get imageY():Number
		{
			return imageYposition;
		}
		
		public function set imageY(num:Number):void
		{
			imageYposition=num;
			imageObj.y=imageYposition;
		}
		
		public function get correctionX():Number
		{
			return correctionXvalue;
		}
		
		public function get correctionY():Number
		{
			return correctionYvalue;
		}
		
	}
}