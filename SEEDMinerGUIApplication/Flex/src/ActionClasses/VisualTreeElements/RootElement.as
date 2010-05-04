package ActionClasses.VisualTreeElements
{
	import mx.containers.VBox;
	import mx.controls.Label;
	
	public class RootElement extends VBox
	{
		public function RootElement(lable:String)
		{
			super();
			
			this.setStyle("backgroundColor","#EFF8FF");
			this.setStyle("verticalAlign","middle");
			this.setStyle("horizontalAlign","center");
			this.setStyle("borderColor","#C4D7FF");
			this.setStyle("borderStyle","solid");
			this.setStyle("dropShadowColor","#DADADA");
			this.setStyle("dropShadowEnabled","true");
			this.setStyle("borderThickness","1");
			this.setStyle("cornerRadius","8");
			
			var labelObj:Label=new Label();
			labelObj.text=lable;
			labelObj.setStyle("fontSize",13);
			labelObj.setStyle("color","#2080CB");
			this.width=Element.estimateStringPixelLength(labelObj.text);
			this.height=Element.rootElementHeight;
			this.addChild(labelObj);
		}

	}
}