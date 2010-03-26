package ActionClasses
{
	import flash.events.MouseEvent;
	
	import mx.controls.Image;
	import mx.core.DragSource;
	import mx.managers.DragManager;

	public class MySQLDataSource extends ActionObjectParent
	{
		private const MySQL_DATASOURCE:Number = 1;
		
		public function MySQLDataSource()
		{
			imageObj = new Image();
			var idgen:int =Util.generateId();
			imageObj.id=idgen.toString();
			this.id=idgen;
			imageObj.addEventListener(MouseEvent.MOUSE_DOWN,mouseDownHandler);
			imageObj.addEventListener(MouseEvent.CLICK,mouseClickHandler);
		}

		override public function type():Number
		{
			return MySQL_DATASOURCE;
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