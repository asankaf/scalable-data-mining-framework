package ActionClasses
{
	import mx.managers.PopUpManager;
	
	public class TextViewer extends ActionObjectParent
	{
		public function TextViewer()
		{
			super("Text Viewer");
		}
		
		override public function type():Number
		{
			return TEXT_VIEWER;
		}

	}
}