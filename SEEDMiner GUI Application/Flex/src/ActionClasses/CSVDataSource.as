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
		private var correctionXvalue:Number;
		private var correctionYvalue:Number;
		private var arrowDrawing:Boolean=false;
		private var idValue:Number;
		
		public function CSVDataSource()
		{
			imageObj = new Image();
			imageObj.addEventListener(MouseEvent.MOUSE_DOWN,mouseDownHandler);
			imageObj.addEventListener(MouseEvent.CLICK,mouseClickHandler);
		}

		public function type():Number
		{
			return CSV_DATASOURCE;
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
		
		private function mouseDownHandler(event:MouseEvent):void
		{
			correctionXvalue=event.localX;
			correctionYvalue=event.localY;
			
		    var dragInitiator:Image=Image(event.currentTarget);
		    var ds:DragSource = new DragSource();
		    ds.addData(this, "actionObj");
		
		    var imageProxy:Image = new Image();
		    imageProxy.source = dragInitiator.source;
		             
		    DragManager.doDrag(dragInitiator, ds, event, imageProxy, 0, 0, 0.80);
		}
		
		private function mouseClickHandler(event:MouseEvent):void
		{
		    arrowDrawing=true;
		}
		
	}
}