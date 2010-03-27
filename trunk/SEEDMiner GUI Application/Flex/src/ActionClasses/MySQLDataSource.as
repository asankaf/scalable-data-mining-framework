package ActionClasses
{
	public class MySQLDataSource extends ActionObjectParent
	{	
		public function MySQLDataSource()
		{
			super("MySQL DataSource");
		}

		override public function type():Number
		{
			return MySQL_DATASOURCE;
		}
		
	}
}