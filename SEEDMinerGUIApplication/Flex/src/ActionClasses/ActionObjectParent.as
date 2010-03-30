package ActionClasses
{
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import mx.containers.VBox;
	import mx.controls.Image;
	import mx.controls.Label;
	import mx.core.DragSource;
	import mx.managers.DragManager;
	import mx.managers.PopUpManager;

	public class ActionObjectParent implements ActionObject
	{
		public static const CSV_DATASOURCE:Number = 0;
		public static const MySQL_DATASOURCE:Number = 1;
		public static const TEXT_VIEWER:Number = 2;
		
		public static const NO_DRAWING:Number=-1;
		public static const ARROW_DRAWING:Number=0;
		public static const ARROW_COMPLETE:Number=1;
		
		public static const ACTIONOBJECT_NOT_CLICKED:Number=0;
		public static const ACTIONOBJECT_CLICKED:Number=1;
		public static const ACTIONOBJECT_DOUBLE_CLICKED:Number=2;
		
		private var vboxObj:VBox;
		private var imageObj:Image;
		private var labelObj:Label;
		private var vboxXposition:Number;
		private var vboxYposition:Number;
		private var correctionXvalue:Number;
		private var correctionYvalue:Number;
		public static var arrowDrawingStatus:Number=NO_DRAWING;
		public static var drawingEvent:DrawingEvent;
		private var idValue:String;
		public static var actionObjectClickStatus:Number=ACTIONOBJECT_NOT_CLICKED;
		public var config:Object;
		
		private function mouseMoveHandler(event:MouseEvent):void
		{
		    var dragInitiator:VBox=VBox(event.currentTarget);
		    var ds:DragSource = new DragSource();
		    ds.addData(this, "actionObj");
		    var imageProxy:Image = new Image();
		    imageProxy.source = Image(dragInitiator.getChildAt(0)).source;
		    DragManager.doDrag(Image(dragInitiator.getChildAt(0)), ds, event, imageProxy,0,2, 0.80);
		}
		
		private function mouseDownHandler(event:MouseEvent):void
		{
		    correctionXvalue=VBox(event.currentTarget).contentMouseX;
			correctionYvalue=VBox(event.currentTarget).contentMouseY;
		}
		
		private  function mouseClickHandler(event:MouseEvent):void
		{
			//trace("clicked");
			actionObjectClickStatus=ACTIONOBJECT_CLICKED;
			if(arrowDrawingStatus==NO_DRAWING)
			{
				//trace("drawing start");
				arrowDrawingStatus=ARROW_DRAWING;
				drawingEvent=new DrawingEvent()
				drawingEvent.startId=VBox(event.currentTarget).id;
			}
			else if(arrowDrawingStatus==ARROW_DRAWING)
			{
				arrowDrawingStatus=ARROW_COMPLETE;
				drawingEvent.destinationId=VBox(event.currentTarget).id;
			}
		}
		
		private  function mouseDoubleClickHandler(event:MouseEvent):void
		{
			trace("double clicked");
			actionObjectClickStatus=ACTIONOBJECT_DOUBLE_CLICKED;
			arrowDrawingStatus=NO_DRAWING;
			
			if(this.type()==ActionObjectParent.CSV_DATASOURCE)
			{ 
				if(config==null)
				{
					config=CSVConfigPopUp(PopUpManager.createPopUp(this.vbox, CSVConfigPopUp , false));
				}
				else
				{
					PopUpManager.addPopUp(CSVConfigPopUp(config),this.vbox,false);
				}
				var point1:Point = new Point();
				point1.x=0;
	            point1.y=0;                
	            point1=this.vbox.localToGlobal(point1);
	            //login.x=point1.x-50;
	            //login.y=point1.y+90; 
	            CSVConfigPopUp(config).x=200;
	            CSVConfigPopUp(config).y=point1.y+90;
			}
		}
		
		public function ActionObjectParent(label:String)
		{
			vboxObj=new VBox();
			imageObj = new Image();
			labelObj=new Label();
			//vboxObj.setStyle("borderColor","ffffff")
			//vboxObj.setStyle("borderStyle","solid")
			//vboxObj.setStyle("borderThickness","1")
			labelObj.setStyle("textAlign","center");
			vboxObj.setStyle("verticalAlign","middle");
			vboxObj.setStyle("horizontalAlign","center");
			labelObj.text=label;
			vboxObj.width=120;
			vboxObj.height=120;
			labelObj.height=20;
			var idgen:String =Util.generateId();
			vboxObj.id=idgen;
			this.idValue=idgen;
			vboxObj.addChild(imageObj);
			vboxObj.addChild(labelObj);
			//trace(idgen);
			vboxObj.addEventListener(MouseEvent.CLICK,mouseClickHandler);
			vboxObj.addEventListener(MouseEvent.MOUSE_MOVE,mouseMoveHandler);
			vboxObj.addEventListener(MouseEvent.MOUSE_DOWN,mouseDownHandler);
			vboxObj.doubleClickEnabled=true;
			vboxObj.addEventListener(MouseEvent.DOUBLE_CLICK,mouseDoubleClickHandler);
		}

		public function type():Number
		{
			return -1;
		}
		
		public function get id():String
		{
			return idValue;
		}
		
		public function set id(idval:String):void
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
		
		public function get vbox():VBox
		{
			return vboxObj;
		}
		
		public function get label():Label
		{
			return labelObj;
		}
		
		public function set labelText(lableTxt:String):void
		{
			labelText=lableTxt;
		}
		
		public function get vboxX():Number
		{
			return vboxXposition;
		}
		
		public function set vboxX(num:Number):void
		{
			vboxXposition=num;
			vboxObj.x=vboxXposition;
		}
		
		public function get vboxY():Number
		{
			return vboxYposition;
		}
		
		public function set vboxY(num:Number):void
		{
			vboxYposition=num;
			vboxObj.y=vboxYposition;
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