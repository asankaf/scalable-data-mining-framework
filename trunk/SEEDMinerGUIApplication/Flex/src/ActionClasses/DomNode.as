package ActionClasses
{
	import flash.geom.Point;
	
	public class DomNode
	{
		var name:String;
		var child:Array;
		var parent:DomNode;
		var type:String;
		
		var x:int=0;
		var y:int=0;
		
		var start:Point;
		var end:Point;
		
		public function DomNode(name:String,type:String)
		{
			this.name=name;
			this.type=type;
			child=new Array();
			start=new Point();
			end=new Point();
		}
		public function toString():String
		{
			return name;
		}
	}
}