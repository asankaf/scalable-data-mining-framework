package ActionClasses
{
	import ActionClasses.VisualTreeElements.Element;
	import ActionClasses.VisualTreeElements.LeafElement;
	import ActionClasses.VisualTreeElements.LinkElement;
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
		var levelDistance:int=100;
		var nodeDistance:int=Element.nodeDistance;
		
		var treeWidth:int=0;
		var treeHeight:int=0;
		var mostNegativeValue:int=0;
		var mostPositiveValue:int=0;
		
		public function GenerateGraphicalTree(dom:ClassificationDom,treePopUp:TreeViewPopUp)
		{
			treePopUpCanvas=treePopUp.drawingCanvas;
			
			
			dom.root.x=0;
			dom.root.y=0;
			calculateNodeCoordinates(dom.root);
			
			//tempLevel=0;
			//calculateTreeLevels(dom.root);
			//treePopUpCanvas.height=treeLevels*levelDistance+30;

			
			//tempLevel=0;
			//childrenInLevel=new Array(treeLevels+1);
			//initializeArray(childrenInLevel);
			//calculateChildrenInALevel(dom.root);
			//maxChildrenInALevel=childrenInLevel.sort(Array.DESCENDING)[0];
			//treePopUpCanvas.width=maxChildrenInALevel*150;
			
			//middle=treePopUpCanvas.width/2;	
				
			calculateTreeWidthAndHeightAndMostNegativeValue(dom.root);
			
			treeWidth=-mostNegativeValue+mostPositiveValue;
			calculateTreeLevels(dom.root);
			treeHeight=treeLevels*levelDistance;
			trace(treeHeight);
			
			if(treePopUp.width<treeWidth)
			{
				treePopUpCanvas.width=treeWidth+100;		
			}
			else 
			{
				treePopUpCanvas.width=treePopUp.width-200;		
			}
			if(treePopUp.height<treeWidth)
			{
				treePopUpCanvas.height=(treeHeight-1)*Element.nodeDistance+50;
			}
			else 
			{
				treePopUpCanvas.height=treePopUp.height-200;
			}		
			
			var add:int=treePopUp.width/2-(-mostNegativeValue+mostPositiveValue)/2;
			if(add<0)
			{
				add=-add;
			}
			var shiftX:int=-mostNegativeValue+add;
			var shiftY:int=50;
			
			dom.root.x=shiftX;
			dom.root.y=shiftY;
			
			calculateNodeCoordinates(dom.root);
			
			traverseAndDraw(dom.root);
		}
				
		public function calculateTreeWidthAndHeightAndMostNegativeValue(root:DomNode):void
		{
			if(root.x<mostNegativeValue)
			{
				mostNegativeValue=root.x;
			}
			if(mostPositiveValue<root.x)
			{
				mostPositiveValue=root.x;
			}

			for(var i:int=0;i<root.child.length;i++)
			{
				var child:DomNode=root.child[i];
				calculateTreeWidthAndHeightAndMostNegativeValue(child);
			}
		}
		
		public function calculateNodeCoordinates(root:DomNode):void
		{

			if(root.type!=NodeParent.LINK)
			{
				var start:int=root.x-((root.child.length-1)/2)*nodeDistance
				//trace(start);
				for(var j:int=0;j<root.child.length;j++)
				{
					var childLink:DomNode=DomNode(root.child[j]);
					//link sart
					childLink.start.x=root.x;
					childLink.start.y=root.y+25;
					//children
					var childNode:DomNode=childLink.child[0];
					childNode.y=root.y+1*levelDistance;				
					childNode.x=start+j*nodeDistance;
					//link end
					childLink.end.x=childNode.x;
					childLink.end.y=childNode.y;
				}
			} 
			
			for(var i:int=0;i<root.child.length;i++)
			{
				var child:DomNode=root.child[i];
				calculateNodeCoordinates(child);
			}
			
			
		}
		
		public function traverseAndDraw(root:DomNode):void
		{
			//if(root.type!=NodeParent.LINK)
			//{
				drawNode(root);
				
			//}

			for(var i:int=0;i<root.child.length;i++)
			{
				var child:DomNode=root.child[i];
				traverseAndDraw(child);
			}
		}
		
		public function drawNode(root:DomNode):void
		{
			if(root.type==NodeParent.ROOT||root.type==NodeParent.CHILD)
			{
				var nodeRoot:RootElement=new RootElement(root.name);
				nodeRoot.x=root.x-nodeRoot.width/2;
				nodeRoot.y=root.y;
				treePopUpCanvas.rawChildren.addChild(nodeRoot);
			}
			else if(root.type==NodeParent.LEAF)
			{
				var nodeLeaf:LeafElement=new LeafElement(root.name);
				nodeLeaf.x=root.x-nodeLeaf.width/2;
				nodeLeaf.y=root.y;
				treePopUpCanvas.rawChildren.addChild(nodeLeaf);
			}
			else if(root.type==NodeParent.LINK)
			{
				//trace(root.name);
				var nodeLink:LinkElement=new LinkElement(root.name,root.start,root.end);
				treePopUpCanvas.rawChildren.addChild(nodeLink.line);
				treePopUpCanvas.rawChildren.addChild(nodeLink.lable);
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