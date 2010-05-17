package seedminer
{
	import mx.containers.VBox;
	import mx.controls.Label;
	import mx.states.SetStyle;
	
	public class TimePopUp extends VBox
	{
		public var myLable:Label;
		public function TimePopUp()
		{
			width=120;
			height=24;
			setStyle("cornerRadius","8");
			setStyle("backgroundColor","#F2E288");
			setStyle("borderColor","#FFFFFF");
			setStyle("borderStyle","solid");			
			setStyle("borderThickness","2");
			setStyle("verticalAlign","middle");
			setStyle("horizontalAlign","center");			
			setStyle("verticalGap","-2");
			alpha=.8;
			myLable=new Label();
			myLable.setStyle("textAlign","center");
			myLable.setStyle("color","#000000");
			myLable.setStyle("fontWeight","bold");
			myLable.setStyle("fontSize","12");
			addChild(myLable);
		}

	}
}