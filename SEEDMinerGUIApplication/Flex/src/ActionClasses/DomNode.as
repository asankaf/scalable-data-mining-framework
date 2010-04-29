package ActionClasses
{
	public class DomNode
	{
		var name:String;
		var child:Array;
		var parent:DomNode;
		var type:int;
		public function DomNode(name:String,type:int)
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