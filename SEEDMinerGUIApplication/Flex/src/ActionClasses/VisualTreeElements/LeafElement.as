package ActionClasses.VisualTreeElements
{
	import mx.containers.VBox;
	import mx.controls.Label;
	
	public class LeafElement extends VBox
	{
		public function LeafElement(lable:String)
		{
			super();
			
			this.setStyle("backgroundColor","#D5DAE3");
			this.setStyle("verticalAlign","middle");
			this.setStyle("horizontalAlign","center");
			this.setStyle("borderColor","#777777");
			this.setStyle("borderStyle","solid");
			this.setStyle("borderThickness","1");
			this.setStyle("cornerRadius","4");
			
			var labelObj:Label=new Label();
			labelObj.text=lable;
			labelObj.setStyle("fontSize",13);
			this.width=labelObj.text.length*17;
			this.height=25;
			this.addChild(labelObj);
		}

	}
}