package ActionClasses
{
	public class Path
	{
		public var startX:int;
		public var startY:int;
		public var endX:int;
		public var endY:int;
		
		public function Path(sX:int,sY:int,eX:int,eY:int)
		{
			startX=sX;
			startY=sY;
			endX=eX;
			endY=eY;
		}
		
		public function length():int
		{
			return  Math.sqrt(Math.pow((this.startX-this.endX),2)+Math.pow((this.startY-this.endY),2));
		}
	}
}