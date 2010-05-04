package ActionClasses.VisualTreeElements
{
	
	public class Element
	{
		public static var rootElementHeight:int=25;
		public static var nodeTextCharacterWidth:int=9;
		public static var linkLableCharacterWidth:int=13;
		public static var nodeDistance:int=150;
		
		public static function estimateStringPixelLength(str:String):int
		{
			var estimatedLength:int=0;
			if(12<str.length)
			{
				for(var i:int=0;i<str.length;i++)
				{
					var char:String=str.charAt(i);
					if(char==".")
					{
						estimatedLength+=1;
					}
					else if(char=="i"||char=="I")
					{
						estimatedLength+=1;
					}
					else
					{
						estimatedLength+=9;
					}		
				}
			}
			else
			{
				estimatedLength=str.length*9;
			}
			
			
			return estimatedLength;
		}
	}
	
}