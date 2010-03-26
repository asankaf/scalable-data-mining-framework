package ActionClasses
{
	public class Util
	{
		public static function generateId():Number
		{
			var i:int;
			var rnum:String="";
			var rand:int;
			for (i = 0; i < 7; i++)
			{
				rand=Math.ceil((Math.random())*10-1);
				if(i==0&&rand==0)
				{
					rand+=1;
				}
			    rnum+=rand.toString();
			}	
			return parseInt(rnum);
		}

	}
}