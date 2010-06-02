package ActionClasses
{
	public class MsSqlDataSource extends ActionObjectParent
	{
		
		public function MsSqlDataSource()
		{
			super("MSSQL DataSource");
		}

		override public function type():Number
		{
			return MSSQL_DATASOURCE;
		}
		
	}
}