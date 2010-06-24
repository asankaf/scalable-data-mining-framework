#pragma once
#include "classifiertree.h"
#include "nosplit.h"

/************************************************************************
*    Class  :C45PruneableClassifierTree	  
*    Author :Amila De Silva
*    Subj   :
* Class for handling a tree structure used for
* classification.
*    Version: 1
************************************************************************/
class C45PruneableClassifierTree :
	public ClassifierTree
{
public:
	/**
	* Constructor. 
	*/
	_declspec(dllexport) C45PruneableClassifierTree(void);

	/**
	* Destructor. 
	*/
	_declspec(dllexport) virtual ~C45PruneableClassifierTree(void);

	/**
	* Constructor for pruneable tree structure. Stores reference
	* to associated training data at each node.
	*
	* toSelectLocModel selection method for local splitting model
	* pruneTree true if the tree is to be pruned
	* cf the confidence factor for pruning
	* raiseTree 
	* cleanup 
	*/
	_declspec(dllexport) C45PruneableClassifierTree(ModelSelection * _to_select_locModel,
		bool _prune_tree,
		float _cf,
		bool _raise_tree,
		bool _cleanup,
		bool _collapse_tree);

	/**
	* Method for building a pruneable classifier tree.
	*/
	_declspec(dllexport) void buildClassifier(DataSource * _source, BitStreamInfo * _existence_map);

	/**
	* Returns a newly created tree.	
	*/
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
	C45PruneableClassifierTree * son(int _index);

	/**
	* Computes estimated errors for one branch.
	*/
	double getEstimatedErrorsForBranch(DataSource * _data, BitStreamInfo * _existence); 
};