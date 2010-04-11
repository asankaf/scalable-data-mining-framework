#pragma once
#include "classifiertree.h"

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
};