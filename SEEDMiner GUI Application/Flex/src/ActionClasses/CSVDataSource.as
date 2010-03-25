package ActionClasses
{
	import flash.events.MouseEvent;
	
	import mx.controls.Image;
	import mx.core.DragSource;
	import mx.managers.DragManager;
	
	public class CSVDataSource implements ActionObject
	{
		private const CSV_DATASOURCE:Number = 0;
		private var imageObj:Image;
		private var imageXposition:Number;
		private var imageYposition:Number;	
		public var correctionX:Number;
		public var correctionY:Number;
		
		public function CSVDataSource()
		{
			imageObj = new Image();
			imageObj.addEventListener(MouseEvent.MOUSE_DOWN,mouseDownHandler);
		}

		public function type():Number
		{
			return CSV_DATASOURCE;
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
		
		private function mouseDownHandler(event:MouseEvent):void
		{
			correctionX=event.localX;
			correctionY=event.localY;
			
		    var dragInitiator:Image=Image(event.currentTarget);
		    var ds:DragSource = new DragSource();
		    ds.addData(this, "actionObj");
		
		    var imageProxy:Image = new Image();
		    imageProxy.source = dragInitiator.source;
		             
		    DragManager.doDrag(dragInitiator, ds, event, imageProxy, 0, 0, 0.80);
		}
		
	}
}