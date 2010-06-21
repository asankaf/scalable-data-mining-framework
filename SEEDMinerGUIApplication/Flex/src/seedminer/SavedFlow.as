package seedminer
{
	import flash.utils.Dictionary;
	
	public class SavedFlow
	{
		public var actionObjectsOnCanvas:Dictionary;
		public var actionObjectSequence:Array;
		public var arrowsOnCanvas:Array;
		
		public function SavedFlow(actionObjectsOnCanvas:Dictionary,actionObjectSequence:Array,arrowsOnCanvas:Array)
		{
			this.actionObjectsOnCanvas=actionObjectsOnCanvas;
			this.actionObjectSequence=actionObjectSequence;
			this.arrowsOnCanvas=arrowsOnCanvas;
		}
	}
}