package ActionClasses
{
	import flash.utils.ByteArray;
	
	public class Util
	{
		public static function generateId():String
		{
			var i:int;
			var rnum:String="";
			var rand:int;
			for (i = 0; i < 20; i++)
			{
				rand=Math.ceil((Math.random())*10-1);
				if(i==0&&rand==0)
				{
					rand+=1;
				}
			    rnum+=rand.toString();
			}	
			return rnum;
		}
		
		public static function getShortestPath(source:ActionObject,destination:ActionObject):Path
		{			
			var width:int=source.vbox.width;
			var step:int=width/2;
			//trace("step "+step);
			
			var sourceX:int=source.vboxX;
			var sourceY:int=source.vboxY;
			var destinationX:int=destination.vboxX;
			var destinationY:int=destination.vboxY;
						
			var shorestPath:Path;
			
			for(var i:int=0;i<=2;i++)
			{
				for(var j:int=0;j<=2;j++)
				{
					for(var k:int=0;k<=2;k++)
					{
						for(var l:int=0;l<=2;l++)
						{
							if((i==1&&j==1)||(k==1&&l==1))
							{
							//	continue;
							}
							var currentPath:Path=new Path(sourceX+step*j,sourceY+step*i,destinationX+step*l,destinationY+step*k);
							if(shorestPath==null)
							{
								shorestPath=currentPath;
							}
							else if(currentPath.length()<shorestPath.length())
							{
								shorestPath=currentPath;
							}
							//trace(i+" "+j+" "+k+" "+l);
						}
					}
				}
			}
			return shorestPath;
		}

	}
}