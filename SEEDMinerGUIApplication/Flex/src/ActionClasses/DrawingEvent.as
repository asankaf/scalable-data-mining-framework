package ActionClasses
{
	public class DrawingEvent
	{
		private var _startId:String;
		private var _destinationId:String;
		
		public function DrawingEvent()
		{
		}
		
		public function get startId():String
		{
			return _startId;
		}
		
		public function set startId(from:String):void
		{
			_startId=from;
		}
		
		public function get destinationId():String
		{
			return _destinationId;
		}
		public function set destinationId(_to:String):void
		{
			_destinationId=_to;
		}
	}
}