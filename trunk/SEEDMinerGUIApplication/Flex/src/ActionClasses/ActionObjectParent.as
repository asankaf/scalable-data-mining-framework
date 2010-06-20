package ActionClasses
{
	import flash.events.MouseEvent;
	
	import mx.containers.VBox;
	import mx.controls.Image;
	import mx.controls.Label;
	import mx.core.DragSource;
	import mx.managers.DragManager;
	import mx.managers.PopUpManager;
	
	import seedminer.AprioryConfigPopUp;
	import seedminer.MsSQLDataSourcesSelectPopUp;
	import seedminer.SpacePopUp;
	import seedminer.TimePopUp;
	import seedminer.XMLLoaderConfigPopUp;


	public class ActionObjectParent implements ActionObject
	{
		public static const CSV_DATASOURCE:Number = 0;
		public static const MySQL_DATASOURCE:Number = 1;
		public static const TEXT_VIEWER:Number = 2;
		public static const ALGORITHM_APRIORY:Number = 3;
		public static const TREE_VIEWER:Number = 4;
		public static const ALGORITHM_CLASSIFICATION:Number = 5;
		public static const FILTER_RESAMPLE:Number = 6;
		public static const WAH_COMPRESSTION:Number = 7;
		public static const WAH_COMPRESSTION_2:Number = 8;
		public static const XML_LOADER:Number = 9;
		public static const DESCRITIZE:Number = 10;
		public static const RANGESPLITE:Number = 11;
		public static const MSSQL_DATASOURCE:Number = 12;
		public static const ALGORITHM_NAIVEBAYES:Number = 13;
		
		
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
		public var timeStamp:TimePopUp;
		public var spaceStamps:SpacePopUp;
		
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
			actionObjectClickStatus=ACTIONOBJECT_CLICKED;
			if(arrowDrawingStatus==NO_DRAWING)
			{
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
			}
			else if(this.type()==ActionObjectParent.MySQL_DATASOURCE)
			{ 
				if(config==null)
				{
					config=MySqlDataSourcesSelectPopUp(PopUpManager.createPopUp(this.vbox, MySqlDataSourcesSelectPopUp , false));
				}
				else
				{
					PopUpManager.addPopUp(MySqlDataSourcesSelectPopUp(config),this.vbox,false);
				}
			}
			else if(this.type()==ActionObjectParent.XML_LOADER)
			{ 
				if(config==null)
				{
					config=XMLLoaderConfigPopUp(PopUpManager.createPopUp(this.vbox, XMLLoaderConfigPopUp , false));
				}
				else
				{
					PopUpManager.addPopUp(XMLLoaderConfigPopUp(config),this.vbox,false);
				}
			}
			else if(this.type()==ActionObjectParent.MSSQL_DATASOURCE)
			{ 
				if(config==null)
				{
					config=MsSQLDataSourcesSelectPopUp(PopUpManager.createPopUp(this.vbox, MsSQLDataSourcesSelectPopUp , false));
				}
				else
				{
					PopUpManager.addPopUp(MsSQLDataSourcesSelectPopUp(config),this.vbox,false);
				}
			}
			else if(this.type()==ActionObjectParent.ALGORITHM_APRIORY)
			{ 
				if(config==null)
				{
					config=AprioryConfigPopUp(PopUpManager.createPopUp(this.vbox, AprioryConfigPopUp , false));
				}
				else
				{
					PopUpManager.addPopUp(AprioryConfigPopUp(config),this.vbox,false);
				}
			}
		}
		
		public function ActionObjectParent(label:String)
		{
			vboxObj=new VBox();
			
			imageObj = new Image();
			labelObj=new Label();			
			labelObj.setStyle("textAlign","center");
			labelObj.setStyle("color","#5A5A5A");
			vboxObj.setStyle("verticalAlign","middle");
			vboxObj.setStyle("horizontalAlign","center");
			//vboxObj.setStyle("borderStyle","solid");
			//vboxObj.setStyle("borderThickness","1");
			//vboxObj.setStyle("borderColor","#555555");
			labelObj.text=label;
			vboxObj.width=140;
			vboxObj.height=140;
			labelObj.height=20;
			var idgen:String =Util.generateId();
			vboxObj.id=idgen;
			this.idValue=idgen;
			vboxObj.addChild(imageObj);
			vboxObj.addChild(labelObj);
			vboxObj.addEventListener(MouseEvent.CLICK,mouseClickHandler);
			vboxObj.addEventListener(MouseEvent.MOUSE_MOVE,mouseMoveHandler);
			vboxObj.addEventListener(MouseEvent.MOUSE_DOWN,mouseDownHandler);
			vboxObj.doubleClickEnabled=true;
			vboxObj.addEventListener(MouseEvent.DOUBLE_CLICK,mouseDoubleClickHandler);
		}
		
		public function addTimeStamp(timeStampObj:TimePopUp):void
		{
			timeStamp=timeStampObj;
			vboxObj.addChild(timeStamp);
		}
		
		public function removeTimeStamp():void
		{			
			if(timeStamp!=null)
			{
				vboxObj.removeChild(timeStamp);
			}			
			timeStamp=null;
		}
		
		public function addSpaceStamp(space_stamp:SpacePopUp):void
		{
			spaceStamps=space_stamp;
			vboxObj.addChild(spaceStamps);
		}
		
		public function removeSpaceStamp():void
		{			
			if(spaceStamps!=null)
			{
				vboxObj.removeChild(spaceStamps);
			}			
			spaceStamps=null;
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
		
		public function get configObj():Object
		{
			return config;
		}
		
		public function set configObj(confobj:Object)
		{
			config=confobj;
		}
	}
}