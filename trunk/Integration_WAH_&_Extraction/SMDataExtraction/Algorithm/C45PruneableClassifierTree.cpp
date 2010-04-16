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

C45PruneableClassifierTree::C45PruneableClassifierTree(ModelSelection * toSelectLocModel, bool pruneTree, float cf, bool raiseTree, bool cleanup, bool collapseTree):ClassifierTree(toSelectLocModel)
{
	//ClassifierTree::ClassifierTree(toSelectLocModel);
	m_pruneTheTree = pruneTree;
	m_CF = cf;
	m_subtreeRaising = raiseTree;
	m_cleanup = cleanup;
	m_collapseTheTree = collapseTree;
}

void C45PruneableClassifierTree::buildClassifier(DataSource * _source, BitStreamInfo * _existence_map) {

 	buildTree(_source, _existence_map,m_subtreeRaising);
// 	if (m_collapseTheTree) {
// 		collapse();
// 	}
// 	if (m_pruneTheTree) {
// 		prune();
// 	}
// 	if (m_cleanup) {
// 		cleanup(new Instances(data, 0));
// 	}
}

C45PruneableClassifierTree::~C45PruneableClassifierTree(void)
{
}
