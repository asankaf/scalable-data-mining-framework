package ActionClasses
{
	public class WAHCompression extends ActionObjectParent
	{
		public function WAHCompression()
		{
			super("WAH Compression");
		}
		
		override public function type():Number
		{
			return WAH_COMPRESSTION;
		}

	}
}