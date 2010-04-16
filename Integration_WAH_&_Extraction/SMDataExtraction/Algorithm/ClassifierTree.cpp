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

void ClassifierTree::buildClassifier( DataSource * data,BitStreamInfo * _existence_bitmap )
{
	//buildTree(data, false);
}

ClassifierTree * ClassifierTree::getNewTree(DataSource * _data, BitStreamInfo * _existence_map)
{
	ClassifierTree * newTree = new ClassifierTree(m_toSelectModel);
	newTree->buildTree(_data,_existence_map, false);

	return newTree;
}

void ClassifierTree::buildTree(DataSource * data, BitStreamInfo * _existence_map,boolean keepData )
{

	vector<BitStreamInfo *> localInstances;

	if (keepData) {
		m_train = data;
	}
	m_test = NULL;
	m_isLeaf = false;
	m_isEmpty = false;
	m_sons = NULL;
	m_localModel = m_toSelectModel->selectModel(data,_existence_map);
	m_localModel->Print();
 	if (m_localModel->NumSubsets() > 1) {
		localInstances = m_localModel->Existence_maps();
		m_sons = new ClassifierTree * [m_localModel->NumSubsets()];
		m_sonsLength = m_localModel->NumSubsets();
		for (int i = 0; i < m_sonsLength; i++) 
		{
			m_sons[i] = getNewTree(data,localInstances[i]);
			localInstances[i] = NULL;
		}
		m_sons_vect = vector<ClassifierTree *>(m_sons,m_sons+m_sonsLength);
	}
	else
	{
		m_isLeaf = true;
		if (Utils::eq((double)_existence_map->Count(), 0))
		{
			m_isEmpty = true;
		}
		data = NULL;
	}
}
