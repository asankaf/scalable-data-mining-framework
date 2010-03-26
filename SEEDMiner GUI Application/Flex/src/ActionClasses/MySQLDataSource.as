package ActionClasses
{
	public class MySQLDataSource extends ActionObjectParent
	{	
		public function MySQLDataSource()
		{
			super();
		}

		override public function type():Number
		{
			return MySQL_DATASOURCE;
		}
		
	}
}