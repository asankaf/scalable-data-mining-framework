package ActionClasses
{
	import ActionClasses.VisualTreeElements.RootElement;
	
	import mx.containers.Canvas;
	
	public class GenerateGraphicalTree
	{
		var treePopUpCanvas:Canvas;
		var middle:int;
		var treeLevels:int=0;
		var maxChildrenInALevel:int=0;
		var childrenInLevel:Array;
		var tempLevel:int=0;
		var levelDistance:int=50;
		
		public function GenerateGraphicalTree(dom:ClassificationDom,treePopUp:TreeViewPopUp)
		{
			treePopUpCanvas=treePopUp.drawingCanvas;
			
			
			tempLevel=0;
			calculateTreeLevels(dom.root);
			treePopUpCanvas.height=treeLevels*levelDistance+30;

			
			tempLevel=0;
			childrenInLevel=new Array(treeLevels+1);
			initializeArray(childrenInLevel);
			calculateChildrenInALevel(dom.root);
			maxChildrenInALevel=childrenInLevel.sort(Array.DESCENDING)[0];
			treePopUpCanvas.width=maxChildrenInALevel*150;
			
			middle=treePopUpCanvas.width/2;	
			
			tempLevel=0;
			traverseAndDraw(dom.root);

		}
		
		public function traverseAndDraw(root:DomNode):void
		{
			tempLevel++;
			drawNode(root,tempLevel-1);
			
			for(var i:int=0;i<root.child.length;i++)
			{
				var child:DomNode=root.child[i];
				traverseAndDraw(child);
			}
			
			tempLevel--;
		}
		
		public function drawNode(root:DomNode,level:int):void
		{
			if(root.type==NodeParent.ROOT||root.type==NodeParent.CHILD)
			{
				var node:RootElement=new RootElement(root.name);
				node.x=calculateX(root,node);
				node.y=calculateY(level);
				treePopUpCanvas.rawChildren.addChild(node);
			}
		}
		
		public function calculateX(root:DomNode,node:RootElement):int
		{
			if(root.type==NodeParent.ROOT)
			{
				return middle-node.width/2;
			}
			return 10;
		}
		
		public function calculateY(level:int):int
		{
			return level*levelDistance;
		}
		
		public function calculateTreeLevels(root:DomNode):void
		{
			if(root.type!=NodeParent.LINK)
			{
				tempLevel++;
				//trace(root+" "+tempLevel);
				//trace(root+root.child.length);
				if(treeLevels<tempLevel)
				{
					treeLevels=tempLevel;
				}
			} 
			
			for(var i:int=0;i<root.child.length;i++)
			{
				var child:DomNode=root.child[i];
				calculateTreeLevels(child);
			}
			
			if(root.type!=NodeParent.LINK)
			{
				tempLevel--;
				//trace(root+" "+tempLevel);
			}
		}
		
		public function calculateChildrenInALevel(root:DomNode):void
		{
			if(root.type!=NodeParent.LINK)
			{
				tempLevel++;
				childrenInLevel[tempLevel]+=1;
			} 
			
			for(var i:int=0;i<root.child.length;i++)
			{
				var child:DomNode=root.child[i];
				calculateChildrenInALevel(child);
			}
			
			if(root.type!=NodeParent.LINK)
			{
				tempLevel--;
			}
		}
		
		public function initializeArray(arr:Array):void
		{
			for(var i:int=0;i<arr.length;i++)
			{
				arr[i]=0;
			}
		}
	}
}