#pragma once
#include "classifiertree.h"
#include "nosplit.h"
class C45PruneableClassifierTree :
	public ClassifierTree
{
public:
	_declspec(dllexport) C45PruneableClassifierTree(void);
	_declspec(dllexport) virtual ~C45PruneableClassifierTree(void);
	_declspec(dllexport) C45PruneableClassifierTree(ModelSelection * toSelectLocModel,
		bool pruneTree,
		float cf,
		bool raiseTree,
		bool cleanup,
		bool collapseTree);
	_declspec(dllexport) void buildClassifier(DataSource * _source, BitStreamInfo * _existence_map);
	ClassifierTree * getNewTree(DataSource * _data, BitStreamInfo * _existence_map);
	
	/**
	* Collapses a tree to a node if training error doesn't increase.
	*/
	void collapse();

	/**
	* Prunes a tree using C4.5's pruning procedure.
	*/
	void prune();

private:
	/** True if the tree is to be pruned. */
	bool m_pruneTheTree;

	/** True if the tree is to be collapsed. */
	bool m_collapseTheTree;

	/** The confidence factor for pruning. */
	float m_CF;

	/** Is subtree raising to be performed? */
	bool m_subtreeRaising;

	/** Cleanup after the tree has been built. */
	bool m_cleanup;

	/**
	* Computes errors of tree on training data.	
	*/
	double getTrainingErrors();

	/**
	* Method just exists to make program easier to read.	
	*/
	ClassifierSplitModel * C45PruneableClassifierTree::localModel();

	/**Deletes subtrees*/
	void clearSubtrees();

	/**
	* Method just exists to make program easier to read.
	*/
	C45PruneableClassifierTree * son(int index);

	/**
	* Computes estimated errors for one branch.
	*/
	double getEstimatedErrorsForBranch(DataSource * data, BitStreamInfo * _existence); 
};