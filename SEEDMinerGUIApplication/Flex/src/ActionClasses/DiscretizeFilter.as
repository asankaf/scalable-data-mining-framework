package ActionClasses
{
	public class DiscretizeFilter extends ActionObjectParent
	{
		public function DiscretizeFilter()
		{
			super("Descritize");
		}
		override public function type():Number
		{
			return DESCRITIZE;
		}

	}
}