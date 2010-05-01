package ActionClasses.VisualTreeElements
{
	import mx.containers.VBox;
	import mx.controls.Label;
	
	public class RootElement extends VBox
	{
		public function RootElement(lable:String)
		{
			super();
			
			this.setStyle("backgroundColor","#C4D8FA");
			this.setStyle("verticalAlign","middle");
			this.setStyle("horizontalAlign","center");
			this.setStyle("borderColor","#2B5191");
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