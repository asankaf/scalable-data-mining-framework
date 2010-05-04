package ActionClasses
{
	import mx.managers.PopUpManager;
	
	public class TreeViewer extends ActionObjectParent
	{
		public function TreeViewer()
		{
			super("Tree Viewer");
		}
		
		override public function type():Number
		{
			return TREE_VIEWER;
		}
	}
}