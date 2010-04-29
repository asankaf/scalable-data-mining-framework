package ActionClasses
{
	public class StackObject
	{
		var level:int;
		var node:DomNode;
		public function StackObject(level:int,node:DomNode)
		{
			this.level=level;
			this.node=node;
		}

	}
}