#include "StdAfx.h"
#include "C45Tree.h"


C45Tree::C45Tree(void)
{
	m_unpruned = false;	
	m_collapseTree = true;
	m_CF = 0.25;	
	m_minNumObj = 2;	
	m_useMDLcorrection = true;			
	m_binarySplits = false;
	m_subtreeRaising = true;
	m_noCleanup = false;	
}

C45Tree::~C45Tree(void)
{
}

void C45Tree::buildClassifier(WrapDataSource * instances)
{
	ModelSelection  * modSelection;	 

	if (m_binarySplits)
	{
		//modSelection = new BinC45ModelSelection(m_minNumObj, instances, m_useMDLcorrection);
		int i = 0;
	}
	else{
		//modSelection = new C45ModelSelection(m_minNumObj, instances, m_useMDLcorrection);
	}
	
// 		m_root = new C45PruneableClassifierTree(modSelection, !m_unpruned, m_CF,m_subtreeRaising, !m_noCleanup, m_collapseTree);
// 	
// 	m_root.buildClassifier(instances);
// 	if (m_binarySplits) {
// 		((BinC45ModelSelection)modSelection).cleanup();
// 	} else {
// 		((C45ModelSelection)modSelection).cleanup();
// 	}
}
