#include "StdAfx.h"
#include "ClassifierTree.h"

ClassifierTree::ClassifierTree(void)
{
}

ClassifierTree::ClassifierTree(ModelSelection * toSelectLocModel)
{
	m_toSelectModel = toSelectLocModel;
}

ClassifierTree::~ClassifierTree(void)
{
}

void ClassifierTree::buildClassifier(WrapDataSource * data)
{
	//buildTree(data, false);
}

/**
* Builds the tree structure.
*
* @param data the data for which the tree structure is to be
* generated.
* @param keepData is training data to be kept?
* @throws Exception if something goes wrong
*/
void ClassifierTree::buildTree(WrapDataSource * data, boolean keepData)
{

// 	Instances [] localInstances;
// 
// 	if (keepData) {
// 		m_train = data;
// 	}
// 	m_test = null;
// 	m_isLeaf = false;
// 	m_isEmpty = false;
// 	m_sons = null;
// 	m_localModel = m_toSelectModel.selectModel(data);
// 	if (m_localModel.numSubsets() > 1) {
// 		localInstances = m_localModel.split(data);
// 		data = null;
// 		m_sons = new ClassifierTree [m_localModel.numSubsets()];
// 		for (int i = 0; i < m_sons.length; i++) {
// 			m_sons[i] = getNewTree(localInstances[i]);
// 			localInstances[i] = null;
// 		}
// 	}else{
// 		m_isLeaf = true;
// 		if (Utils.eq(data.sumOfWeights(), 0))
// 			m_isEmpty = true;
// 		data = null;
// 	}
}
