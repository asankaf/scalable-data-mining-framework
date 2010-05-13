package ActionClasses
{
	public class AlgorithmClassification extends ActionObjectParent
	{

		public function AlgorithmClassification()
		{
			super("Clasification");
		}

		override public function type():Number
		{
			return ALGORITHM_CLASSIFICATION;
		}
	}
}