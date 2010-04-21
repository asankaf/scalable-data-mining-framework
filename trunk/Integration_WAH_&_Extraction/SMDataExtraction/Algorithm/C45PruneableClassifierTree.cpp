#include "StdAfx.h"
#include "C45PruneableClassifierTree.h"


C45PruneableClassifierTree::C45PruneableClassifierTree(void)
{
	m_pruneTheTree = false;
	m_collapseTheTree = false;
	m_CF = 0.25f;
	m_subtreeRaising = true;
	m_cleanup = true;
}

ClassifierTree * C45PruneableClassifierTree::getNewTree(DataSource * _data, BitStreamInfo * _existence_map)
{
	C45PruneableClassifierTree * newTree = 
		new C45PruneableClassifierTree(m_toSelectModel, m_pruneTheTree, m_CF,
		m_subtreeRaising, m_cleanup, m_collapseTheTree);
	newTree->buildTree(_data, _existence_map,m_subtreeRaising);

	return newTree;
}

C45PruneableClassifierTree::C45PruneableClassifierTree(ModelSelection * toSelectLocModel, bool pruneTree, float cf, bool raiseTree, bool cleanup, bool collapseTree):ClassifierTree(toSelectLocModel)
{
	//ClassifierTree::ClassifierTree(toSelectLocModel);
	m_pruneTheTree = pruneTree;
	m_CF = cf;
	m_subtreeRaising = raiseTree;
	m_cleanup = cleanup;
	m_collapseTheTree = collapseTree;
}

C45PruneableClassifierTree * C45PruneableClassifierTree::son(int index)
{
	return dynamic_cast<C45PruneableClassifierTree *>(m_sons[index]);
}

void C45PruneableClassifierTree::collapse()
{
	double errorsOfSubtree;
	double errorsOfTree;
	int i;

	if (!m_isLeaf)
	{
		errorsOfSubtree = getTrainingErrors();
		errorsOfTree = localModel()->distribution()->numIncorrect();
		if (errorsOfSubtree >= errorsOfTree-1E-3){

			// Free adjacent trees
			clearSubtrees();
			m_isLeaf = true;

			// Get NoSplit Model for tree.
			m_localModel = new NoSplit(localModel()->distribution());
		}else{
			for (i=0;i<m_sonsLength;i++)
				son(i)->collapse();
		}
	}
}

ClassifierSplitModel * C45PruneableClassifierTree::localModel()
{

	return dynamic_cast<ClassifierSplitModel *>(m_localModel);
}

double C45PruneableClassifierTree::getTrainingErrors()
{
	double errors = 0;
	int i;

	if (m_isLeaf)
	{
		return localModel()->distribution()->numIncorrect();
	}
	else{
		for (i=0;i<m_sonsLength;i++)
		{
			errors = errors+son(i)->getTrainingErrors();
		}
		return errors;
	}
}

void C45PruneableClassifierTree::buildClassifier(DataSource * _source, BitStreamInfo * _existence_map) {

 	buildTree(_source, _existence_map,m_subtreeRaising);
	if (m_collapseTheTree) {
		collapse();
	}
	if (m_pruneTheTree) {
	//	prune();
	}
// 	if (m_cleanup) {
// 		cleanup(new Instances(data, 0));
// 	}
}


/*
void C45PruneableClassifierTree::prune()
{

	double errorsLargestBranch;
	double errorsLeaf;
	double errorsTree;
	int indexOfLargestBranch;
	C45PruneableClassifierTree * largestBranch;
	int i;

	if (!m_isLeaf){

		// Prune all subtrees.
		for (i=0;i<m_sonsLength;i++)
			son(i)->prune();

		// Compute error for largest branch
		indexOfLargestBranch = localModel()->distribution()->maxBag();
		if (m_subtreeRaising)
		{
			errorsLargestBranch = son(indexOfLargestBranch)->
				getEstimatedErrorsForBranch((Instances)m_train);
		} else {
			errorsLargestBranch = Double.MAX_VALUE;
		}

		// Compute error if this Tree would be leaf
		errorsLeaf = 
			getEstimatedErrorsForDistribution(localModel().distribution());

		// Compute error for the whole subtree
		errorsTree = getEstimatedErrors();

		// Decide if leaf is best choice.
		if (Utils.smOrEq(errorsLeaf,errorsTree+0.1) &&
			Utils.smOrEq(errorsLeaf,errorsLargestBranch+0.1)){

				// Free son Trees
				m_sons = null;
				m_isLeaf = true;

				// Get NoSplit Model for node.
				m_localModel = new NoSplit(localModel().distribution());
				return;
		}

		// Decide if largest branch is better choice
		// than whole subtree.
		if (Utils.smOrEq(errorsLargestBranch,errorsTree+0.1)){
			largestBranch = son(indexOfLargestBranch);
			m_sons = largestBranch.m_sons;
			m_localModel = largestBranch.localModel();
			m_isLeaf = largestBranch.m_isLeaf;
			newDistribution(m_train);
			prune();
		}
	}
}
*/

/*
double C45PruneableClassifierTree::getEstimatedErrorsForBranch(DataSource * data, BitStreamInfo * _existence) 
{
	Instances [] localInstances;
	double errors = 0;
	int i;

	if (m_isLeaf)
		return getEstimatedErrorsForDistribution(new Distribution(data));
	else{
		Distribution savedDist = localModel().m_distribution;
		localModel().resetDistribution(data);
		localInstances = (Instances[])localModel().split(data);
		localModel().m_distribution = savedDist;
		for (i=0;i<m_sons.length;i++)
			errors = errors+
			son(i).getEstimatedErrorsForBranch(localInstances[i]);
		return errors;
	}
}
*/

void C45PruneableClassifierTree::clearSubtrees()
{
	for (size_t i = 0 ; i < m_sonsLength ; i++)
	{
		delete m_sons[i];
		m_sons[i] = NULL;
	}
	m_sons = NULL;
}

C45PruneableClassifierTree::~C45PruneableClassifierTree(void)
{
}
