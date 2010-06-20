package ActionClasses
{
	public class AlgorithmClassification extends ActionObjectParent
	{

		public function AlgorithmClassification()
		{
			super("C4.5");
		}

		override public function type():Number
		{
			return ALGORITHM_CLASSIFICATION;
		}
	}
}