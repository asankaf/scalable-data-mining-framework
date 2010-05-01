package ActionClasses
{
	public class ClassificationDom
	{
		public var root:DomNode;
		public function ClassificationDom(treeString:String)
		{
			
			var currentNode:DomNode;
			var currentLevel:int=1;
			var stack:Array=new Array();
			var originalString:Array=treeString.split(" ");
			var nospacesString:String=originalString.join("");
			var nodeString:Array = nospacesString.split( '\n' );
			
			var rootString:String =nodeString[0];
			createRoot(rootString);
			//trace(root);
			        
        	for ( var i:int = 0; i < nodeString.length; i++ ) {
        		
        		var currentString:String=nodeString[i];
          		//trace(currentString);
          		
          		if(currentString.charAt(0)!="|")
          		{
          			stack=new Array();
          			if(0<currentString.indexOf(":",0))
          			{
          				var child:DomNode=createLinkWithColon(currentString,root);         				
          				child.child.push(new DomNode(currentString.split(':')[1],NodeParent.LEAF));
          				currentNode=root;
          			}
          			else
          			{
          				var child:DomNode=createLinkNormal(currentString,root);;         				
						currentNode=child;
          			}
          		}
          		
          		else
          		{
          			var nextLevel:int=calculateLevel(currentString);
          			var extractedCurrentString:String=currentString.slice(nextLevel,currentString.length);
          			
          			if(0==stack.length)
          			{
          				var rootString:String =extractedCurrentString;
          				var _root:DomNode=createChildRoot(rootString);						
						
						currentNode.child.push(_root);
						currentNode=_root;
						
          				if(0<extractedCurrentString.indexOf(":",0))
	          			{
	          				var child:DomNode=createLinkWithColon(extractedCurrentString,currentNode);
	          				child.child.push(new DomNode(extractedCurrentString.split(':')[1],NodeParent.LEAF));
	          			}
	          			
	          			else
	          			{
	          				var child:DomNode=createLinkNormal(extractedCurrentString,currentNode);
	          			}
	          			
	          			stack.push(new StackObject(nextLevel,currentNode));
          			}
          			
          			else
          			{
          				var stackObj:StackObject=StackObject(stack.pop());
          				var stackTop:int=stackObj.level;
          				stack.push(stackObj);
          				
          				if(stackTop==nextLevel)
          				{
          					if(0<extractedCurrentString.indexOf(":",0))
		          			{
		          				var child:DomNode=createLinkWithColon(extractedCurrentString,currentNode);
		          				child.child.push(new DomNode(extractedCurrentString.split(':')[1],NodeParent.LEAF));
		          			}
		          			else
		          			{
		          				var child:DomNode=createLinkNormal(extractedCurrentString,currentNode);
		          			}
          				}
          				
          				else if(stackTop<nextLevel)
          				{
          					var rootString:String =extractedCurrentString;
	          				var _root:DomNode=createChildRoot(rootString);
								
							currentNode.child.push(_root);
							currentNode=_root;
							
	          				if(0<extractedCurrentString.indexOf(":",0))
		          			{
		          				var child:DomNode=createLinkWithColon(extractedCurrentString,currentNode);
		          				child.child.push(new DomNode(extractedCurrentString.split(':')[1],NodeParent.LEAF));
		          			}
		          			else
		          			{
		          				var child:DomNode=createLinkNormal(extractedCurrentString,currentNode);
		          			}
		          			stack.push(new StackObject(nextLevel,currentNode));
          				}
          				
          				else if(nextLevel<stackTop)
          				{	
          					var stackObj:StackObject;
	          				for(var j:int=0;j<stack.length;j++)
	          				{
	          					stackObj=StackObject(stack.pop());
	          					var stackTop:int=stackObj.level;
	          					if(stackTop==nextLevel)
	          					{
	          						stack.push(stackObj);
	          						break;
	          					}
	          				}

							currentNode=stackObj.node;
							
	          				if(0<extractedCurrentString.indexOf(":",0))
		          			{
		          				var child:DomNode=createLinkWithColon(extractedCurrentString,currentNode);
		          				child.child.push(new DomNode(extractedCurrentString.split(':')[1],NodeParent.LEAF));
		          			}
		          			else
		          			{
		          				var child:DomNode=createLinkNormal(extractedCurrentString,currentNode);
		          			}
          				}
          			}

          		}
        	}
        	//traverse(root);
		}
		
		
		
		
		
		
		public function traverse(root:DomNode):void
		{	
			trace(root+" "+root.type);
			for(var i:int = 0; i < root.child.length; i++ )
			{
				traverse(root.child[i]);
			}
		}
		public function calculateLevel(str:String):int
		{
			var count:int;
			for(var i:int = 0; i < str.length; i++ )
			{
				if(str.charAt(i)=="|")
          		{
          			count++;
          		}
          		else
          		{
          			break;
          		}
			}
			return count;
		}
		
		public function createRoot(rootString:String):void
		{
			if(0<rootString.indexOf('<=',0))
			{
				root=new DomNode(rootString.split('<=')[0],NodeParent.ROOT);
			}
			else if(0<rootString.indexOf('>',0))
			{
				root=new DomNode(rootString.split('>')[0],NodeParent.ROOT);
			}
			else
			{
				root=new DomNode(rootString.split('=')[0],NodeParent.ROOT);
			}
		}
		
		public function createChildRoot(rootString:String):DomNode
		{
			var _root:DomNode;
			if(0<rootString.indexOf('<=',0))
			{
				_root=new DomNode(rootString.split('<=')[0],NodeParent.CHILD);
			}
			else if(0<rootString.indexOf('>',0))
			{
				_root=new DomNode(rootString.split('>')[0],NodeParent.CHILD);
			}
			else
			{
				_root=new DomNode(rootString.split('=')[0],NodeParent.CHILD);
			}
			return _root;
		}
		
		public function createLinkWithColon(currentString:String,root:DomNode):DomNode
		{
			var child:DomNode;
			if(0<currentString.indexOf('<=',0))
			{
				child=new DomNode(currentString.split('<=')[1].split(':')[0],NodeParent.LINK);
				root.child.push(child);
			}
			else if(0<currentString.indexOf('>',0))
			{
				child=new DomNode(currentString.split('>')[1].split(':')[0],NodeParent.LINK)
				root.child.push(child);
			}
			else
			{
				child=new DomNode(currentString.split('=')[1].split(':')[0],NodeParent.LINK)
				root.child.push(child);
			}
			return child;
		}
		
		public function createLinkNormal(extractedCurrentString:String,currentNode:DomNode):DomNode
		{
			var child:DomNode;
			if(0<extractedCurrentString.indexOf('<=',0))
			{
				child=new DomNode(extractedCurrentString.split('<=')[1],NodeParent.LINK);
				currentNode.child.push(child);
			}
			else if(0<extractedCurrentString.indexOf('>',0))
			{
				child=new DomNode(extractedCurrentString.split('>')[1],NodeParent.LINK)
				currentNode.child.push(child);
			}
			else
			{
				child=new DomNode(extractedCurrentString.split('=')[1],NodeParent.LINK)
				currentNode.child.push(child);
			}
			return child;
		}
		
		public function createChild(currentString:String,root:DomNode):DomNode
		{
			var child:DomNode;
			if(0<currentString.indexOf('<=',0))
			{
				child=new DomNode(currentString.split('<=')[1],NodeParent.CHILD);
				root.child.push(child);
			}
			else if(0<currentString.indexOf('>',0))
			{
				child=new DomNode(currentString.split('>')[1],NodeParent.CHILD)
				root.child.push(child);
			}
			else
			{
				child=new DomNode(currentString.split('=')[1],NodeParent.CHILD)
				root.child.push(child);
			}
			return child;
		}
		
		
	}
}





