package ActionClasses
{
	public class AlgorithmClassification extends ActionObjectParent
	{

		public function AlgorithmClassification()
		{
			super("Classification");
		}

		override public function type():Number
		{
			return ALGORITHM_CLASSIFICATION;
		}
	}
}