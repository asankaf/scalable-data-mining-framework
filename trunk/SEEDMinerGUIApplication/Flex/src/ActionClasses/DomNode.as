package ActionClasses
{
	public class DomNode
	{
		var name:String;
		var child:Array;
		var parent:DomNode;
		var type:String;
		
		var x:int=0;
		var y:int=0;
		
		public function DomNode(name:String,type:String)
		{
			this.name=name;
			this.type=type;
			child=new Array();
		}
		public function toString():String
		{
			return name;
		}
	}
}