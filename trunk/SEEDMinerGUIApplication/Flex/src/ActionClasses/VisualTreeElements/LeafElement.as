package ActionClasses.VisualTreeElements
{
	import mx.containers.VBox;
	import mx.controls.Label;
	
	public class LeafElement extends VBox
	{
		public function LeafElement(lable:String)
		{
			super();
			
			this.setStyle("backgroundColor","#F3F3F3");
			this.setStyle("verticalAlign","middle");
			this.setStyle("horizontalAlign","center");
			this.setStyle("borderColor","#CDCDCD");
			this.setStyle("borderStyle","solid");
			this.setStyle("dropShadowColor","#DADADA");
			this.setStyle("dropShadowEnabled","true");
			this.setStyle("borderThickness","1");
			this.setStyle("cornerRadius","1");
			
			var labelObj:Label=new Label();
			labelObj.text=lable;
			labelObj.setStyle("fontSize",13);
			labelObj.setStyle("color","#787878");
			this.width=Element.estimateStringPixelLength(labelObj.text);
			this.height=Element.rootElementHeight;
			this.addChild(labelObj);
		}

	}
}