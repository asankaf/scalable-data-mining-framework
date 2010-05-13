package ActionClasses
{
	public class FilterResample extends ActionObjectParent
	{
		
		public function FilterResample()
		{
			super("Remove Null");
		}

		override public function type():Number
		{
			return FILTER_RESAMPLE;
		}

	}
}