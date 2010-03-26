package ActionClasses
{
	public class CSVDataSource extends ActionObjectParent
	{	
		public function CSVDataSource()
		{
			super();
		}

		override public function type():Number
		{
			return CSV_DATASOURCE;
		}
		
	}
}