package ActionClasses
{
	public class AlgorithmNaiveBayes extends ActionObjectParent
	{
		public function AlgorithmNaiveBayes()
		{
			super("Naive Bayes");
		}

		override public function type():Number
		{
			return ALGORITHM_NAIVEBAYES;
		}
		
	}
}