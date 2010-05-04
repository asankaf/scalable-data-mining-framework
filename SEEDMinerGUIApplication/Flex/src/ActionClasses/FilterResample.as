package ActionClasses
{
	public class FilterResample extends ActionObjectParent
	{
		
		public function FilterResample()
		{
			super("Resample");
		}

		override public function type():Number
		{
			return FILTER_RESAMPLE;
		}

	}
}